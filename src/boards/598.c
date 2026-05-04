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

static uint8_t reg[2];

static void sync () {
	setprg16(0x8000, reg[0] &0x18 | reg[1] &0x07);
	setprg16(0xC000, reg[0] &0x18 |         0x07);
	setchr8(0);
	switch(reg[0] &0x03) {
		case 0x00: setmirror(MI_0); break;
		case 0x01: setmirror(MI_V); break;
		case 0x02: setmirror(MI_H); break;
		case 0x03: setmirror(MI_1); break;
	}
}

static DECLFW (writeReg) {
	reg[A >>14 &1] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = 0;
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
	AddExState(reg, 2, 0, "REGS");
}
