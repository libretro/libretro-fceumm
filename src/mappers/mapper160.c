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

/*
iNES Mapper 160 appeared to describe variant behavior of iNES Mapper 090, but
Mapper 90 fully encompasses the behavior Mapper 160 was intended to emulate.
*/

#if 0
static uint8 reg = 0;

static void Sync(void) {
	setprg32(0x8000, 0);
	setchr8(reg & 1);
}

static DECLFW(SAWrite) {
	if (A & 0x100) {
		reg = V;
		Sync();
	}
}

static void M160Power(void) {
	reg = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, SAWrite);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper160_Init(CartInfo *info) {
	GameStateRestore = StateRestore;
	info->Power = M160Power;
	AddExState(&reg, 1, 0, "LATC");
}
#endif