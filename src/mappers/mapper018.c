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

static uint8 prg[4], chr[8];
static uint8 IRQa, mirr;
static int32 IRQCount, IRQLatch;

static SFORMAT StateRegs[] = {
	{ prg, 4, "PREG" },
	{ chr, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ &IRQLatch, 4, "IRQL" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);

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

	if (mirr & 2)
		setmirror(MI_0);
	else
		setmirror(mirr & 0x01);
}

static DECLFW(M018WriteIRQ) {
	switch (A & 0xF003) {
	case 0xE000:
		IRQLatch &= 0xFFF0;
		IRQLatch |= (V & 0x0F) << 0x00;
		break;
	case 0xE001:
		IRQLatch &= 0xFF0F;
		IRQLatch |= (V & 0x0F) << 0x04;
		break;
	case 0xE002:
		IRQLatch &= 0xF0FF;
		IRQLatch |= (V & 0x0F) << 0x08;
		break;
	case 0xE003:
		IRQLatch &= 0x0FFF;
		IRQLatch |= (V & 0x0F) << 0x0C;
		break;
	case 0xF000:
		IRQCount = IRQLatch;
		break;
	case 0xF001:
		IRQa = V & 0x01;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xF002:
		mirr = V & 0x03;
		Sync();
		break;
	}
}

static DECLFW(M018WritePrg) {
	uint32 i = ((A >> 1) & 1) | ((A - 0x8000) >> 11);

	if (A & 0x01) {
		prg[i] = (prg[i] & 0x0F) | (V << 4);
	} else {
		prg[i] = (prg[i] & 0xF0) | (V & 0x0F);
	}
	Sync();
}

static DECLFW(M018WriteChr) {
	uint32 i = ((A >> 1) & 1) | ((A - 0xA000) >> 11);

	if (A & 0x01) {
		chr[i] = (chr[i] & 0x0F) | (V << 4);
	} else {
		chr[i] = (chr[i] & 0xF0) | (V & 0x0F);
	}
	Sync();
}

static void M018Power(void) {
	IRQa = 0;
	prg[0] = 0;
	prg[1] = 1;
	prg[2] = ~1;
	prg[3] = ~0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x9FFF, M018WritePrg);
	SetWriteHandler(0xA000, 0xDFFF, M018WriteChr);
	SetWriteHandler(0xE000, 0xFFFF, M018WriteIRQ);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M018IRQHook(int a) {
	if (IRQa && IRQCount) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQCount = 0;
			IRQa = 0;
		}
	}
}

static void M018Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper018_Init(CartInfo *info) {
	info->Power = M018Power;
	info->Close = M018Close;
	MapIRQHook = M018IRQHook;
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
