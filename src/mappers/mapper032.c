/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

static uint8 preg[2], creg[8], mirr;

static uint8 *WRAM = NULL;

#ifndef WRAM_SIZE
#define WRAM_SIZE 8192
#endif

static SFORMAT StateRegs[] =
{
	{ preg, 4, "PREG" },
	{ creg, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void M32Sync(void) {
	uint8 i;
	uint16 swap = ((mirr & 2) << 13);
	setmirror((mirr & 1) ^ 1);
	setprg8r(0x10, 0x6000, 0);
	setprg8(0x8000 ^ swap, preg[0]);
	setprg8(0xA000, preg[1]);
	setprg8(0xC000 ^ swap, ~1);
	setprg8(0xE000, ~0);
	for (i = 0; i < 8; i++)
		setchr1(i << 10, creg[i]);
}

static void M32Write0(uint32 A, uint8 V) {
	preg[0] = V;
	M32Sync();
}

static void M32Write1(uint32 A, uint8 V) {
	mirr = V;
	M32Sync();
}

static void M32Write2(uint32 A, uint8 V) {
	preg[1] = V;
	M32Sync();
}

static void M32Write3(uint32 A, uint8 V) {
	creg[A & 7] = V;
	M32Sync();
}

static void M32Power(void) {
	M32Sync();
	SetReadHandler(0x6000, 0x7fff, CartBR);
	SetWriteHandler(0x6000, 0x7fff, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x8FFF, M32Write0);
	SetWriteHandler(0x9000, 0x9FFF, M32Write1);
	SetWriteHandler(0xA000, 0xAFFF, M32Write2);
	SetWriteHandler(0xB000, 0xBFFF, M32Write3);
        FCEU_CheatAddRAM(WRAM_SIZE >> 10, 0x6000, WRAM);
}

static void M32Close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void M32StateRestore(int version) {
	M32Sync();
}

void Mapper32_Init(CartInfo *info) {
	info->Power = M32Power;
	info->Close = M32Close;
	GameStateRestore = M32StateRestore;

	WRAM = (uint8*)FCEU_gmalloc(WRAM_SIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAM_SIZE, 1);
	AddExState(WRAM, WRAM_SIZE, 0, "WRAM");

	AddExState(&StateRegs, ~0, 0, 0);
}
