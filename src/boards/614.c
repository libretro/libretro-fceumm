/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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

static uint8 reg[3];

static void sync () {
	int prg = reg[0] &0x30 | reg[1] <<3 &0x08;
	if (reg[0] &0x08) {
		if (reg[0] &0x40)
			setprg32(0x8000, prg >>1 &0x1C | reg[0] >>1 &0x03);
		else {
			setprg16(0x8000, prg &0x38 | reg[2] &0x07);
			setprg16(0xC000, prg &0x38 | reg[2] &0x07);
		}
	} else {
		setprg16(0x8000, prg &0x38 | reg[2] &0x07);
		setprg16(0xC000, prg &0x38 |         0x07);
	}
	setchr8(0);
	setmirror(reg[0] &0x80? MI_H: MI_V);
}

static DECLFW (writeReg) {
	reg[A >>13 &2] = V;
	if (~A &0x4000) reg[1] = A &0xFF;
	sync();
}

static void reset () {
	reg[0] = reg[1] = reg[2] = 0;
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

void Mapper614_Init (CartInfo *info) {
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 3, 0, "REGS");
}
