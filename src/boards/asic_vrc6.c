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
#include "asic_vrc6.h"

static void (*VRC6_cbSync)();
static int VRC6_A0, VRC6_A1;
static int (*VRC6_cbGetPRGBank)(uint8);
static int (*VRC6_cbGetCHRBank)(uint8);
static DECLFR ((*VRC6_cbReadWRAM));
static DECLFW ((*VRC6_cbWriteWRAM));
static uint8 VRC6_prg[2];
static uint8 VRC6_chr[8];
static uint8 VRC6_misc;
static uint8 VRC6_latch;
static uint8 VRC6_mode;
static uint8 VRC6_count;
static signed short int VRC6_cycles;

static SFORMAT VRC6_stateRegs[] = {
	{ VRC6_prg, 2, "VC6P" },
	{ VRC6_chr, 8, "VC6C" },
	{ &VRC6_misc, 1, "VC6M" },
	{ &VRC6_latch, 1, "V6LT" },
	{ &VRC6_mode, 1, "V6MO" },
	{ &VRC6_count, 1, "V6CT" },
	{ &VRC6_cycles, 2 | FCEUSTATE_RLSB, "V6CY" },
	{ 0 }
};

void VRC6_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

void VRC6_syncPRG (int AND, int OR) {
	setprg8(0x8000, VRC6_cbGetPRGBank(0) &AND | OR);
	setprg8(0xA000, VRC6_cbGetPRGBank(1) &AND | OR);
	setprg8(0xC000, VRC6_cbGetPRGBank(2) &AND | OR);
	setprg8(0xE000, VRC6_cbGetPRGBank(3) &AND | OR);
}

void VRC6_syncCHR (int AND, int OR) {
	setchr1(0x0000, VRC6_cbGetCHRBank(0) &AND | OR);
	setchr1(0x0400, VRC6_cbGetCHRBank(1) &AND | OR);
	setchr1(0x0800, VRC6_cbGetCHRBank(2) &AND | OR);
	setchr1(0x0C00, VRC6_cbGetCHRBank(3) &AND | OR);
	setchr1(0x1000, VRC6_cbGetCHRBank(4) &AND | OR);
	setchr1(0x1400, VRC6_cbGetCHRBank(5) &AND | OR);
	setchr1(0x1800, VRC6_cbGetCHRBank(6) &AND | OR);
	setchr1(0x1C00, VRC6_cbGetCHRBank(7) &AND | OR);
}

void VRC6_syncMirror () { /* Only emulates features used by known games, meaning mode 0 with CHR A10 substitution */
	setmirror(VRC6_misc &8? (VRC6_misc &4? MI_1: MI_0): VRC6_misc &4? MI_H: MI_V);
}

int VRC6_getPRGBank (uint8 bank) {
	if (bank &2)
		return bank &1? 0xFF: VRC6_prg[1];
	else
		return VRC6_prg[0] <<1 | bank &1;
}

int VRC6_getCHRBank (uint8 bank) { /* Only emulates features used by known games, meaning mode 0 with CHR A10 substitution */
	return VRC6_chr[bank &7];
}

DECLFR(VRC6_readWRAM) {
	if (VRC6_misc &0x80)
		return VRC6_cbReadWRAM? VRC6_cbReadWRAM(A): CartBR(A);
	else
		return A >>8;
}

DECLFW(VRC6_writeWRAM) {
	if (VRC6_misc &0x80) {
		CartBW(A, V);
		if (VRC6_cbWriteWRAM) VRC6_cbWriteWRAM(A, V);
	}
}

DECLFW(VRC6_writeReg) {
	unsigned int index;
	index = (A &VRC6_A0? 1: 0) | (A &VRC6_A1? 2: 0);
	switch (A >>12 &7) {
		case 0: case 4:
			VRC6_prg[A >>14 &1] = V;
			VRC6_cbSync();
			break;
		case 1: case 2: /* Sound */
			break;
		case 3:
			if (index == 3) {
				VRC6_misc = V;
				VRC6_cbSync();
			}
			break;
		case 5: case 6:
			VRC6_chr[A >>11 &4 | index] = V;
			VRC6_cbSync();
			break;
		case 7:
			switch (index) {
				case 0:
					VRC6_latch = V;
					break;
				case 1:
					VRC6_mode = V;
					if (VRC6_mode &0x02) {
						VRC6_count = VRC6_latch;
						VRC6_cycles = 341;
					}
					X6502_IRQEnd(FCEU_IQEXT);
					break;
				case 2:
					VRC6_mode = VRC6_mode &~0x02 | VRC6_mode <<1 &0x02;
					X6502_IRQEnd(FCEU_IQEXT);
					break;
			}
			break;
	}
}

void FP_FASTAPASS(1) VRC6_cpuCycle (int a) {
	while (a--) {
		if (VRC6_mode &0x02 && (VRC6_mode &0x04 || (VRC6_cycles -= 3) <= 0)) {
			if (~VRC6_mode &0x04) VRC6_cycles += 341;
			if (!++VRC6_count) {
				VRC6_count = VRC6_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void VRC6_clear () {
	VRC6_prg[0] = 0; VRC6_prg[1] = 0xFE;
	VRC6_chr[0] = 0; VRC6_chr[1] = 1; VRC6_chr[2] = 2; VRC6_chr[3] = 3; VRC6_chr[4] = 4; VRC6_chr[5] = 5; VRC6_chr[6] = 6; VRC6_chr[7] = 7;
	VRC6_misc = VRC6_latch = VRC6_mode = VRC6_count = VRC6_cycles = 0;
	VRC6_cbSync();
}

static void VRC6_setHandlers () {
	SetReadHandler(0x6000, 0x7FFF,  VRC6_readWRAM);
	SetWriteHandler(0x6000, 0x7FFF, VRC6_writeWRAM);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC6_writeReg);
	MapIRQHook =VRC6_cpuCycle;
}

static void VRC6_configure (void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC6_A0 = A0;
	VRC6_A1 = A1;
	VRC6_cbSync = sync;
	VRC6_cbGetPRGBank = prg? prg: VRC6_getPRGBank;
	VRC6_cbGetCHRBank = chr? chr: VRC6_getCHRBank;
	VRC6_cbReadWRAM = read;
	VRC6_cbWriteWRAM = write;
}

void VRC6_activate (uint8 clear, void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC6_configure(sync, A0, A1, prg, chr, read, write);
	VRC6_setHandlers();
	if (clear)
		VRC6_clear();
	else
		VRC6_cbSync();
}

void VRC6_addExState () {
	AddExState(VRC6_stateRegs, ~0, 0, 0);
}

void VRC6_restore (int version) {
	VRC6_cbSync();
}

void VRC6_power () {
	VRC6_setHandlers();
	VRC6_clear();
}

void VRC6_init (CartInfo *info, void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC6_addExState();
	VRC6_configure(sync, A0, A1, prg, chr, read, write);
	info->Power = VRC6_power;
	info->Reset = VRC6_cbSync;
	GameStateRestore = VRC6_restore;
}
