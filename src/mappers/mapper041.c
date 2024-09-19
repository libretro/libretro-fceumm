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
	setprg32(0x8000, reg[0] & 0x07);
	setchr8(((reg[0] >> 1) & 0x0C) | (reg[1] & 0x03));
	setmirror(((reg[0] >> 5) & 0x01) ^ 0x01);
}

static DECLFW(M041Write0) {
	reg[0] = A;
	Sync();
}

static DECLFW(M041Write1) {
	if (reg[0] & 0x04) {
		/* bus conflict */
		reg[1] = (V & CartBR(A));
		Sync();
	}
}

static void M041Reset(void) {
	reg[0] = reg[1] = 0;
	Sync();
}

static void M041Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x67FF, M041Write0);
	SetWriteHandler(0x8000, 0xFFFF, M041Write1);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper041_Init(CartInfo *info) {
	info->Power = M041Power;
	info->Reset = M041Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
