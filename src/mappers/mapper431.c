/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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
	setprg16(0x8000, ((reg[0] >> 2) & ~0x07) | (reg[1] & 0x07));
	setprg16(0xC000, ((reg[0] >> 2) & ~0x07) | 0x07);
	setchr8(0);
	setmirror((reg[0] & 0x01) ^ 0x01);
}

static DECLFW(M431Write) {
	reg[(A >> 14) & 0x01] = V;
	Sync();
}

static void M431Reset(void) {
	reg[1] = reg[0] = 0;
	Sync();
}

static void M431Power(void) {
	reg[1] = reg[0] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M431Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper431_Init(CartInfo *info) {
	info->Power = M431Power;
	info->Reset = M431Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
