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
	int prg = reg[3] <<3 | reg[1] &0x07;
	if (reg[3] &0x08)
		setprg32(0x8000, prg >>1);
	else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg |7);
	}
	setchr4(0x0000, reg[3] <<5 | reg[1] >>3 &0x1F);
	setchr4(0x1000, reg[3] <<5 | reg[2] >>3 &0x1F);
	setmirror(reg[3] &0x04? MI_H: MI_V);
}

static DECLFW (writeReg) {
	reg[A >>13 &3] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	sync();
}

static void power () {
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
	reset();
}

static void stateRestore (int version) {
	sync();
}

void Mapper598_Init (CartInfo *info) {
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 4, 0, "REGS");
}
