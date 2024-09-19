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
 *
 * GG1 boards, similar to T-262, with no Data latch
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	uint8 prg   = (latch.addr >> 2) & 0x1F;
	uint8 nrom  = (latch.addr & 0x80) != 0;
	uint8 unrom = (latch.addr & 0x200) != 0;

	setprg16(0x8000, prg);
	setprg16(0xC000, (prg & ~(0x07 * nrom)) | (0x07 * unrom));
	setchr8(0);
	setmirror(((latch.addr & 2) >> 1) ^ 1);
}

static DECLFR(M301Read) {
	if ((latch.addr & 0x100) && (PRGsize[0] <= (512 * 1024))) {
		A = (A & 0xFFFE) + (dipsw & 0x01);
	}
	return CartBR(A);
}

static void M301Power(void) {
	dipsw = 0;
	Latch_Power();
}

static void M301Reset(void) {
	dipsw++;
	Latch_RegReset();
}

void Mapper301_Init(CartInfo *info) {
	Latch_Init(info, Sync, M301Read, FALSE, FALSE);
	info->Power = M301Power;
	info->Reset = M301Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
