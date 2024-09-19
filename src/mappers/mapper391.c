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

/* BS-110 PCB, previously called NC7000M due to a mix-up. */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M391PW(uint16 A, uint16 V) {
	uint8 mask = (reg[0] & 0x08) ? 0x0F : 0x1F;
	uint8 base = (reg[0] << 4) & 0x30;

	if (reg[0] & 0x20) {
		if (reg[0] & 0x04) { /* NROM-256 */
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~0x03) | 0);
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & mask) & ~0x03) | 1);
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~0x03) | 2);
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] & mask) & ~0x03) | 3);
		} else { /* NROM-128 */
			setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~0x01) | 0);
			setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & mask) & ~0x01) | 1);
			setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] & mask) & ~0x01) | 0);
			setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] & mask) & ~0x01) | 1);
		}
	} else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M391CW(uint16 A, uint16 V) {
	uint16 mask = (reg[0] & 0x40) ? 0x7F : 0xFF;
	uint16 base = ((reg[0] << 3) & 0x80) | ((reg[1] << 8) & 0x100);

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW(M391Write) {
	if (MMC3_WramIsWritable()) {
		if (!(reg[0] & 0x80)) {
			reg[0] = V;
			reg[1] = ((A >> 8) & 0xFF);
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M391Reset(void) {
	reg[0] = reg[1] = 0;
	MMC3_Reset();
}

static void M391Power(void) {
	reg[0] = reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M391Write);
}

void Mapper391_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M391CW;
	MMC3_pwrap = M391PW;
	info->Power = M391Power;
	info->Reset = M391Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
