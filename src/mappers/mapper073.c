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
 * Konami VRC-3
 *
 */

#include "mapinc.h"

static uint8 prg;
static uint8 IRQx;	/* autoenable */
static uint8 IRQm;	/* mode */
static uint8 IRQa;
static uint16 IRQLatch, IRQCount;

static SFORMAT StateRegs[] = {
	{ &prg, 1, "PREG" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQx, 1, "IRQX" },
	{ &IRQm, 1, "IRQM" },
	{ &IRQLatch, 2, "IRQL" },
	{ &IRQCount, 2, "IRQC" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, prg);
	setprg16(0xC000, ~0);
	setchr8(0);
}

static DECLFW(M073Write) {
	switch (A & 0xF000) {
	case 0x8000:
		IRQLatch &= 0xFFF0;
		IRQLatch |= (V & 0x0F) << 0;
		break;
	case 0x9000:
		IRQLatch &= 0xFF0F;
		IRQLatch |= (V & 0x0F) << 4;
		break;
	case 0xA000:
		IRQLatch &= 0xF0FF;
		IRQLatch |= (V & 0x0F) << 8;
		break;
	case 0xB000:
		IRQLatch &= 0x0FFF;
		IRQLatch |= (V & 0x0F) << 12;
		break;
	case 0xC000:
		IRQm = V & 0x04;
		IRQx = V & 0x01;
		IRQa = V & 0x02;
		if (IRQa) {
			IRQCount = IRQLatch;
		}
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xD000:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQa = IRQx;
		break;
	case 0xF000:
		prg = V;
		Sync();
		break;
	}
}

static void M073IRQHook(int a) {
	int32 i;

	if (IRQa) {
		for (i = 0; i < a; i++) {
			uint32 IRQCountMask = IRQm ? 0xFF : 0xFFFF;
			if ((IRQCount & IRQCountMask) == IRQCountMask) {
				IRQCount = IRQLatch;
				X6502_IRQBegin(FCEU_IQEXT);
			} else {
				IRQCount++;
			}
		}
	}
}

static void M073Power(void) {
	IRQLatch = IRQm = IRQx = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M073Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

static void M073Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper073_Init(CartInfo *info) {
	info->Power = M073Power;
	info->Close = M073Close;
	MapIRQHook = M073IRQHook;
	AddExState(StateRegs, ~0, 0, NULL);
	GameStateRestore = StateRestore;

	WRAMSIZE = 8192;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
}
