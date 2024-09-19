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

/* TODO: ADPCM not emulated */

#include "mapinc.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, (reg >> 4) & 0x03);
	setchr8(((reg >> 4) & 0x04) | (reg & 0x03));
}

static DECLFW(M086Write) {
	reg = V;
	Sync();
}

static void M086Power(void) {
	reg = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x6FFF, M086Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper086_Init(CartInfo *info) {
	info->Power = M086Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
