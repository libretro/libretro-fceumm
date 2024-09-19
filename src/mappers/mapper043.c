/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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

static uint8 reg, swap;
static uint32 IRQCount, IRQa;

static SFORMAT StateRegs[] = {
	{ &IRQCount, 4, "IRQC" },
	{ &IRQa, 4, "IRQA" },
	{ &reg, 1, "REGS" },
	{ &swap, 1, "SWAP" },
	{ 0 }
};

static void Sync(void) {
	int transo[8] = { 4, 3, 4, 4, 4, 7, 5, 6 };

	setprg4(0x5000, 8 << 1); /* Only YS-612 advanced version */
	setprg8(0x6000, swap ? 0 : 2);
	setprg8(0x8000, 1);
	setprg8(0xA000, 0);
	setprg8(0xC000, transo[reg & 0x07]);
	setprg8(0xE000, swap ? 8 : 9); /* hard dump for mr.Mary is 128K,
	                                * bank 9 is the last 2K ok bank 8 repeated 4 times, then till the end of 128K
	                                * instead used bank A, containing some CHR data, ines rom have unused banks removed,
	                                * and bank A moved to the bank 9 place for compatibility with other crappy dumps
	                                */
	setchr8(0);
}

static DECLFW(M043Write) {
	switch (A & 0xF1FF) {
	case 0x4022:
		reg = V;
		Sync();
		break;
	case 0x4120:
		swap = V & 0x01;
		Sync();
		break;
	case 0x8122: /* hacked version */
	case 0x4122: /* original version */
		IRQa = V & 0x01;
		IRQCount = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	}
}

static void M043Power(void) {
	reg = swap = 0;
	Sync();
	SetReadHandler(0x5000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x8FFF, M043Write);
}

static void M043IRQHook(int a) {
	IRQCount += a;
	if (IRQa && (IRQCount >= 4096)) {
		IRQa = 0;
		X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper043_Init(CartInfo *info) {
	info->Power = M043Power;
	MapIRQHook = M043IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
