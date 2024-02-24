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

static uint8 prg[2], chr[8], mirr, cmd;
static uint8 IRQa;
static int16 IRQCount, IRQLatch;

static SFORMAT StateRegs[] = {
	{ &cmd, 1, "CMD0" },
	{ prg, 2, "PREG" },
	{ chr, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQLatch, 2, "IRQL" },
	{ 0 }
};

static void Sync(void) {
	if (cmd & 0x80) {
		setprg8(0x8000, ~1);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, prg[0]);
		setprg8(0xE000, ~0);
	} else {
		setprg8(0x8000, prg[0]);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, ~1);
		setprg8(0xE000, ~0);
	}

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	switch (mirr >> 6) {
	case 0:
		setmirror(MI_V);
		break;
	case 2:
		setmirror(MI_H);
		break;
	default:
		setmirror(MI_0);
		break;
	}
}

static DECLFW(M065Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		prg[(A >> 13) & 0x01] = V;
		Sync();
		break;
	case 0x9000:
		switch (A & 0x07) {
		case 0:
			cmd = V;
			Sync();
			break;
		case 1:
			mirr = V;
			Sync();
			break;
		case 3:
			IRQa = V & 0x80;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 4:
			IRQCount = IRQLatch;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 5:
			IRQLatch &= 0x00FF;
			IRQLatch |= V << 8;
			break;
		case 6:
			IRQLatch &= 0xFF00;
			IRQLatch |= V;
			break;
		}
		break;
	case 0xB000:
		chr[A & 0x07] = V;
		Sync();
		break;
	}
}

static void M065Power(void) {
	prg[0] = 0;
	prg[1] = 1;

	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;

	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xBFFF, M065Write);
}

static void M065IRQ(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper065_Init(CartInfo *info) {
	info->Power = M065Power;
	MapIRQHook = M065IRQ;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
