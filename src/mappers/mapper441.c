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

/* 841026C and 850335C multicart circuit boards */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M441PW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x08) ? 0x0F : 0x1F;
	uint8 base  = (reg << 4) & 0x30;

	if (reg & 0x04) {
		setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & ~0x02) & mask));
		setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & ~0x02) & mask));
		setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] |  0x02) & mask));
		setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] |  0x02) & mask));
	} else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M441CW(uint16 A, uint16 V) {
	uint16 mask = (reg & 0x40) ? 0x7F : 0xFF;
	uint16 base  = (reg << 3) & 0x180;

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW(M441Write) {
	if (MMC3_WramIsWritable()) {
		if (!(reg & 0x80)) {
			reg = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M441Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M441Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M441Write);
}

void Mapper441_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M441CW;
	MMC3_pwrap = M441PW;
	info->Power = M441Power;
	info->Reset = M441Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
