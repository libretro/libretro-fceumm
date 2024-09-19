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

/* iNES Mapper 171 - Kaiser KS-7058 */

#include "mapinc.h"

static uint8 chr[2];

static SFORMAT StateRegs[] = {
	{ chr, 2, "CREG" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, 0);
	setchr4(0x0000, chr[0]);
	setchr4(0x1000, chr[1]);
}

static DECLFW(M171Write) {
	chr[A & 0x01] = V;
	Sync();
}

static void M171Power(void) {
	chr[0] = chr[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M171Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper171_Init(CartInfo *info) {
	info->Power = M171Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
