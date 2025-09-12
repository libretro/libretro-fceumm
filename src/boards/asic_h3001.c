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
#include "asic_h3001.h"

static void (*H3001_cbSync)();
static uint8 H3001_prg[2];
static uint8 H3001_chr[8];
static uint8 H3001_layout;
static uint8 H3001_nt;
static uint8 H3001_irq;
static uint16 H3001_reload;
static uint16 H3001_count;

static SFORMAT H3001_state[] = {
	{ H3001_prg,    2, "H31P" },
	{ H3001_chr,    8, "H31C" },
	{&H3001_layout, 1, "H31L" },
	{&H3001_nt,     1, "H31M" },
	{&H3001_irq,    1, "H31N" },
	{&H3001_reload, 2 | FCEUSTATE_RLSB, "H31R" },
	{&H3001_count,  2 | FCEUSTATE_RLSB, "H31T" },
	{ 0 }
};

static int H3001_getPRGBank (uint8 bank) {
	if (H3001_layout &0x80 && ~bank &1) bank ^= 2;
	return bank &2? 0xFE | bank &1: H3001_prg[bank &1];
}

void H3001_syncPRG (int AND, int OR) {
	setprg8(0x8000, H3001_getPRGBank(0) &AND |OR);
	setprg8(0xA000, H3001_getPRGBank(1) &AND |OR);
	setprg8(0xC000, H3001_getPRGBank(2) &AND |OR);
	setprg8(0xE000, H3001_getPRGBank(3) &AND |OR);
}

void H3001_syncCHR (int AND, int OR) {
	setchr1(0x0000, H3001_chr[0]);
	setchr1(0x0400, H3001_chr[1]);
	setchr1(0x0800, H3001_chr[2]);
	setchr1(0x0C00, H3001_chr[3]);
	setchr1(0x1000, H3001_chr[4]);
	setchr1(0x1400, H3001_chr[5]);
	setchr1(0x1800, H3001_chr[6]);
	setchr1(0x1C00, H3001_chr[7]);
}

void H3001_syncMirror () {
	setmirror(H3001_nt &0x40? (H3001_nt &0x80? MI_1: MI_0): H3001_nt &0x80? MI_H: MI_V);
}

DECLFW (H3001_write) {
	switch(A >>12 &7) {
		case 0: case 2:
			H3001_prg[A >>13 &1] = V;
			H3001_cbSync();
			break;
		case 1:
			switch(A &7) {
				case 0:
					H3001_layout = V;
					H3001_cbSync();
					break;
				case 1:
					H3001_nt = V;
					H3001_cbSync();
					break;
				case 3:
					H3001_irq = V;
					X6502_IRQEnd(FCEU_IQEXT);
					break;
				case 4:
					H3001_count = H3001_reload;
					X6502_IRQEnd(FCEU_IQEXT);
					break;
				case 5:
					H3001_reload = H3001_reload &0x00FF | V <<8;
					break;
				case 6:
					H3001_reload = H3001_reload &0xFF00 | V;
					break;
			}
			break;
		case 3:
			H3001_chr[A &7] = V;
			H3001_cbSync();
			break;
	}
}

void FP_FASTAPASS(1) H3001_cpuCycle (int a) {
	while (a--) if (H3001_irq &0x80 && !--H3001_count) {
		X6502_IRQBegin(FCEU_IQEXT);
		H3001_irq = 0;
	}
}

void H3001_clear () {
	int i;
	for (i = 0; i < 2; i++) H3001_prg[i] = i;
	for (i = 0; i < 8; i++) H3001_chr[i] = i;
	H3001_layout = H3001_nt = H3001_irq = 0;
	H3001_reload = H3001_count = 0;
	H3001_cbSync();
}

static void H3001_setHandlers () {
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xBFFF, H3001_write);
	MapIRQHook =H3001_cpuCycle;
}

static void H3001_configure (void (*sync)()) {
	H3001_cbSync = sync;
}

void H3001_activate (uint8 clear, void (*sync)()) {
	H3001_configure(sync);
	H3001_setHandlers();
	if (clear)
		H3001_clear();
	else
		H3001_cbSync();

}

void H3001_addExState () {
	AddExState(H3001_state, ~0, 0, 0);
}

void H3001_restore (int version) {
	H3001_cbSync();
}

void H3001_power () {
	H3001_setHandlers();
	H3001_clear();
}

void H3001_init (CartInfo *info, void (*sync)()) {
	H3001_addExState();
	H3001_configure(sync);
	info->Power = H3001_power;
	info->Reset = H3001_cbSync;
	GameStateRestore = H3001_cbSync;
}
