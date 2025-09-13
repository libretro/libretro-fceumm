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

static uint8 reg[2];

static void sync () {
	setprg32(0x8000, 0);
	setchr4(0x0000, reg[0]);
	setchr4(0x1000, reg[1]);
}

static DECLFW (writeReg) {
	reg[A &1] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = 0;
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

void Mapper122_Init (CartInfo *info) {
	info->Reset = reset;
	info->Power = power;
	GameStateRestore = stateRestore;
	AddExState(reg, 2, 0, "REGS");
}
