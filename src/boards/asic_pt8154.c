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
#include "asic_PT8154.h"
#include "asic_mmc3.h"

static void (*PT8154_cbSync)();
static uint8 PT8154_reg;

static SFORMAT PT8154_state[] = {
	{&PT8154_reg,   1, "189R" },
	{ 0 }
};

void PT8154_syncPRG (int AND, int OR) {
	AND >>=2;
	OR >>=2;
	setprg32(0x8000, PT8154_reg &AND | OR &~AND);
}

void PT8154_syncCHR (int AND, int OR) {
	MMC3_syncCHR(AND, OR &~AND);
}

void PT8154_syncMirror () {
	MMC3_syncMirror();
}

DECLFW(PT8154_writeExtra) {
	if (A &0x100) {
		PT8154_reg = V >>4;
		PT8154_cbSync();
	}
}

DECLFW(PT8154_writeWRAM) {
	PT8154_reg = A;
	PT8154_cbSync();
}

static void PT8154_clear () {
	PT8154_reg = 0;
	PT8154_cbSync();
}

static void PT8154_setHandlers () {
	SetWriteHandler(0x4020, 0x5FFF, PT8154_writeExtra);
}

static void PT8154_configure (void (*sync)()) {
	PT8154_cbSync = sync;
}

void PT8154_activate (uint8 clear, void (*sync)()) {
	MMC3_activate(clear, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, PT8154_writeWRAM);
	PT8154_configure(sync);
	PT8154_setHandlers();
	if (clear)
		PT8154_clear();
	else
		PT8154_cbSync();
}

void PT8154_addExState () {
	AddExState(PT8154_state, ~0, 0, 0);
}

void PT8154_restore (int version) {
	PT8154_cbSync();
}

void PT8154_power () {
	MMC3_power();
	PT8154_setHandlers();
	PT8154_clear();
}

void PT8154_init (CartInfo *info, void (*sync)()) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, NULL, NULL, PT8154_writeWRAM);
	PT8154_addExState();
	PT8154_configure(PT8154_cbSync);
	info->Power = PT8154_power;
	info->Reset = PT8154_cbSync;
	GameStateRestore = PT8154_restore;
}
