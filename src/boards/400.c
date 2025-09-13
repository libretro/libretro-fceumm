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
#include "asic_latch.h"

static uint8 reg[4];

static void sync () {
	setprg16(0x8000, reg[2] &~0x07 | Latch_data &0x07);
	setprg16(0xC000, reg[3] &~0x07 |             0x07);
	setchr8(Latch_data >>5);
	setmirror(reg[3] &0x20? MI_H: MI_V);
}

static DECLFW (writeReg) {
	if (!(A &0x7FF)) { /* Exclude 8 Bit Xmas 2008's writes to second APU (also at 7001-7015) */
		reg[A >>11 &3] = V;
		sync();
	}
}

static void power () {
	reg[0] = reg[1] = reg[2] = 0;
	reg[3] = 0x80;
	Latch_power();
	SetWriteHandler(0x6000, 0x7FFF, writeReg);
}

static void reset () {
	reg[0] = reg[1] = reg[2] = 0;
	reg[3] = 0x80;
	Latch_clear();
}

void Mapper400_Init (CartInfo *info) {
	Latch_init(info, sync, 0xC000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(reg, 4, 0, "REGS");
}
