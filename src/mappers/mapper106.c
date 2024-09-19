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
 */

#include "mapinc.h"

static uint8 prg[4], chr[8];
static uint8 mirr, IRQa;
static uint32 IRQCount;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREGS" },
	{ chr, 8, "CREGS" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg8(0x8000, (prg[0] & 0x0F) | 0x10);
	setprg8(0xA000, (prg[1] & 0x1F));
	setprg8(0xC000, (prg[2] & 0x1F));
	setprg8(0xE000, (prg[3] & 0x0F) | 0x10);

	setchr1(0x0000, chr[0] & ~1);
	setchr1(0x0400, chr[1] |  1);
	setchr1(0x0800, chr[2] & ~1);
	setchr1(0x0c00, chr[3] |  1);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);
	
	setmirror((mirr & 0x01) ^ 0x01);
}

static DECLFW(M106Write) {
	switch (A & 0x0F) {
	case 0x00: case 0x01: case 0x02: case 0x03:
	case 0x04: case 0x05: case 0x06: case 0x07:
		chr[A & 0x07] = V;
		Sync();
		break;
	case 0x08: case 0x09: case 0x0A: case 0x0B:
		prg[A & 0x03] = V;
		Sync();
		break;
	case 0x0C:
		mirr = V;
		Sync();
		break;
	case 0x0D:
		IRQa = 0;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x0E:
		IRQCount = (IRQCount & 0xFF00) | V;
		break;
	case 0x0F:
		IRQCount = (IRQCount & 0x00FF) | (V << 8);
		IRQa = 1;
		break;
	}
}

static void M106Power(void) {
	Sync();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M106Write);
}

static void M106Reset(void) { }

static void M106Close(void) {
}

static void M106CpuHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount > 0x10000) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper106_Init(CartInfo *info) {
	info->Reset = M106Reset;
	info->Power = M106Power;
	info->Close = M106Close;
	MapIRQHook = M106CpuHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
