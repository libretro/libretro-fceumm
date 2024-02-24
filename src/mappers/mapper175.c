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

static uint8 reg[2], mirr;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, reg[0]);
	setprg16(0xC000, reg[0]);
	setchr8(reg[0]);
	setmirror(((mirr >> 2) & 0x01) ^ 0x01);
}

static DECLFR(M175Read) {
	switch (A & 0xF000) {
	case 0xF000:
		if (reg[0] != reg[1]) {
			reg[0] = reg[1];
			Sync();
		}
		break;
	}
	return CartBR(A);
}

static DECLFW(M175Write) {
	switch (A & 0xF000) {
	case 0x8000:
		mirr = V;
		Sync();
		break;
	case 0xA000:
		reg[1] = V;
		break;
	}
}

static void M175Power(void) {
	reg[0] = reg[1] = mirr = 0;
	SetReadHandler(0x8000, 0xFFFF, M175Read);
	SetWriteHandler(0x8000, 0xFFFF, M175Write);
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper175_Init(CartInfo *info) {
	info->Power = M175Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
