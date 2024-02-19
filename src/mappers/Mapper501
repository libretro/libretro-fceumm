/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
    { reg, 2, "REGS" },
    { 0 }
};

static void Sync(void) {
	setprg4(0x7000, 0);
	setprg32(0x8000, (reg[0] << 2) + (latch.data & 0x07));
	setchr8(0);
	setmirror(MI_0 + ((latch.data >> 4) & 0x01));
}

static DECLFW(M501WriteReg) {
	if (!(reg[1] & 0x80)) {
		reg[A & 0x01] = V;
		Sync();
	}
}

static void M501Power(void) {
    reg[0] = reg[1] = 0;
	Latch_Power();
	SetReadHandler(0x7000, 0x7FFF, CartBR);
    SetWriteHandler(0x6000, 0x6FFF, M501WriteReg);
}

void Mapper501_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M501Power;
    AddExState(StateRegs, ~0, 0, NULL);
}
