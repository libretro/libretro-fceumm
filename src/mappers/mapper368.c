/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 357 is used for a 4-in-1 multicart (cartridge ID 4602) from Bit Corp.
 * The first game is Bit Corp's hack of the YUNG-08 conversion of Super Mario Brothers 2 (J) named Mr. Mary 2,
 * the other three games are UNROM games.
 */


#include "mapinc.h"

static uint8 reg[2];
static uint8 IRQa;
static uint16 IRQCount;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &IRQCount, 2, "IRQC" },
	{ &IRQa, 1, "IRQA" },
	{ 0 }
};

static void Sync(void) {
	/*
	 The actual bank number is:
	 Value  Bank#
	 ------------
	 0      4
	 1      3
	 2      5
	 3      3
	 4      6
	 5      3
	 6      7
	 7      3 */
	uint8 prg = (reg[0] & 0x01) ? 0x03 : (0x04 | ((reg[0] >> 1) & 0x03));

	setprg8(0x6000, 0x02);
	setprg8(0x8000, 0x01);
	setprg8(0xA000, 0x00);
	setprg8(0xC000, prg);
	setprg8(0xE000, 0x08);
	setchr8(0);
}

static DECLFR(M368Read) {
	if ((A & 0xF1FF) == 0x4122) {
		return (0x8A | (reg[1] & 0x35));
	}
	return CartBR(A);
}

static DECLFW(M368Write) {
	switch (A & 0xF1FF) {
	case 0x4022:
		reg[0] = V;
		Sync();
		break;
	case 0x4122:
		reg[1] = V;
		IRQa = V & 0x01;
		if (!IRQa) {
			IRQCount = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;
	}
}

static void M368Power(void) {
	reg[0] = reg[1] = 0;
	IRQa = IRQCount = 0;
	Sync();
	SetReadHandler(0x4020, 0x4FFF, M368Read);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x4FFF, M368Write);
}

static void M368Reset(void) {
	reg[0] = reg[1] = 0;
	IRQa = IRQCount = 0;
	Sync();
}

static void M368IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount >= 4096) {
			IRQCount -= 4096;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper368_Init(CartInfo *info) {
	info->Reset = M368Reset;
	info->Power = M368Power;
	MapIRQHook = M368IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
