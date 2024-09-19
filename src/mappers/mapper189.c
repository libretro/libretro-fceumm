/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

static uint8 reg;

static void M189PW(uint16 A, uint16 V) {
	setprg32(0x8000, reg | (reg >> 4));
}

static DECLFW(M189Write4) {
	if (A & 0x100) {
		reg = V;
		MMC3_FixPRG();
	}
}

static DECLFW(M189Write6) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
	}
}

static void M189Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x4120, 0x5FFF, M189Write4);
	SetWriteHandler(0x6000, 0x7FFF, M189Write6);
}

void Mapper189_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M189PW;
	info->Power = M189Power;
	AddExState(&reg, 1, 0, "EXPR");
}
