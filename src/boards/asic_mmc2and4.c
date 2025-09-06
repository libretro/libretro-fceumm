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
#include "asic_mmc2and4.h"

static void (*MMC24_cbSync)();
static uint8 MMC24_reg[6];
static uint8 MMC24_latch[2];

static SFORMAT MMC24_state[] = {
	{ MMC24_reg,    6, "MC2R" },
	{ MMC24_latch,  2, "MC2S" },
	{ 0 }
};

static void (*MMC24_cbSync)();

void MMC24_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

void MMC2_syncPRG (int AND, int OR) {
	setprg8(0x8000, MMC24_reg[0] &AND |OR);
	setprg8(0xA000,         0xFD &AND |OR);
	setprg8(0xC000,         0xFE &AND |OR);
	setprg8(0xE000,         0xFF &AND |OR);
}

void MMC4_syncPRG (int AND, int OR) {
	setprg16(0x8000, MMC24_reg[0] &AND |OR);
	setprg16(0xC000,         0xFF &AND |OR);
}

void MMC24_syncCHR (int AND, int OR) {
	setchr4(0x0000, MMC24_reg[1 +MMC24_latch[0]] &AND |OR);
	setchr4(0x1000, MMC24_reg[3 +MMC24_latch[1]] &AND |OR);
}

void MMC24_syncMirror () {
	setmirror(MMC24_reg[5] &1? MI_H: MI_V);
}

static void FP_FASTAPASS(1) MMC24_trapPPUAddressChange (uint32 A) {
	if ((A &0x2FF0) == 0xFD0 || (A &0x2FF0) == 0xFE0) {
		MMC24_latch[A >>12 &1] = A >>5 &1;
		MMC24_cbSync();
	}
}

DECLFW(MMC24_write) {
	MMC24_reg[(A >>12) -0xA] = V;
	MMC24_cbSync();
}

static void MMC24_clear () {
	MMC24_reg[0] = 0; MMC24_reg[1] = 0; MMC24_reg[2] = 2; MMC24_reg[3] = 0; MMC24_reg[4] = 0; MMC24_reg[5] = 0;
	MMC24_latch[0] = 0; MMC24_latch[1] = 0;
	MMC24_cbSync();
}

static void MMC24_setHandlers() {
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0xA000, 0xFFFF, MMC24_write);
	PPU_hook = MMC24_trapPPUAddressChange;
}

static void MMC24_configure (void (*sync)()) {
	MMC24_cbSync = sync;
}

void MMC24_activate (uint8 clear, void (*sync)()) {
	MMC24_configure(sync);
	MMC24_setHandlers();
	if (clear)
		MMC24_clear();
	else
		MMC24_cbSync();
}

void MMC24_addExState () {
	AddExState(MMC24_state, ~0, 0, 0);
}

void MMC24_restore (int version) {
	MMC24_cbSync();
}

void MMC24_power () {
	MMC24_setHandlers();
	MMC24_clear();
}

void MMC24_init (CartInfo *info, void (*sync)()) {
	MMC24_addExState();
	MMC24_configure(sync);
	info->Power = MMC24_power;
	info->Reset = MMC24_cbSync;
	GameStateRestore = MMC24_restore;
}
