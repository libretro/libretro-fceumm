/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
 * FDS Conversion - Almana no Kiseki - (Co Tung Co.)'s CTC-15 circuit board
 *
 */

#include "mapinc.h"
#include "fdssound.h"

static uint8 reg, latch, IRQa;
static int32 IRQCount;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REG" },
	{ &latch, 1, "LATC" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setchr8(0);
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, reg);
	setprg16(0xC000, 0x03);
}

static DECLFW(M548Write4800) {
	latch =  ((A >> 3) & 0x04) | ((A >> 2) & 0x03);
	IRQa = (A & 0x04) != 0x04;
	if (!IRQa) {
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
	}
}

static DECLFW(M548Write5000) {
	reg = latch ^ 0x05;
	Sync();
}
	

static void M548IRQ(int a) {
	int count = a;
	if (IRQa) {
		while (count > 0) {
			if (IRQCount == 23680) {
				X6502_IRQBegin(FCEU_IQEXT);
			} else if (IRQCount == 24320) {
				X6502_IRQEnd(FCEU_IQEXT);
			}
			count--;
			IRQCount++;
		}
	}
}

static void M548Power(void) {
	latch = 7;
	reg = latch ^ 0x05;
	IRQa = 0;
	IRQCount = 0;
	FDSSound_Power();
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4800, 0x4FFF, M548Write4800);
	SetWriteHandler(0x5000, 0x57FF, M548Write5000);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M548Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper548_Init(CartInfo *info) {
	info->Power = M548Power;
	info->Close = M548Close;
	MapIRQHook = M548IRQ;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
