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
 * FDS Conversion
 *
 */

#include "mapinc.h"

static uint8 reg;
static uint32 IRQCount, IRQa;

static SFORMAT StateRegs[] = {
	{ &IRQCount, 4, "IRQC" },
	{ &IRQa, 4, "IRQA" },
	{ &reg, 1, "REG" },
	{ 0 }
};

static void Sync(void) {
	uint8 prg = ((reg & 0x01) << 2) | ((reg & 0x02) >> 1) | ((reg & 0x04) >> 1) | (reg & 0x08);

	setprg8(0x6000, 15);
	setprg8(0x8000, 8);
	setprg8(0xA000, 9);
	setprg8(0xC000, prg);
	setprg8(0xE000, 11);
	setchr8(0);
}

static DECLFW(M050Write) {
	switch (A & 0xD160) {
	case 0x4120:
		IRQa = V & 0x01;
		if (!IRQa) {
			IRQCount = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	case 0x4020:
		reg = V;
		Sync();
		break;
	}
}

static void M050Power(void) {
	reg = 0;
	IRQa = IRQCount = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x4FFF, M050Write);
}

static void M050IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount & 0x1000) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper050_Init(CartInfo *info) {
	info->Power = M050Power;
	MapIRQHook = M050IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
