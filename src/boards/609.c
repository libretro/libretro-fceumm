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

static uint8 reg;

static void sync () {
	int bank;
	for (bank = 0x8000; bank <= 0xF000; bank += 0x2000) {
		int val = reg;
		if (reg &0x20) val = val &~0x01 | bank >>13 &1;
		if (reg &0x40) val = val &~0x02 | bank >>13 &2;
		setprg8(bank, val);
	}
	setchr8(0);
	setmirror(reg &0x80? MI_H: MI_V);
}

static DECLFW (writeReg) {
	if (A &0x100) {
		reg = V;
		sync();
	}
}

static void reset () {
	reg = 0;
	sync();
}

static void power () {
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4020, 0x5FFF, writeReg);
	reset();
}

static void stateRestore (int version) {
	sync();
}

void Mapper609_Init (CartInfo *info) {
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(&reg, 1, 0, "REGS");
}
