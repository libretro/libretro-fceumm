/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[2];
static uint8 dipsw;

static void M432CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x04) ? 0x7F : 0xFF;
	uint16 base  = ((reg[1] << 7) & 0x080) | ((reg[1] << 5) & 0x100) | ((reg[1] << 4) & 0x200);

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M432PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0x0F : 0x1F;
	uint16 base = ((reg[1] << 4) & 0x10) | ((reg[1] << 1) & 0x60);

	if (reg[1] & 0x40) { /* NROM */
		if (reg[1] & 0x80) { /* NROM-256 */
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & ~0x02) & mask));
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & ~0x02) & mask));
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] |  0x02) & mask));
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] |  0x02) & mask));
		} else { /* NROM-128 */
			setprg8(0x8000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xA000, (base & ~mask) | (mmc3.reg[7] & mask));
			setprg8(0xC000, (base & ~mask) | (mmc3.reg[6] & mask));
			setprg8(0xE000, (base & ~mask) | (mmc3.reg[7] & mask));
		}
	} else { /* MMC3 */
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static DECLFR(M432Read) {
	if ((reg[0] & 0x01) || ((reg[1] & 0x20) && ((ROM.prg.size * 16) < 1024))) {
		return dipsw;
	}
	return CartBR(A);
}

static DECLFW(M432Write) {
	if (MMC3_WramIsWritable()) {
		reg[A & 0x01] = V;
		if (!(A & 0x01) && !(V & 0x01) && ((ROM.prg.size * 16) < 1024)) {
			reg[1] &= ~0x20; /* Writing 0 to register 0 clears register 1's DIP bit */
		}
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M432Reset(void) {
	reg[0] = 0;
	reg[1] = 0;
	dipsw++;
	MMC3_Reset();
}

static void M432Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	dipsw = 0;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M432Read);
	SetWriteHandler(0x6000, 0x7FFF, M432Write);
}

void Mapper432_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M432CW;
	MMC3_pwrap = M432PW;
	info->Power = M432Power;
	info->Reset = M432Reset;
	AddExState(reg, 2, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "CMD0");
}
