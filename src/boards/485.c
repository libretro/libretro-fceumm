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
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
    { &reg, 1, "REG " },
    { 0 }
};

static void Sync(void) {
	if (latch.data &0x20) {
		setprg16(0x8000, latch.data &0x1F | reg <<5);
		setprg16(0xC000, latch.data &0x1F | reg <<5);
	} else
		setprg32(0x8000, latch.data >>1 &0x0F | reg <<4);
	setchr8(0);
	setmirror(latch.data &0x80? (latch.data &0x40? MI_1: MI_H): (latch.data &0x40? MI_V: MI_0));
}

static DECLFW(writeReg) {
	reg =V;
	Sync();
}

static void Power() {
	reg =0;
	LatchPower();
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
}

static void Reset() {
	reg =0;
	latch.data =0;
	Sync();
}

void Mapper485_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Power = Power;
	info->Reset = Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
