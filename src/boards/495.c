/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

static uint8 prg[3];
static uint8 chr[4];
static uint8 latch[2];

static SFORMAT stateRegs[] = {
	{ prg,   3, "PRGR" },
	{ chr,   4, "CHRR" },
	{ latch, 2, "LATC" },
	{ 0 }
};

static void sync () {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000,   0xFF);
	setchr4(0x0000, chr[0 | latch[0]]);
	setchr4(0x1000, chr[2 | latch[1]]);
	switch(chr[latch[0]] >>6) {
		case 0: setmirrorw(0, 0, 0, 1); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_V); break;
		case 3: setmirror(MI_1); break;
	}
}

static void FP_FASTAPASS(1) trapPPUAddressChange (uint32 A) {
	if ((A &0x2FF0) == 0xFD0 || (A &0x2FF0) == 0xFE0) {
		latch[A >>12 &1] = ~A >>5 &1;
		sync();
	}
}

static DECLFW (writeReg) {
	if (A <0xE000)
		prg[A >>13 &3] = V;
	else
		chr[A >>10 &3] = V;
	sync();
}

static void restore (int version) {
	sync();
}

static void power () {
	prg[0] = prg[1] = prg[2] = 0;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	latch[0] = 0; latch[1] = 0;
	sync();
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
	PPU_hook = trapPPUAddressChange;
}

void Mapper495_Init (CartInfo *info) {
	AddExState(stateRegs, ~0, 0, 0);
	info->Power = power;
	info->Reset = sync;
	GameStateRestore = restore;
}
