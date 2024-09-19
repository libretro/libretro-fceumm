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

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, reg[0]);
	setchr8(reg[1]);
}

static DECLFW(M244Write) {
	if (V & 8) {
		static const uint8 chr_perm[8][8] = {
			{ 0, 1, 2, 3, 4, 5, 6, 7, },
			{ 0, 2, 1, 3, 4, 6, 5, 7, },
			{ 0, 1, 4, 5, 2, 3, 6, 7, },
			{ 0, 4, 1, 5, 2, 6, 3, 7, },
			{ 0, 4, 2, 6, 1, 5, 3, 7, },
			{ 0, 2, 4, 6, 1, 3, 5, 7, },
			{ 7, 6, 5, 4, 3, 2, 1, 0, },
			{ 7, 6, 5, 4, 3, 2, 1, 0, },
		};
		reg[1] = chr_perm[(V >> 4) & 0x07][V & 0x07];
		Sync();
	} else {
		static const uint8 prg_perm[4][4] = {
			{ 0, 1, 2, 3, },
			{ 3, 2, 1, 0, },
			{ 0, 2, 1, 3, },
			{ 3, 1, 2, 0, },
		};
		reg[0] = prg_perm[(V >> 4) & 0x03][V & 0x03];
		Sync();
	}
}

static void M244Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetWriteHandler(0x8000, 0xFFFF, M244Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper244_Init(CartInfo *info) {
	info->Power = M244Power;
	AddExState(StateRegs, ~0, 0, NULL);
	GameStateRestore = StateRestore;
}
