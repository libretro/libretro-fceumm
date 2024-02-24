/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 * DIS23C01 DAOU ROM CONTROLLER, Korea
 * Metal Force (K)
 * Buzz and Waldog (K)
 * General's Son (K)
 *
 */

#include "mapinc.h"

static uint8 prg, mirr;
static uint16 chr[8];

static SFORMAT StateRegs[] = {
	{ chr, 16, "CREG" },
	{ &prg, 1, "PREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, prg);
	setprg16(0xC000, ~0);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	switch (mirr) {
	case 0:  setmirror(MI_V); break;
	case 1:  setmirror(MI_H); break;
	default: setmirror(MI_0); break;
	}
}

static DECLFW(M156Write) {
	uint8 index = A & 0x03;

	switch (A & 0xFFFC) {
	case 0xC000:
		chr[0 | index] &= (chr[0 | index] & 0xFF00) | V;
		Sync();
		break;
	case 0xC004:
		chr[0 | index] = (chr[0 | index] & 0x00FF) | V << 8;
		Sync();
		break;
	case 0xC008:
		chr[4 | index] = (chr[4 | index] & 0xFF00) | V;
		Sync();
		break;
	case 0xC00C:
		chr[4 | index] = (chr[4 | index] & 0x00FF) | V << 8;
		Sync();
		break;
	case 0xC010:
		prg = V;
		Sync();
		break;
	case 0xC014:
		mirr = V;
		Sync();
		break;
	}
}

static void M156Reset(void) {
	uint32 i;
	for (i = 0; i < 8; i++) {
		chr[i] = 0;
	}
	prg = 0;
	mirr = 2;
}

static void M156Power(void) {
	M156Reset();
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0xC000, 0xCFFF, M156Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M156Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper156_Init(CartInfo *info) {
	info->Reset = M156Reset;
	info->Power = M156Power;
	info->Close = M156Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
