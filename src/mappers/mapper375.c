/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "mapinc.h"
#include "latch.h"

static void Sync(void) {
	uint32 prg = ((latch.addr >> 4) & 0x40) | ((latch.addr >> 3) & 0x20) | ((latch.addr >> 2) & 0x1F);
	uint32 cpuA14 = latch.addr & 0x01;
	uint32 nrom = (latch.addr >> 7) & 0x01;
	uint32 unrom = (latch.addr >> 9) & 0x01;
	uint32 unrom_like = (latch.addr >> 11) & 0x01;

	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, ((prg & ~cpuA14) & ~(0x07 * unrom_like)) | (unrom_like * latch.data));
	setprg16(0xC000, ((prg | cpuA14) & ~(0x07 * !nrom * !unrom)) | (0x07 * !nrom * unrom));

	setchr8(0);
	setmirror(((latch.addr >> 1) & 1) ^ 1);
	if ((latch.addr & 0x80) == 0x80) {
		/* CHR-RAM write protect hack, needed for some multicarts */
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 0);
	} else {
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 1);
	}
}

static DECLFW(M375Write) {
	if (latch.addr & 0x800) {
		latch.data = V;
	} else {
		latch.addr = A;
		latch.data = V;
	}
	Sync();
}

static void M375Power(void) {
	Latch_Power();
	SetWriteHandler(0x8000, 0xFFFF, M375Write);
}

void Mapper375_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, TRUE, FALSE);
	info->Power = M375Power;
	info->Reset = Latch_RegReset;
}
