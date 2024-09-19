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
 *
 * NES 2.0 Mapper 535 - UNL-M535
 * FDS Conversion - Nazo no Murasamejō
 *
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg, IRQa;
static int32 IRQCount;

static SFORMAT StateRegs[] =
{
	{ &reg, 1, "REG" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setchr8(0);
	setprg8(0x6000, reg);
	setprg32(0x8000, 0x03);
	setprg8r(0x10, 0xB800, 0);
}

static DECLFW(M535RamWrite) {
	WRAM[(A - 0xB800) & 0x1FFF] = V;
}

static DECLFW(M535Write) {
	reg = V;
	Sync();
}

static DECLFW(M535IRQaWrite) {
	IRQa = V & 0x02;
	IRQCount = 0;
	X6502_IRQEnd(FCEU_IQEXT);
}

static void M535IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount > 7560) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M535Power(void) {
	FDSSound_Power();
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0xB800, 0xD7FF, M535RamWrite);
	SetWriteHandler(0xE000, 0xEFFF, M535IRQaWrite);
	SetWriteHandler(0xF000, 0xFFFF, M535Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M535Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper535_Init(CartInfo *info) {
	info->Power = M535Power;
	info->Close = M535Close;
	MapIRQHook = M535IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
