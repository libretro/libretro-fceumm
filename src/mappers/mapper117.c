/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

static uint8 prg[4], chr[8], mirr;
static uint8 IRQa, IRQCount, IRQLatch;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ &mirr, 1, "MREG" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, ~0);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirror(mirr ^ 1);
}

static DECLFW(M117Write) {
	switch (A & 0xF000) {
	case 0x8000:
		prg[A & 0x03] = V;
		Sync();
		break;
	case 0xA000:
		switch (A & 0x0F) {
		case 0: case 1: case 2: case 3:
		case 4: case 5: case 6: case 7:
			chr[A & 0x07] = V;
			Sync();
			break;
		}
		break;
	case 0xC000:
		switch (A & 0x03) {
		case 1:
			IRQLatch = V;
			break;
		case 3:
			IRQCount = IRQLatch;
			IRQa |= 2;
			break;
		}
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xD000:
		mirr = V & 1;
		Sync();
		break;
	case 0xE000:
		IRQa &= ~1;
		IRQa |= V & 1;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	}
}

static void M117Power(void) {
	prg[0] = ~3;
	prg[1] = ~2;
	prg[2] = ~1;
	prg[3] = ~0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M117Write);
}

static void M117IRQHook(void) {
	if (IRQa == 3 && IRQCount) {
		IRQCount--;
		if (!IRQCount) {
			IRQa &= 1;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper117_Init(CartInfo *info) {
	info->Power = M117Power;
	GameHBIRQHook = M117IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
