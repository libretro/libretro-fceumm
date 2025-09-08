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
#include "asic_VRC7.h"
#include "wram.h"

static void (*VRC7_cbSync)();
static int VRC7_A0;
static uint8 VRC7_prg[3];
static uint8 VRC7_chr[8];
static uint8 VRC7_misc;
static uint8 VRC7_latch;
static uint8 VRC7_mode;
static uint8 VRC7_count;
static signed short int VRC7_cycles;

static SFORMAT VRC7_stateRegs[] = {
	{ VRC7_prg, 3, "VC7P" },
	{ VRC7_chr, 8, "VC7C" },
	{ &VRC7_misc, 1, "VC7M" },
	{ &VRC7_latch, 1, "V7LT" },
	{ &VRC7_mode, 1, "V7MO" },
	{ &VRC7_count, 1, "V7CT" },
	{ &VRC7_cycles, 2 | FCEUSTATE_RLSB, "V7CY" },
	{ 0 }
};

void VRC7_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

void VRC7_syncPRG (int AND, int OR) {
	setprg8(0x8000, VRC7_prg[0] &AND | OR);
	setprg8(0xA000, VRC7_prg[1] &AND | OR);
	setprg8(0xC000, VRC7_prg[2] &AND | OR);
	setprg8(0xE000,        0xFF &AND | OR);
}

void VRC7_syncCHR (int AND, int OR) {
	setchr1(0x0000, VRC7_chr[0] &AND | OR);
	setchr1(0x0400, VRC7_chr[1] &AND | OR);
	setchr1(0x0800, VRC7_chr[2] &AND | OR);
	setchr1(0x0C00, VRC7_chr[3] &AND | OR);
	setchr1(0x1000, VRC7_chr[4] &AND | OR);
	setchr1(0x1400, VRC7_chr[5] &AND | OR);
	setchr1(0x1800, VRC7_chr[6] &AND | OR);
	setchr1(0x1C00, VRC7_chr[7] &AND | OR);
}

void VRC7_syncMirror () {
	setmirror(VRC7_misc &2? (VRC7_misc &1? MI_1: MI_0): VRC7_misc &1? MI_H: MI_V);
}

DECLFR(VRC7_readWRAM) {
	return VRC7_misc &0x80? CartBR(A): A >>8;
}

DECLFW(VRC7_writeWRAM) {
	if (VRC7_misc &0x80) CartBW(A, V);
}

DECLFW(VRC7_writeReg) {
	unsigned int index;
	index = A &VRC7_A0? 1: 0;
	switch (A >>12 &7) {
		case 0:
			VRC7_prg[index &1] = V;
			VRC7_cbSync();
			break;
		case 1:
			if (!index) {
				VRC7_prg[2] = V;
				VRC7_cbSync();
			}
			break;
		case 2: case 3: case 4: case 5:
			VRC7_chr[(A -0xA000) >>11 &6 | index] = V;
			VRC7_cbSync();
			break;
		case 6:	if (!index) {
				VRC7_misc = V;
				VRC7_cbSync();
			} else
				VRC7_latch = V;
			break;
		case 7:
			if (!index) {
				VRC7_mode = V;
				if (VRC7_mode &0x02) {
					VRC7_count = VRC7_latch;
					VRC7_cycles = 341;
				}
				X6502_IRQEnd(FCEU_IQEXT);
			} else {
				VRC7_mode = VRC7_mode &~0x02 | VRC7_mode <<1 &0x02;
				X6502_IRQEnd(FCEU_IQEXT);
			}
			break;
	}
}

void FP_FASTAPASS(1) VRC7_cpuCycle (int a) {
	while (a--) {
		if (VRC7_mode &0x02 && (VRC7_mode &0x04 || (VRC7_cycles -= 3) <= 0)) {
			if (~VRC7_mode &0x04) VRC7_cycles += 341;
			if (!++VRC7_count) {
				VRC7_count = VRC7_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void VRC7_clear () {
	VRC7_prg[0] = 0; VRC7_prg[1] = 1; VRC7_prg[2] = 0xFE;
	VRC7_chr[0] = 0; VRC7_chr[1] = 1; VRC7_chr[2] = 2; VRC7_chr[3] = 3; VRC7_chr[4] = 4; VRC7_chr[5] = 5; VRC7_chr[6] = 6; VRC7_chr[7] = 7;
	VRC7_misc = VRC7_latch = VRC7_mode = VRC7_count = VRC7_cycles = 0;
	VRC7_cbSync();
}

static void VRC7_setHandlers () {
	SetReadHandler(0x6000, 0x7FFF,  VRC7_readWRAM);
	SetWriteHandler(0x6000, 0x7FFF, VRC7_writeWRAM);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC7_writeReg);
	MapIRQHook =VRC7_cpuCycle;
}

static void VRC7_configure (void (*sync)(), int A0) {
	VRC7_A0 = A0;
	VRC7_cbSync = sync;
}

void VRC7_activate (uint8 clear, void (*sync)(), int A0) {
	VRC7_configure(sync, A0);
	VRC7_setHandlers();
	if (clear)
		VRC7_clear();
	else
		VRC7_cbSync();
}

void VRC7_addExState () {
	AddExState(VRC7_stateRegs, ~0, 0, 0);
}

void VRC7_restore (int version) {
	VRC7_cbSync();
}

void VRC7_power () {
	VRC7_setHandlers();
	VRC7_clear();
}

void VRC7_init (CartInfo *info, void (*sync)(), int A0) {
	VRC7_addExState();
	VRC7_configure(sync, A0);
	info->Power = VRC7_power;
	info->Reset = VRC7_cbSync;
	GameStateRestore = VRC7_restore;
}
