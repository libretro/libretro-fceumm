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
#include "asic_vrc2and4.h"
#include "wram.h"

static void (*VRC24_cbSync)();
static int VRC24_A0, VRC24_A1;
static int (*VRC24_cbGetPRGBank)(uint8);
static int (*VRC24_cbGetCHRBank)(uint8);
static DECLFR((*VRC24_cbReadWRAM));
static DECLFW((*VRC24_cbWriteWRAM));
static DECLFW((*VRC24_cbExternalSelect));
static uint8 VRC24_isVRC4;       /* VRC2 or VRC4? VRC2 has no single-screen mirroring, no PRG A14 swap and no IRQ counter */
static uint8 VRC24_useRepeatBit; /* Some VRC4 clones ignore the "repeat" bit in the IRQ Mode register */
static uint8 VRC24_prg[2];
static uint16 VRC24_chr[8];
static uint8 VRC24_mirroring;
static uint8 VRC24_misc;
uint8 VRC2_pins;                  /* EEPROM interface */
static uint8 VRC4_latch;
static uint8 VRC4_mode;
static uint8 VRC4_count;
static signed short int VRC4_cycles;

static SFORMAT VRC24_stateRegs[] = {
	{ VRC24_prg, 2, "VC2P" },
	{ &VRC24_chr[0], 2 | FCEUSTATE_RLSB, "V2C0" },
	{ &VRC24_chr[1], 2 | FCEUSTATE_RLSB, "V2C1" },
	{ &VRC24_chr[2], 2 | FCEUSTATE_RLSB, "V2C2" },
	{ &VRC24_chr[3], 2 | FCEUSTATE_RLSB, "V2C3" },
	{ &VRC24_chr[4], 2 | FCEUSTATE_RLSB, "V2C4" },
	{ &VRC24_chr[5], 2 | FCEUSTATE_RLSB, "V2C5" },
	{ &VRC24_chr[6], 2 | FCEUSTATE_RLSB, "V2C6" },
	{ &VRC24_chr[7], 2 | FCEUSTATE_RLSB, "V2C7" },
	{ &VRC24_mirroring, 1, "VC2N" },
	{ &VRC24_misc, 1, "VC4M" },
	{ 0 }
};

static SFORMAT VRC2_stateRegs[] = {
	{ &VRC2_pins, 1, "VC2E" },
};

static SFORMAT VRC4_stateRegs[] = {
	{ &VRC4_latch, 1, "V4LT" },
	{ &VRC4_mode, 1, "V4MO" },
	{ &VRC4_count, 1, "V4CT" },
	{ &VRC4_cycles, 2 | FCEUSTATE_RLSB, "V4CY" },
	{ 0 }
};

void VRC24_syncPRG (int AND, int OR) {
	setprg8(0x8000, VRC24_cbGetPRGBank(0) &AND | OR);
	setprg8(0xA000, VRC24_cbGetPRGBank(1) &AND | OR);
	setprg8(0xC000, VRC24_cbGetPRGBank(2) &AND | OR);
	setprg8(0xE000, VRC24_cbGetPRGBank(3) &AND | OR);
}

void VRC24_syncCHR (int AND, int OR) {
	setchr1(0x0000, VRC24_cbGetCHRBank(0) &AND | OR);
	setchr1(0x0400, VRC24_cbGetCHRBank(1) &AND | OR);
	setchr1(0x0800, VRC24_cbGetCHRBank(2) &AND | OR);
	setchr1(0x0C00, VRC24_cbGetCHRBank(3) &AND | OR);
	setchr1(0x1000, VRC24_cbGetCHRBank(4) &AND | OR);
	setchr1(0x1400, VRC24_cbGetCHRBank(5) &AND | OR);
	setchr1(0x1800, VRC24_cbGetCHRBank(6) &AND | OR);
	setchr1(0x1C00, VRC24_cbGetCHRBank(7) &AND | OR);
}

void VRC24_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

int VRC24_getPRGBank (uint8 bank) {
	if (~bank &1 && VRC24_misc &2) bank ^= 2;
	return bank &2? (0xFE | bank &1): VRC24_prg[bank &1];
}

int VRC24_getCHRBank (uint8 bank) {
	return VRC24_chr[bank &7];
}

void VRC24_syncMirror () {
	setmirror(VRC24_isVRC4 && VRC24_mirroring &2? (VRC24_mirroring &1? MI_1: MI_0): (VRC24_mirroring &1? MI_H: MI_V));
}

DECLFR(VRC2_readMicrowire) {
	return VRC2_pins;
}

DECLFR(VRC24_readWRAM) {
	if (VRC24_misc &1 || !VRC24_isVRC4) {
		if (VRC24_cbReadWRAM)
			return VRC24_cbReadWRAM(A);
		else
		if (WRAMSize)
			CartBR(((A -0x6000) &(WRAMSize -1)) +0x6000);
		else
			return A >>8;
	} else
		return A >>8;
}

DECLFW(VRC2_writeMicrowire) {
	VRC2_pins = V;
	VRC24_cbSync();
}

DECLFW(VRC24_writeWRAM) {
	if (VRC24_misc &1 || !VRC24_isVRC4) {
		if (WRAMSize)
			CartBW(((A -0x6000) &(WRAMSize -1)) +0x6000, V);
		if (VRC24_cbWriteWRAM)
			VRC24_cbWriteWRAM(A, V);
	}
}

DECLFW(VRC24_writeReg) {
	unsigned int index, addr;
	addr = A &0xF000 | (A &VRC24_A0? 1: 0) | (A &VRC24_A1? 2: 0); /* address as the chip sees it */
	switch (addr &0xF000) {
	case 0x8000: case 0xA000:
		VRC24_prg[addr >>13 &1] = V;
		VRC24_cbSync();
		break;
	case 0x9000:
		if (!VRC24_isVRC4 || (addr &3) == 0) {
			VRC24_mirroring = V;
			VRC24_cbSync();
		} else
		if (VRC24_isVRC4 && (addr &3) == 2) {
			VRC24_misc = V;
			VRC24_cbSync();
		}
		if (VRC24_isVRC4 && (addr &3) == 3 && VRC24_cbExternalSelect) {
			VRC24_cbExternalSelect(A, V); /* Provide original address */
		}
		break;
	case 0xF000:
		if (VRC24_isVRC4) switch (addr &3) {
			case 0: VRC4_latch = VRC4_latch &0xF0 | V &0x0F; break;
			case 1: VRC4_latch = VRC4_latch &0x0F | V <<4;   break;
			case 2: VRC4_mode = V;
				if (VRC4_mode &0x02) {
					VRC4_count = VRC4_latch;
					VRC4_cycles = 341;
				}
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 3: if (VRC24_useRepeatBit) VRC4_mode = VRC4_mode &~0x02 | VRC4_mode <<1 &0x02;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
		}
		break;
	default:
		index = (addr -0xB000) >>11 | addr >>1 &1;
		if (addr &1)
			VRC24_chr[index] = VRC24_chr[index] & 0x0F | V <<4;
		else
			VRC24_chr[index] = VRC24_chr[index] &~0x0F | V &0x0F;
		VRC24_cbSync();
		break;
	}
}

void FP_FASTAPASS(1) VRC4_cpuCycle (int a) {
	while (a--) {
		if (VRC4_mode &0x02 && (VRC4_mode &0x04 || (VRC4_cycles -= 3) <= 0)) {
			if (~VRC4_mode &0x04) VRC4_cycles += 341;
			if (!++VRC4_count) {
				VRC4_count = VRC4_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void VRC24_clear() {
	VRC24_prg[0] = 0; VRC24_prg[1] = 0;
	VRC24_chr[0] = 0; VRC24_chr[1] = 1; VRC24_chr[2] = 2; VRC24_chr[3] = 3; VRC24_chr[4] = 4; VRC24_chr[5] = 5; VRC24_chr[6] = 6; VRC24_chr[7] = 7;
	VRC24_mirroring = VRC2_pins = VRC4_latch = VRC4_mode = VRC4_count = VRC4_cycles = 0;
	VRC24_misc = 1; /* Start with WRAM enabled */
	VRC24_cbSync();
}

static void VRC24_setHandlers () {
	if (VRC24_isVRC4 || WRAMSize) {
		SetReadHandler(0x6000, 0x7FFF,  VRC24_readWRAM);
		SetWriteHandler(0x6000, 0x7FFF, VRC24_writeWRAM);
	} else {
		SetReadHandler(0x6000, 0x6FFF,  VRC2_readMicrowire);
		SetWriteHandler(0x6000, 0x6FFF, VRC2_writeMicrowire);
	}
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC24_writeReg);
	if (VRC24_isVRC4) MapIRQHook = VRC4_cpuCycle;
}

static void VRC2_configure (void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC24_cbSync = sync;
	VRC24_A0 = A0;
	VRC24_A1 = A1;
	VRC24_isVRC4 = 0;
	VRC24_cbGetPRGBank = prg? prg: VRC24_getPRGBank;
	VRC24_cbGetCHRBank = chr? chr: VRC24_getCHRBank;
	VRC24_cbReadWRAM = read;
	VRC24_cbWriteWRAM = write;
}

static void VRC4_configure (void (*sync)(), int A0, int A1, uint8 useRepeatBit, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write)), DECLFW((*externalSelect))) {
	VRC24_cbSync = sync;
	VRC24_A0 = A0;
	VRC24_A1 = A1;
	VRC24_isVRC4 = 1;
	VRC24_useRepeatBit = useRepeatBit;
	VRC24_cbGetPRGBank = prg? prg: VRC24_getPRGBank;
	VRC24_cbGetCHRBank = chr? chr: VRC24_getCHRBank;
	VRC24_cbReadWRAM = read;
	VRC24_cbWriteWRAM = write;
	VRC24_cbExternalSelect = externalSelect;
}

void VRC24_reconfigure(int A0, int A1) {
	VRC24_A0 = A0;
	VRC24_A1 = A1;
}

void VRC2_activate (uint8 clear, void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC2_configure(sync, A0, A1, prg, chr, read, write);
	VRC24_setHandlers();
	if (clear)
		VRC24_clear();
	else
		VRC24_cbSync();
}

void VRC4_activate (uint8 clear, void (*sync)(), int A0, int A1, uint8 useRepeatBit, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write)), DECLFW((*externalSelect))) {
	VRC4_configure(sync, A0, A1, useRepeatBit, prg, chr, read, write, externalSelect);
	VRC24_setHandlers();
	if (clear)
		VRC24_clear();
	else
		VRC24_cbSync();
}

void VRC2_addExState () {
	AddExState(VRC24_stateRegs, ~0, 0, 0);
	AddExState(VRC2_stateRegs, ~0, 0, 0);
}

void VRC4_addExState () {
	AddExState(VRC24_stateRegs, ~0, 0, 0);
	AddExState(VRC4_stateRegs, ~0, 0, 0);
}

void VRC24_addExState () {
	AddExState(VRC24_stateRegs, ~0, 0, 0);
	AddExState(VRC2_stateRegs, ~0, 0, 0);
	AddExState(VRC4_stateRegs, ~0, 0, 0);
}

void VRC24_restore (int version) {
	VRC24_cbSync();
}

void VRC24_power () {
	VRC24_setHandlers();
	VRC24_clear();
}

void VRC2_init (CartInfo *info, void (*sync)(), int A0, int A1, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	VRC2_addExState();
	VRC2_configure(sync, A0, A1, prg, chr, read, write);
	info->Power = VRC24_power;
	GameStateRestore = VRC24_restore;
}

void VRC4_init (CartInfo *info, void (*sync)(), int A0, int A1, uint8 useRepeatBit, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write)), DECLFW((*externalSelect))) {
	VRC4_addExState();
	VRC4_configure(sync, A0, A1, useRepeatBit, prg, chr, read, write, externalSelect);
	info->Power = VRC24_power;
	GameStateRestore = VRC24_restore;
}
