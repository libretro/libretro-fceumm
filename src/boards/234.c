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

static uint8_t submapper, initialMirroring;
static uint8_t reg[3];
static uint8_t regIndex, cycleCount, fetchCount;

static void sync () {
	int prg, chr;
	if (reg[0] &0x40) { /* NINA-03 */
		prg = reg[0] &0x0E | reg[2] &0x01;
		chr = reg[0] <<2 &0x38 | reg[2] >>4 &0x07;
	} else { /* CNROM */
		prg = reg[0] &0x0F;
		chr = reg[0] <<2 &0x3C | reg[2] >>4 &0x03;
	}
	if (~reg[0] &0x10) {
		prg |= reg[0] >>1 &0x10;
		chr |= reg[0] <<1 &0x40;
	}
	prg ^= 0x10;
	chr ^= 0x40;
	if (prg &0x10) prg &= (ROM_size -1) >>1;
	if (chr &0x40) chr &= VROM_size -1;
	setprg32(0x8000, prg);
	setchr8(chr);
	setmirror(reg[0] &0x80? MI_H: MI_V);
}

static DECLFR (readRegF_submapper0) {
	uint8_t result = CartBR(A);
	if (A >= 0xFFE8 && A < 0xFFF8) {
		reg[2] = result;
		sync();
	} else
	if (A >= 0xFFC0 && A < 0xFFE0 && !(reg[0] &0x3F)) {
		reg[1] = result;
		sync();
	} else
	if (A >= 0xFF80 && A < 0xFFA0 && !(reg[0] &0x3F)) {
		reg[0] = result;
		sync();
	}
	return result;
}

static DECLFR (readReg4_submapper1) {
	/* Reading from 41xx selects a register for the subsequent read within five CPU cycles.
	   However, because FCEU only executes the MapIRQHook after each instruction, this doesn't work, so it is necessary to count both cycle and fetches
	   to account for switching code that executes from RAM and from ROM. */
	regIndex = A &0xFF;
	cycleCount = 5;
	fetchCount = 4;
	return X.DB;
}

static DECLFR (readReg8_submapper1) {
	uint8_t result = CartBR(A);
	if (cycleCount && fetchCount--) switch(regIndex &0xC0) {
		case 0x00:
			reg[2] = result;
			sync();
			break;
		case 0x40:
			reg[1] = result;
			sync();
			break;
		case 0x80:
			reg[0] = result;
			regIndex = 0xFF;
			sync();
			break;			
	}
	return result;
}

static void FP_FASTAPASS(1) cpuCycle_submapper1 (int a) {
	while (a--) {
		if (cycleCount) cycleCount--;
	}
}

static void reset () {
	reg[0] = reg[1] = reg[2] = cycleCount = fetchCount = 0;
	regIndex = 0xFF;
	if (ROM_size <= 4) { /* Extracts don't set up the mirroring and mode registers by themselves, so fill up these registers based on iNES header data */
		reg[0] = 0x3F;
		if (~initialMirroring &1) reg[0] |= 0x80;
		if (ROM_size == 4 || VROM_size >= 8) reg[0] |= 0x40;
	}
	sync();
}

static void power () {
	if (submapper == 1) {
		SetReadHandler(0x4100, 0x41FF, readReg4_submapper1);
		SetReadHandler(0x8000, 0xFFFF, readReg8_submapper1);
	} else {
		SetReadHandler(0x8000, 0xEFFF, CartBR);
		SetReadHandler(0xF000, 0xFFFF, readRegF_submapper0);
	}
	reset();
}

static void stateRestore (int version) {
	sync();
}

void Mapper234_Init (CartInfo *info) {
	submapper = info->submapper;
	initialMirroring = info->mirror;
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = stateRestore;
	AddExState(reg, 3, 0, "REGS");
	AddExState(&regIndex, 1, 0, "RIDX");
	AddExState(&cycleCount, 1, 0, "CYCC");
	AddExState(&fetchCount, 1, 0, "FETC");
	if (submapper == 1) MapIRQHook = cpuCycle_submapper1;
}
