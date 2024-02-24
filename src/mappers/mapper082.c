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
 *
 * Taito X1-017 board, battery backed
 * NES 2.0 Mapper 552 represents the actual way the mask ROM is connected and is thus
 * the correct bank order, while iNES Mapper 082 represents the bank order as it was
 * understood before January 2020 when the mapper was reverse-engineered.
 */

#include "mapinc.h"

static uint8 chr[6], prg[3], protect[3], ctrl;

static SFORMAT StateRegs[] = {
	{ prg, 3, "PREGS" },
	{ chr, 6, "CREGS" },
	{ protect, 3, "PROT" },
	{ &ctrl, 1, "CTRL" },

	{ 0 }
};

static uint32 GetPRGBank(uint8 V) {
	if (iNESCart.mapper == 552) {
		return (((V << 5) & 0x20) |
		((V << 3) & 0x10) |
		((V << 1) & 0x08) |
		((V >> 1) & 0x04) |
		((V >> 3) & 0x02) |
		((V >> 5) & 0x01));
	}
	return V >> 2;
}

static void Sync(void) {
	uint32 swap = ((ctrl & 2) << 11);

	setprg8r(0x10, 0x6000, 0);

	setprg8(0x8000, GetPRGBank(prg[0]));
	setprg8(0xA000, GetPRGBank(prg[1]));
	setprg8(0xC000, GetPRGBank(prg[2]));
	setprg8(0xE000, ~0);

	setchr2(0x0000 ^ swap, chr[0] >> 1);
	setchr2(0x0800 ^ swap, chr[1] >> 1);
	setchr1(0x1000 ^ swap, chr[2]);
	setchr1(0x1400 ^ swap, chr[3]);
	setchr1(0x1800 ^ swap, chr[4]);
	setchr1(0x1C00 ^ swap, chr[5]);

	setmirror(ctrl & 0x01);
}

static DECLFR(ReadWRAM) {
	if (((A >= 0x6000) && (A <= 0x67FF) && (protect[0] == 0xCA)) ||
	    ((A >= 0x6800) && (A <= 0x6FFF) && (protect[1] == 0x69)) ||
	    ((A >= 0x7000) && (A <= 0x73FF) && (protect[2] == 0x84))) {
		return CartBR(A);
	}
	return cpu.openbus;
}

static DECLFW(WriteWRAM) {
	if (((A >= 0x6000) && (A <= 0x67FF) && (protect[0] == 0xCA)) ||
	    ((A >= 0x6800) && (A <= 0x6FFF) && (protect[1] == 0x69)) ||
	    ((A >= 0x7000) && (A <= 0x73FF) && (protect[2] == 0x84))) {
		CartBW(A, V);
	}
}

static DECLFW(M082Write) {
	switch (A & 0x0F) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05: chr[A & 7] = V; break;
	case 0x06: ctrl = V & 3; break;
	case 0x07: protect[0] = V; break;
	case 0x08: protect[1] = V; break;
	case 0x09: protect[2] = V; break;
	case 0x0A: prg[0] = V; break;
	case 0x0B: prg[1] = V; break;
	case 0x0C: prg[2] = V; break;
	default:
		/* IRQ emulation ignored since no commercial games uses it */
		return;
	}
	Sync();
}

static void M082Power(void) {
	Sync();
	SetReadHandler(0x6000, 0xffff, CartBR);
	SetReadHandler(0x6000, 0x73ff, ReadWRAM);
	SetWriteHandler(0x6000, 0x73ff, WriteWRAM);
	SetWriteHandler(0x7ef0, 0x7eff, M082Write); /* external WRAM might end at $73FF */
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M082Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper082_Init(CartInfo *info) {
	info->Power = M082Power;
	info->Close = M082Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
}

void Mapper552_Init(CartInfo *info) {
	Mapper082_Init(info);
}
