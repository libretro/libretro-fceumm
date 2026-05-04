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

static uint8_t reg[4];

static void sync () {
	if (reg[3] &0x01) { /* UNROM mode */
		if (reg[2] &0x20) { /* 128 KiB */
			setprg16(0x8000, reg[2] >>3 &~0x07 | reg[1] &0x07);
			setprg16(0xC000, reg[2] >>3 &~0x07 |         0x07);
		} else { /* 64 KiB */
			setprg16(0x8000, reg[2] >>3 &~0x03 | reg[1] &0x07);
			setprg16(0xC000, reg[2] >>3 &~0x03 | reg[1] &0x04 | 0x03);
		}
	} else { /* NROM */
		setprg16(0x8000, reg[2] >>3 &~0x07 | reg[1] &~1);
		setprg16(0xC000, reg[2] >>3 &~0x07 | reg[1] | 1);
	}
	setchr8(0);
	setmirror(reg[0] &0x01? MI_H: MI_V);
}

static DECLFW (writeReg) {
	reg[A >>14 &1] = V;
	if (A <0xC000) {
		reg[2] = A &0xFF;
		reg[3] = A >>8;
	}
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

void Mapper605_Init (CartInfo *info) {
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 4, 0, "REGS");
}
