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
#include "asic_mmc2.h"

static void (*MMC2_cbSync)();
static uint8 MMC2_reg[6];
static uint8 MMC2_latch[2];

static SFORMAT MMC2_state[] = {
	{ MMC2_reg,    6, "MC2R" },
	{ MMC2_latch,  2, "MC2S" },
	{ 0 }
};

static void (*MMC2_cbSync)();

void MMC2_syncPRG (int AND, int OR) {
	setprg8(0x8000, MMC2_reg[0] &AND |OR);
	setprg8(0xA000,        0xD &AND |OR);
	setprg8(0xC000,        0xE &AND |OR);
	setprg8(0xE000,        0xF &AND |OR);
}

void MMC2_syncCHR (int AND, int OR) {
	setchr4(0x0000, MMC2_reg[1 +MMC2_latch[0]] &AND |OR);
	setchr4(0x1000, MMC2_reg[3 +MMC2_latch[1]] &AND |OR);
}

void MMC2_syncMirror () {
	setmirror(MMC2_reg[5] &1? MI_H: MI_V);
}

static void FP_FASTAPASS(1) MMC2_trapPPUAddressChange (uint32 A) {
	if ((A &0x2FF0) == 0xFD0 || (A &0x2FF0) == 0xFE0) {
		MMC2_latch[A >>12 &1] = A >>5 &1;
		MMC2_cbSync();
	}
}

DECLFW(MMC2_write) {
	MMC2_reg[(A >>12) -0xA] = V;
	MMC2_cbSync();
}

static void MMC2_clear () {
	MMC2_reg[0] = 0; MMC2_reg[1] = 0; MMC2_reg[2] = 2; MMC2_reg[3] = 0; MMC2_reg[4] = 0; MMC2_reg[5] = 0;
	MMC2_latch[0] = 0; MMC2_latch[1] = 0;
	MMC2_cbSync();
}

static void MMC2_setHandlers() {
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0xA000, 0xFFFF, MMC2_write);
	PPU_hook = MMC2_trapPPUAddressChange;
}

static void MMC2_configure (void (*sync)()) {
	MMC2_cbSync = sync;
}

void MMC2_activate (uint8 clear, void (*sync)()) {
	MMC2_configure(sync);
	MMC2_setHandlers();
	if (clear)
		MMC2_clear();
	else
		MMC2_cbSync();
}

void MMC2_addExState () {
	AddExState(MMC2_state, ~0, 0, 0);
}

void MMC2_restore (int version) {
	MMC2_cbSync();
}

void MMC2_power () {
	MMC2_setHandlers();
	MMC2_clear();
}

void MMC2_init (CartInfo *info, void (*sync)()) {
	MMC2_addExState();
	MMC2_configure(sync);
	info->Power = MMC2_power;
	info->Reset = MMC2_cbSync;
	GameStateRestore = MMC2_restore;
}
