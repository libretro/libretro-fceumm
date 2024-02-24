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

static uint8 prg, chr[4], mirr;
static uint8 IRQa, toggle = 0;
static int16 IRQCount, IRQLatch;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
	{ &toggle, 1, "TOGL" },
	{ chr, 4, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQLatch, 2, "IRQL" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, prg);
	setprg16(0xC000, ~0);

	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);

	switch (mirr & 0x03) {
	case 0:
		setmirror(MI_V);
		break;
	case 1:
		setmirror(MI_H);
		break;
	case 2:
		setmirror(MI_0);
		break;
	case 3:
		setmirror(MI_1);
		break;
	}
}

static DECLFW(M067Write) {
	switch (A & 0xF800) {
	case 0x8800:
	case 0x9800:
	case 0xA800:
	case 0xB800:
		chr[(A >> 12) & 0x03] = V;
		Sync();
		break;
	case 0xC000:
	case 0xC800:
		IRQCount &= 0xFF << (toggle << 3);
		IRQCount |= V << ((toggle ^ 1) << 3);
		toggle ^= 1;
		break;
	case 0xD800:
		toggle = 0;
		IRQa = V & 0x10;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xE800:
		mirr = V;
		Sync();
		break;
	case 0xF800:
		prg = V;
		Sync();
		break;
	}
}

static void M067Power(void) {
	prg = 0;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	IRQa = IRQCount = IRQLatch = toggle = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M067Write);
}

static void M067IRQ(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount < 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper067_Init(CartInfo *info) {
	info->Power = M067Power;
	MapIRQHook = M067IRQ;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
