/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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
 *
 * PCB-018 board, discrete multigame cart 110-in-1
 *
 * Mapper 225
 * Mapper 255
 *
 */

/* 2020-2-20 - merge mapper 255, re-implement extra RAM */

#include "mapinc.h"
#include "latch.h"

static uint8 extraRAM[4];

static SFORMAT StateRegs[] = {
	{ extraRAM, 4, "PROT" },
	{ 0 }
};

static void Sync(void) {
	uint8 base = (latch.addr >> 8) & 0x40; 
	uint8 prg  = (latch.addr >> 6) & 0x3F;
	uint8 chr  = latch.addr & 0x3F;
	uint8 mirr = ((latch.addr >> 13) & 1) ^ 1;

	if (latch.addr & 0x1000) {
		setprg16(0x8000, base | prg);
		setprg16(0xC000, base | prg);
	} else {
		setprg32(0x8000, (base | prg) >> 1);
	}
	setchr8(base | chr);
	setmirror(mirr);
}

static DECLFW(M225LoWrite) {
	/* e.g. 115-in-1 [p1][!] CRC32 0xb39d30b4 */
	if (A & 0x800) {
		extraRAM[A & 3] = V & 0x0F;
	}
}

static DECLFR(M225LoRead) {
	if (A & 0x800) {
		return extraRAM[A & 3];
	}
	return cpu.openbus;
}

static void M225Power(void) {
	Latch_Power();
	SetReadHandler(0x5000, 0x5fff, M225LoRead);
	SetWriteHandler(0x5000, 0x5fff, M225LoWrite);
}

void Mapper225_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M225Power;
	AddExState(StateRegs, ~0, 0, NULL);
}

void Mapper255_Init(CartInfo *info) {
	Mapper225_Init(info);
}