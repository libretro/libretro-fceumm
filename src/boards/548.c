/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 */

#include "mapinc.h"
#include "cartram.h"
#include "../fds_apu.h"

static uint8 reg;
static uint8 latch;
static uint8 counting;
static uint16 counter;

static SFORMAT stateRegs[] ={
	{ &reg, 1, "REGM" },
	{ &latch, 1, "LATC" },
	{ &counting, 1, "IRQA" },
	{ &counter, 2, "IRQC" },
        { 0 }
};

static void sync () {
	setprg8r(0x10, 0x6000, 0);
	setprg16(0x8000, reg);
	setprg16(0xC000, 3);
	setchr8(0);
}

static DECLFW (writeLatch) {
	latch = A >>2 &0x03 | A >>3 &0x04;
	if (A &4) {
		counting = 0;
		counter = 0;
		X6502_IRQEnd(FCEU_IQEXT);
	} else
		counting = 1;
}

static DECLFW (writeReg) {
	reg = latch ^0x05;
	sync();
}

static void FP_FASTAPASS(1) cpuCycle (int a) {
	while (a--) {
		if (counting) {
			if (counter == 23680)
				X6502_IRQBegin(FCEU_IQEXT);
			else
			if (counter == 24320)
				X6502_IRQEnd(FCEU_IQEXT);
			counter++;
		}
	}
}

static void reset() {
	FDSSoundReset();
	X6502_IRQEnd(FCEU_IQEXT);
	latch = 0x07;
	reg = latch ^0x05;
	counting = 0;
	counter = 0;
	sync();
}

static void power() {
	FDSSoundPower();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x4800, 0x4FFF, writeLatch);
	SetWriteHandler(0x5000, 0x57FF, writeReg);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	reset();
}

static void stateRestore(int version) {
	sync();
}

void Mapper548_Init (CartInfo *info) {
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	MapIRQHook = cpuCycle;
	GameStateRestore = stateRestore;
	AddExState(stateRegs, ~0, 0, 0);
}
