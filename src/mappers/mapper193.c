/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 * MEGA-SOFT WAR IN THE GULF
 *
 */

#include "mapinc.h"

static uint8 reg[8];
static uint8 mirror;

static SFORMAT StateRegs[] = {
	{ &mirror, 1, "MIRR" },
	{ reg, 8, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, reg[3]);
	setprg8(0xA000, ~2);
	setprg8(0xC000, ~1);
	setprg8(0xE000, ~0);
	setchr4(0x0000, reg[0] >> 2);
	setchr2(0x1000, reg[1] >> 1);
	setchr2(0x1800, reg[2] >> 1);
	setmirror((mirror & 0x01) ^ 0x01);
}

static DECLFW(M193Write) {
	reg[A & 7] = V;
	Sync();
}

static void M193Power(void) {
	memset(reg, 0, sizeof(reg));
	Sync();
	SetWriteHandler(0x6000, 0x7FFF, M193Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, CartBW);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper193_Init(CartInfo *info) {
	info->Power = M193Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
