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
 */

#include "mapinc.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REG" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, 0);
	setchr4(0x0000, reg & 0x0F);
	setchr4(0x1000, (reg >> 4) & 0x0F);
}

static DECLFW(M184Write) {
	reg = V;
	Sync();
}

static void M184Power(void) {
	reg = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, M184Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper184_Init(CartInfo *info) {
	info->Power = M184Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
