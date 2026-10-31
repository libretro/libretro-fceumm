/* FCE Ultra - NES/Famicom Emulator: N6-H/D+RAM
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

static uint8_t reg[2];
static uint8_t scratch[4];

static void sync () {
	if (reg[1] &0x08) {
		if (reg[1] &0x04) {
			setprg16(0x8000, reg[0]);
			setprg16(0xC000, reg[0]);
		} else
			setprg32(0x8000, reg[0] >>1);
	} else {
			setprg16(0x8000, reg[0]);
			setprg16(0xC000, reg[0] |7);
	}
	setchr8(0);
	setmirror(reg[1] &0x01? MI_H: MI_V);
}

static DECLFR (readScratch) {
	return scratch[A &3];
}
	
static DECLFW (writeScratch) {
	scratch[A &3] = V;
}

static DECLFW (writeReg) {
	if (A &0x4000)
		reg[0] = reg[0] &~0x07 | V &0x07;
	else
		reg[A >>8 &1] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = 0;
	sync();
}

static void power () {
	reg[0] = reg[1] = 0;
	scratch[0] = scratch[1] = scratch[2] = scratch[3] = 0;
	SetReadHandler(0x5000, 0x5FFF, readScratch);
	SetWriteHandler(0x5000, 0x5FFF, writeScratch);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, writeReg);
	reset();
}

static void stateRestore (int version) {
	sync();
}

void Mapper640_Init (CartInfo *info) {
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 2, 0, "REGS");
	AddExState(scratch, 4, 0, "SCRA");
}
