/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * Dance 2000 12-in-1
 *
 */

#include "mapinc.h"

static uint8 prg, mode;
static uint32 lastnt = 0;

static SFORMAT StateRegs[] =
{
	{ &prg, 1, "REGS" },
	{ &mode, 1, "MODE" },
	{ &lastnt, 4, "LSNT" },
	{ 0 }
};

static void Sync(void) {
	setmirror((mode ^ 1) & 1);
	setprg8r(0x10, 0x6000, 0);
	setchr4(0x0000, lastnt);
	setchr4(0x1000, 1);
	if (mode & 4) {
		setprg32(0x8000, prg & 7);
	} else {
		setprg16(0x8000, prg & 0x0f);
		setprg16(0xC000, 0);
	}
}

static DECLFW(M518Write) {
	switch (A) {
	case 0x5000: prg = V; Sync(); break;
	case 0x5200: mode = V; if (mode & 4) Sync(); break;
	}
}

static DECLFR(M518Read) {
	if (prg & 0x40)
		return cpu.openbus;
	else
		return CartBR(A);
}

static void M518Power(void) {
	prg = mode = 0;
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, M518Read);
	SetWriteHandler(0x5000, 0x5FFF, M518Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M518PPUHook(uint32 A) {
	if (mode & 2) {
		if ((A & 0x3000) == 0x2000) {
			uint32 curnt = A & 0x800;
			if (curnt != lastnt) {
				setchr4(0x0000, curnt >> 11);
				lastnt = curnt;
			}
		}
	} else {
		lastnt = 0;
		setchr4(0x0000, 0);
	}
}

static void M518Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper518_Init(CartInfo *info) {
	info->Power = M518Power;
	info->Close = M518Close;
	PPU_hook = M518PPUHook;
	GameStateRestore = StateRestore;

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	AddExState(StateRegs, ~0, 0, NULL);
}
