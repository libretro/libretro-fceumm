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
#include "asic_tc3294.h"
#include "asic_mmc3.h"

static void (*TC3294_cbSync)();
static uint8 TC3294_index;
static uint8 TC3294_reg[4];

static SFORMAT TC3294_state[] = {
	{ TC3294_reg,   8, "329S" },
	{&TC3294_index, 1, "329I" },
	{ 0 }
};

void TC3294_syncWRAM (int OR) {
	MMC3_syncWRAM(OR);
}

void TC3294_syncPRG (int AND, int OR) {
	int prgAND = ~TC3294_reg[3] &0x3F;
	int prgOR  = TC3294_reg[1] | TC3294_reg[2] <<2 &0x300;
	MMC3_syncPRG(prgAND &AND, prgOR &~prgAND &AND | OR &~AND);
}

void TC3294_syncCHR (int AND, int OR) {
	int chrAND = 0xFF >>(~TC3294_reg[2] &0xF);
	int chrOR  = TC3294_reg[0] | TC3294_reg[2] <<4 &0xF00;
	MMC3_syncCHR(chrAND &AND, chrOR &~chrAND &AND | OR &~AND);
}

void TC3294_syncMirror () {
	MMC3_syncMirror();
}

DECLFW(TC3294_write) {
	if (~TC3294_reg[3] &0x40) {
		TC3294_reg[TC3294_index++ &3] = V;
		TC3294_cbSync();
	}
}

static void TC3294_clear () {
	TC3294_reg[0] = 0x00; TC3294_reg[1] = 0x00; TC3294_reg[2] = 0x0F; TC3294_reg[3] = 0x00;
	TC3294_index = 0;
	TC3294_cbSync();
}

static void TC3294_setHandlers () {
}

static void TC3294_configure (void (*sync)()) {
	TC3294_cbSync = sync;
}

void TC3294_activate (uint8 clear, void (*sync)()) {
	MMC3_activate(clear, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, TC3294_write);
	TC3294_configure(sync);
	TC3294_setHandlers();
	if (clear)
		TC3294_clear();
	else
		TC3294_cbSync();
}

void TC3294_addExState () {
	AddExState(TC3294_state, ~0, 0, 0);
}

void TC3294_restore (int version) {
	TC3294_cbSync();
}

void TC3294_power () {
	MMC3_power();
	TC3294_setHandlers();
	TC3294_clear();
}

void TC3294_init (CartInfo *info, void (*sync)()) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, TC3294_write);
	TC3294_addExState();
	TC3294_configure(TC3294_cbSync);
	info->Power = TC3294_power;
	info->Reset = TC3294_cbSync;
	GameStateRestore = TC3294_restore;
}
