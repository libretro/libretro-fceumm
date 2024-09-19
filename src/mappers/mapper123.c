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

/* iNES Mapper 123 - UNL-H2288 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M123PW(uint16 A, uint16 V) {
	if (reg & 0x40) {
		uint8 bank = ((reg & 0x28) >> 2) | (reg & 0x05);

		if (reg & 2) {
			setprg32(0x8000, bank >> 1);
		} else {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else {
		setprg8(A, V & 0x3F);
	}
}

static DECLFW(M123WriteHi) {
	static const uint8 m114_perm[8] = { 0, 3, 1, 5, 6, 7, 2, 4 };

	if (!(A & 0x01)) {
		V = (V & 0xC0) | m114_perm[V & 0x07];
	}

	MMC3_CMDWrite(A, V);
}

static DECLFW(M123WriteLo) {
	if (A & 0x800) {
		reg = V;
		MMC3_FixPRG();
	}
}

static void M123Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M123WriteLo);
	SetWriteHandler(0x8000, 0x9FFF, M123WriteHi);
}

void Mapper123_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M123PW;
	info->Power = M123Power;
	AddExState(&reg, 1, 0, "EXPR");
}
