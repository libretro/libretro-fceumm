/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* Chipset used on various PCBs named WX-KB4K, T4A54A, BS-5652... */
/* "Rockman 3" on YH-322 and "King of Fighters 97" on "Super 6-in-1" enable interrupts without initializing the frame IRQ register and therefore freeze on real hardware.
   They can run if another game is selected that does initialize the frame IRQ register, then soft-resetting to the menu and selecting the previously-freezing games. */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M134PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x04) ? 0x0F : 0x1F;
	uint16 base = ((reg[1] << 4) & 0x30) | ((reg[0] << 2) & 0x40);

	if (reg[1] & 0x80) { /* NROM mode */
		if (reg[1] & 0x08) { /* NROM-128 mode */
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~1) | 0);
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~1) | 1);
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~1) | 0);
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~1) | 1);
		} else { /* NROM-256 mode */
			setprg8(0x8000, (base & ~mask) | (((mmc3.reg[6] & ~0x02) & mask) & ~1) | 0);
			setprg8(0xA000, (base & ~mask) | (((mmc3.reg[6] & ~0x02) & mask) & ~1) | 1);
			setprg8(0xC000, (base & ~mask) | (((mmc3.reg[6] |  0x02) & mask) & ~1) | 0);
			setprg8(0xE000, (base & ~mask) | (((mmc3.reg[6] |  0x02) & mask) & ~1) | 1);
		}
	} else { /* MMC3 */
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M134CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x40) ? 0x7F : 0xFF;
	uint16 base = ((reg[1] << 3) & 0x180) | ((reg[0] << 4) & 0x200);

	if (reg[0] & 0x08) { /* In CNROM mode, outer bank register 2 replaces the MMC3's CHR registers, and CHR A10-A12 are PPU A10-A12. */
		setchr8(((base & ~mask) >> 3) | (reg[2] & (mask >> 3)));
	} else {
		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static DECLFR(M134Read) {
	if (reg[0] & 0x40) {
		return dipsw;
	}
	return CartBR(A);
}

static DECLFW(M134Write) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
		if (!(reg[0] & 0x80)) {
			reg[A & 0x03] = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
		} else if ((A & 0x03) == 2) {
			reg[2] = (reg[2] & ~0x03) | (V & 0x03);
			MMC3_FixCHR();
		}
	}
}

static void M134Reset(void) {
	dipsw++;
	dipsw &= 15;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
}

static void M134Power(void) {
	dipsw = 0;
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M134Write);
	SetReadHandler(0x8000, 0xFFFF, M134Read);
}

void Mapper134_Init(CartInfo *info) {
	MMC3_Init(info, info->iNES2 ? (info->PRGRamSize + info->PRGRamSaveSize) / 1024 : 8, info->battery);
	MMC3_cwrap = M134CW;
	MMC3_pwrap = M134PW;
	info->Power = M134Power;
	info->Reset = M134Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
