/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

static uint8 prg[4];
static uint8 chr[4];

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 4, "CREG" },
	{ 0 }
};

static void Sync(void) {
	setprg2r(0x10, 0x6800, 0);
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, prg[3]);
	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);
}

static DECLFR(M246Read) {
	if ((A & 0xFFE4) == 0xFFE4) {
		size_t prgOffset = ((prg[3] | 0x10) << 13) | 0x1000 | (A & 0xFFF);
		return PRGptr[0][prgOffset & 0x7FFFF];
	}
	return CartBR(A);
}

static DECLFW(M246Write) {
	if (A & 0x04) {
		chr[A & 0x03] = V;
		Sync();
	} else {
		prg[A & 0x03] = V;
		Sync();
	}
}

static void M246Power(void) {
	prg[0] = 0;
	prg[1] = 1;
	prg[2] = ~1;
	prg[3] = ~0;
	Sync();

	SetReadHandler(0x6800, 0x6FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, M246Read);
	SetWriteHandler(0x6000, 0x67FF, M246Write);
	SetWriteHandler(0x6800, 0x6FFF, CartBW);

	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M246Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper246_Init(CartInfo *info) {
	info->Power = M246Power;
	info->Close = M246Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 2048;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
}
