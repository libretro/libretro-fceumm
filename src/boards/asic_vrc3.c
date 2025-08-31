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
#include "asic_VRC3.h"

static void (*VRC3_cbSync)();
static uint8 VRC3_prg;
static uint8 VRC3_irq;
static uint16 VRC3_count;
static uint16 VRC3_reload;

static SFORMAT VRC3_state[] = {
	{&VRC3_prg, 1, "VC3P" },
	{&VRC3_irq, 1, "VC3I" },
	{&VRC3_count, 1, "VC3C" },
	{&VRC3_reload, 1, "VC3R" },
	{ 0 }
};

void VRC3_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

void VRC3_syncPRG (int AND, int OR) {
	setprg16(0x8000, VRC3_prg &AND |OR);
	setprg16(0xC000,     0xFF &AND |OR);
}

void VRC3_syncCHR (int AND, int OR) {
	setchr8(OR);
}

DECLFW(VRC3_write) {
	int shift;
	switch (A >>12 &7) {
		case 0: case 1: case 2: case 3:
			V &=0xF;
			shift = A >>10 &0xC;
			VRC3_reload = VRC3_reload &~(0xF <<shift) | V <<shift;
			break;
		case 4:
			VRC3_irq = V;
			if (VRC3_irq &2) VRC3_count = VRC3_reload;
			break;
		case 5:
			VRC3_irq = VRC3_irq &~2 | VRC3_irq <<1 &1;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 7:
			VRC3_prg = V;
			VRC3_cbSync();
			break;
	}
}

void FP_FASTAPASS(1) VRC3_cpuCycle (int a) {
	while (a--) {
		int mask = VRC3_irq &4? 0xFF: 0xFFFF;
		if ((VRC3_count++ &mask) == mask) {
			VRC3_count = VRC3_reload;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void VRC3_clear () {
	VRC3_prg = VRC3_irq = VRC3_count = VRC3_reload = 0;
	X6502_IRQEnd(FCEU_IQEXT);
	VRC3_cbSync();
}

static void VRC3_setHandlers () {
	SetReadHandler (0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, VRC3_write);
	MapIRQHook = VRC3_cpuCycle;
}

static void VRC3_configure (void (*sync)()) {
	VRC3_cbSync = sync;
}

void VRC3_activate (uint8 clear, void (*sync)()) {
	VRC3_configure(sync);
	VRC3_setHandlers();
	if (clear)
		VRC3_clear();
	else
		VRC3_cbSync();
}

void VRC3_addExState () {
	AddExState(VRC3_state, ~0, 0, 0);
}

void VRC3_restore (int version) {
	VRC3_cbSync();
}

void VRC3_power () {
	VRC3_setHandlers();
	VRC3_clear();
}

void VRC3_init (CartInfo *info, void (*sync)()) {
	VRC3_addExState();
	VRC3_configure(sync);
	info->Power = VRC3_power;
	info->Reset = VRC3_cbSync;
	GameStateRestore = VRC3_restore;
}
