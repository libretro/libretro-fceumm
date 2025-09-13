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
#include "asic_fme7.h"

static void (*FME7_cbSync)();
static uint8 FME7_index;
static uint8 FME7_reg[16];

static SFORMAT FME7_stateRegs[] = {
	{ &FME7_index,  1, "FM7I" },
	{  FME7_reg,   16, "FM7R" },
	{ 0 }
};

void FME7_syncWRAM (int OR) {
	if (PRGsize[0x10] && FME7_reg[8] &0x40) setprg8r(0x10, 0x6000, FME7_reg[8] | OR);
}

void FME7_syncPRG (int AND, int OR) {
	if (~FME7_reg[8] &0x40) setprg8(0x6000, FME7_reg[8] &AND | OR);
	setprg8(0x8000, FME7_reg[ 9] &AND | OR);
	setprg8(0xA000, FME7_reg[10] &AND | OR);
	setprg8(0xC000, FME7_reg[11] &AND | OR);
	setprg8(0xE000,        0xFF  &AND | OR);
}

void FME7_syncCHR (int AND, int OR) {
	setchr1(0x0000, FME7_reg[0] &AND | OR);
	setchr1(0x0400, FME7_reg[1] &AND | OR);
	setchr1(0x0800, FME7_reg[2] &AND | OR);
	setchr1(0x0C00, FME7_reg[3] &AND | OR);
	setchr1(0x1000, FME7_reg[4] &AND | OR);
	setchr1(0x1400, FME7_reg[5] &AND | OR);
	setchr1(0x1800, FME7_reg[6] &AND | OR);
	setchr1(0x1C00, FME7_reg[7] &AND | OR);
}

void FME7_syncMirror () {
	setmirror(FME7_reg[12] &2? (FME7_reg[12] &1? MI_1: MI_0): FME7_reg[12] &1? MI_H: MI_V);
}

DECLFR (FME7_readWRAM) {
	return ~FME7_reg[8] &0x40 || FME7_reg[8] &0x80? CartBR(A): A >>8;
}

DECLFW (FME7_writeWRAM) {
	if (FME7_reg[8] &0x80) CartBW(A, V);
}

DECLFW (FME7_writeReg) {
	if (A &0x2000) {
		FME7_reg[FME7_index &0xF] = V;
		if ((FME7_index &0xF) == 13) X6502_IRQEnd(FCEU_IQEXT);
		FME7_cbSync();
	} else
		FME7_index = V;
}

void FP_FASTAPASS(1) FME7_cpuCycle (int a) {
	while (a--) {
		int16_t counter = FME7_reg[14] | FME7_reg[15] <<8;
		if (FME7_reg[13] &0x80 && !--counter && FME7_reg[13] &0x01) X6502_IRQBegin(FCEU_IQEXT);
		FME7_reg[14] = counter &0xFF;
		FME7_reg[15] = counter >>8;
	}
}

void FME7_clear () {
	int i;
	for (i = 0; i < 16; i++) FME7_reg[i] = 0;
	FME7_cbSync();
}

static void FME7_setHandlers () {
	SetReadHandler(0x6000, 0x7FFF,  FME7_readWRAM);
	SetWriteHandler(0x6000, 0x7FFF, FME7_writeWRAM);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xBFFF, FME7_writeReg);
	MapIRQHook = FME7_cpuCycle;
}

static void FME7_configure (void (*sync)()) {
	FME7_cbSync = sync;
}

void FME7_activate (uint8 clear, void (*sync)()) {
	FME7_configure(sync);
	FME7_setHandlers();
	if (clear)
		FME7_clear();
	else
		FME7_cbSync();
}

void FME7_addExState () {
	AddExState(FME7_stateRegs, ~0, 0, 0);
}

void FME7_restore (int version) {
	FME7_cbSync();
}

void FME7_power () {
	FME7_setHandlers();
	FME7_clear();
}

void FME7_init (CartInfo *info, void (*sync)()) {
	FME7_addExState();
	FME7_configure(sync);
	info->Power = FME7_power;
	info->Reset = FME7_cbSync;
	GameStateRestore = FME7_restore;
}
