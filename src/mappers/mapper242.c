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
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 dipsw;

static void Sync(void) {
	uint32 prg = (latch.addr >> 2) & 0x1F;
	uint32 cpuA14 = latch.addr & 0x01;
	uint32 nrom = (latch.addr >> 7) & 0x01;
	uint32 unrom = (latch.addr >> 9) & 0x01;
    uint32 M242TwoChips = (iNESCart.PRGRomSize & 0x20000) && (iNESCart.PRGRomSize > 0x20000);

	if (M242TwoChips) {
		if (latch.addr & 0x600) { /* First chip */
			prg &= ((ROM.prg.size & ~0x08) - 1);
		} else { /* Second chip */
			prg &= 0x07;
			prg += (ROM.prg.size & ~0x08);
		}
	}

	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, prg & ~cpuA14);
	setprg16(0xC000, ((prg | cpuA14) & ~(0x07 * !nrom * !unrom)) | (0x07 * !nrom * unrom));

	setchr8(0);
	setmirror(((latch.addr >> 1) & 1) ^ 1);
	if (!iNESCart.battery && (latch.addr & 0x80) == 0x80 && (ROM.prg.size * 16) > 256) {
		/* CHR-RAM write protect hack, needed for some multicarts */
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 0);
	} else {
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 1);
	}
}

static DECLFR(M242Read) {
	if (latch.addr & 0x100) {
		A |= dipsw;
	}
	return CartBROB(A);
}

static void M242Power(void) {
	dipsw = 0;
	Latch_Power();
}

static void M242Reset(void) {
	dipsw = (dipsw + 1) & 0x1F;
	Latch_RegReset();
}

void Mapper242_Init(CartInfo *info) {
	Latch_Init(info, Sync, M242Read, TRUE, FALSE);
	info->Power = M242Power;
	info->Reset = M242Reset;
	AddExState(&dipsw, 1, 0, "PADS");
}
