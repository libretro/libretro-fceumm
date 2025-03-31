/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

static uint8 reg[2];

static SFORMAT StateRegs[] = {
    { reg, 2, "REGS" },
    { 0 }
};

static void Sync(void) {
	uint8 mask = (8 << (reg[1] >> 4 & 3)) - 1;
	setprg4(0x7000, 0);
	if (reg[1] & 6)
		setprg32(0x8000, (reg[0] << 2) + (latch.data & (mask >> 1)));
	else {
		setprg16(0x8000, (reg[0] << 3) + (latch.data & mask));
		setprg16(0xC000, (reg[0] << 3) + mask);
	}
	setchr8(0);
	if (reg[1] &2)
		setmirror(latch.data &0x10? MI_1: MI_0);
	else
		setmirror(reg[1] &0x01? MI_V: MI_H);
}

static DECLFW(M502WriteReg) {
	if (~reg[1] & 0x80) {
		reg[A & 1] = V;
		Sync();
	}
}

static void M502Power() {
    reg[0] = reg[1] = 0;
	LatchPower();
	SetReadHandler(0x7000, 0x7FFF, CartBR);
    SetWriteHandler(0x6000, 0x6FFF, M502WriteReg);
}

static void M502Reset() {
    reg[0] = reg[1] = 0;
    Sync();
}

void Mapper502_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, 0, 0);
	info->Power = M502Power;
	info->Reset = M502Reset;
    AddExState(StateRegs, ~0, 0, NULL);
}
