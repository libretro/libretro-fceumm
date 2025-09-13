/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

static uint8 reg[4];

static void sync () {
	setprg16(0x8000, reg[1] <<3 | reg[0] &0x07);
	setprg16(0xC000, reg[3] <<3 | reg[2] &0x07);
	setchr8(0);
}

static DECLFW (writeReg) {
	reg[A >>13 &3] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0xFF;
	sync();
}

static void power () {
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
	reset();
}

static void stateRestore (int version) {
	sync();
}

void Mapper582_Init (CartInfo *info) {
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = stateRestore;
	AddExState(reg, 4, 0, "REGS");
}
