/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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
 * NES 2.0 Mapper 330 is used for a bootleg version of Contra/Gryzor.
 * as implemented from
 * http://forums.nesdev.org/viewtopic.php?f=9&t=17352&p=218722#p218722
 */

#include "mapinc.h"
#include "n163sound.h"

static uint8 IRQa;
static uint16 IRQCount;

static uint8 prg[4], chr[8], nt[4];

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ nt, 4, "NTAR" },
	{ 0 }
};

static void Sync(void) {
	int i;

	setprg8r(0x10, 0x6000, 0);
	setprg8(0xE000, ~0);

	for (i = 0; i < 3; i++) setprg8(0x8000 + (i << 13), prg[i]);
	for (i = 0; i < 8; i++) setchr1(i << 10, chr[i]);
	for (i = 0; i < 3; i++) setntamem(NTARAM + 0x400 * (nt[i] & 0x01), 1, i);
}

static DECLFW(M330WriteCHR) {
	if ((A & 0x400) && !(A & 0x4000)) {
		if (A & 0x2000) {
			IRQCount &= 0x00FF;
			IRQCount |= (V & 0x7F) << 8;
			IRQa = V & 0x80;
			X6502_IRQEnd(FCEU_IQEXT);
		} else {
			IRQCount &= 0xFF00;
			IRQCount |= V;
		}
	} else {
		int index = (A >> 11) & 0x07;
		chr[index] = V;
		Sync();
	}
}

static DECLFW(M330WriteNT) {
	if (!(A & 0x400)) {
		int index = (A >> 11) & 0x03;
		nt[index] = V;
		Sync();
	}
}

static DECLFW(M330WritePRG) {
	if ((A >= 0xF000) && (A & 0x800)) {
		N163Sound_Write(A, V);
	} else if (!(A & 0x400)) {
		int index = (A >> 11) & 0x03;
		prg[index] = V;
		Sync();
	}
}

static void M330Power(void) {
	int i;
	
	for (i = 0; i < 4; i++) prg[i] = i;
	for (i = 0; i < 8; i++) chr[i] = i;
	for (i = 0; i < 4; i++) nt[i] = (i >> 1) & 0x01;

	IRQa = IRQCount = 0;

	Sync();

	SetReadHandler(0x4800, 0x4FFF, N163Sound_Read);
	SetWriteHandler(0x4800, 0x4FFF, N163Sound_Write);

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);

	SetWriteHandler(0x8000, 0xBFFF, M330WriteCHR);
	SetWriteHandler(0xC000, 0xDFFF, M330WriteNT);
	SetWriteHandler(0xE000, 0xFFFF, M330WritePRG);
}

static void M330IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount > 0x7FFF) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
			IRQCount = 0;
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper330_Init(CartInfo *info) {
	info->Power = M330Power;
	MapIRQHook = M330IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAM = (uint8 *)FCEU_gmalloc(8192);
	SetupCartPRGMapping(0x10, WRAM, 8192, 1);
	AddExState(WRAM, 8192, 0, "WRAM");

	N163Sound_ESI();
}
