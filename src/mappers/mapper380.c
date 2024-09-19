/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2023
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

/* NES 2.0 Mapper 380 denotes the 970630C circuit board,
 * used on a 512 KiB multicart having 42 to 80,000 listed NROM and UNROM games. */

#include "mapinc.h"
#include "latch.h"

static uint8 dipsw;

static SFORMAT StateRegs[] = {
   { &dipsw, 1, "DPSW" },
   { 0 }
};

static void Sync(void) {
	uint32 prg = (latch.addr >> 2) & 0x1F;
	uint32 cpuA14 = (latch.addr & 0x01) != 0x01;
	uint32 ourom = (latch.addr >> 8) & 0x01;
	uint32 nrom = (latch.addr >> 9) & 0x01;

	setprg16(0x8000, prg & ~(cpuA14 * nrom));
	setprg16(0xC000, (prg | (cpuA14 * nrom)) | (0x07 * !nrom) | (0x08 * (iNESCart.submapper == 1) * !nrom * ourom));

	setchr8(0);
	setmirror(((latch.addr >> 1) & 0x01) ^ 0x01);
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, !(latch.addr & 0x80));
}

static DECLFR(M380Read) {
	if ((iNESCart.submapper == 0) && (latch.addr & 0x100)) {
		A |= dipsw;
	}
	return CartBR(A);
}

static void M380Reset(void) {
	dipsw = (dipsw + 1) & 0xF;
	Latch_RegReset();
}

void Mapper380_Init(CartInfo *info) {
	Latch_Init(info, Sync, M380Read, FALSE, FALSE);
	info->Reset = M380Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
