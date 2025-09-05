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
#include "asic_mmc3.h"

static void (*MMC3_cbSync)();
static int (*MMC3_cbGetPRGBank)(uint8);
static int (*MMC3_cbGetCHRBank)(uint8);
static DECLFR ((*MMC3_cbReadWRAM));
static DECLFW ((*MMC3_cbWriteWRAM));
static uint8 MMC3_type;
static uint8 MMC3_index;
static uint8 MMC3_reg[8];
static uint8 MMC3_mirroring;
static uint8 MMC3_wramControl;
static uint8 MMC3_reloadValue;
static uint8 MMC3_reloadRequest;
static uint8 MMC3_irqEnable;
static uint8 MMC3_counter;

static SFORMAT MMC3_state[] = {
	{ MMC3_reg,           8, "M3GS" },
	{&MMC3_index,         1, "M3IX" },
	{&MMC3_mirroring,     1, "M3NT" },
	{&MMC3_wramControl,   1, "M3WR" },
	{&MMC3_reloadRequest, 1, "M3IR" },
	{&MMC3_counter,       1, "M3IC" },
	{&MMC3_reloadValue,   1, "M3IL" },
	{&MMC3_irqEnable,     1, "M3IA" },
	{ 0 }
};

void MMC3_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

int MMC3_getPRGBank (uint8 bank) {
	if (MMC3_index &0x40 && ~bank &1) bank ^= 2;
	return bank &2? 0xFE | bank &1: MMC3_reg[6 | bank &1];
}

int MMC3_getCHRBank (uint8 bank) {
	if (MMC3_index &0x80) bank ^= 4;
	return bank &4? MMC3_reg[bank -2]: MMC3_reg[bank >>1] &~1 | bank &1;
}

DECLFR(MMC3_readWRAM) {
	if (MMC3_wramControl &0x80 || MMC3_type == MMC3_TYPE_AX5202P || MMC3_type == MMC3_TYPE_MMC6)
		return MMC3_cbReadWRAM? MMC3_cbReadWRAM(A): CartBR(A);
	else
		return A >>8;
}

DECLFW(MMC3_writeWRAM) {
	if ((MMC3_wramControl &0x80 || MMC3_type == MMC3_TYPE_AX5202P) && ~MMC3_wramControl &0x40 || MMC3_type == MMC3_TYPE_MMC6) {
		CartBW(A, V);
		if (MMC3_cbWriteWRAM) MMC3_cbWriteWRAM(A, V);
	}
}

void MMC3_syncPRG (int AND, int OR) {
	int bank;
	for (bank = 0; bank < 4; bank++) setprg8(0x8000 | bank <<13, MMC3_cbGetPRGBank(bank) &AND |OR);
}

void MMC3_syncCHR (int AND, int OR) {
	int bank;
	for (bank = 0; bank < 8; bank++) setchr1(bank <<10, MMC3_cbGetCHRBank(bank) &AND |OR);
}

void MMC3_syncMirror () {
	setmirror(MMC3_mirroring &1? MI_H: MI_V);
}

void MMC3_clockCounter () {
	uint8 prevCounter = MMC3_counter;
	MMC3_counter = MMC3_reloadRequest || !MMC3_counter? MMC3_reloadValue: --MMC3_counter;
	if ((prevCounter || MMC3_type != MMC3_TYPE_NEC || MMC3_reloadRequest) && !MMC3_counter && MMC3_irqEnable) X6502_IRQBegin(FCEU_IQEXT);
	MMC3_reloadRequest = 0;
}

DECLFW(MMC3_writeReg) {
	switch(A &0xE001) {
		case 0x8000: MMC3_index = V; break;
		case 0x8001: MMC3_reg[MMC3_index &7] = V; break;
		case 0xA000: MMC3_mirroring = V; break;
		case 0xA001: MMC3_wramControl = V; break;
		case 0xC000: MMC3_reloadValue = V; break;
		case 0xC001: MMC3_reloadRequest = 1; MMC3_counter = 0; break;
		case 0xE000: X6502_IRQEnd(FCEU_IQEXT); /* Fall-through */
		case 0xE001: MMC3_irqEnable = A &1; break;
	}
	if (A <0xC000) MMC3_cbSync();
}

static void MMC3_clear () {
	MMC3_reg[0] = 0; MMC3_reg[1] = 2; MMC3_reg[2] = 4; MMC3_reg[3] = 5; MMC3_reg[4] = 6; MMC3_reg[5] = 7; MMC3_reg[6] = 0; MMC3_reg[7] = 1;
	MMC3_index = MMC3_mirroring = MMC3_wramControl = MMC3_reloadValue = MMC3_reloadRequest = MMC3_irqEnable = MMC3_counter = 0;
	MMC3_cbSync();
}

static void MMC3_setHandlers () {
	SetReadHandler (0x6000, 0x7FFF, MMC3_readWRAM);
	SetWriteHandler(0x6000, 0x7FFF, MMC3_writeWRAM);
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, MMC3_writeReg);
	GameHBIRQHook = MMC3_clockCounter;
}

static void MMC3_configure (void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC3_type = type;
	MMC3_cbSync = sync;
	MMC3_cbGetPRGBank = prg? prg: MMC3_getPRGBank;
	MMC3_cbGetCHRBank = chr? chr: MMC3_getCHRBank;
	MMC3_cbReadWRAM = read;
	MMC3_cbWriteWRAM = write;
}

void MMC3_activate (uint8 clear, void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC3_configure(sync, type, prg, chr, read, write);
	MMC3_setHandlers();
	if (clear)
		MMC3_clear();
	else
		MMC3_cbSync();
}

void MMC3_addExState () {
	AddExState(MMC3_state, ~0, 0, 0);
}

void MMC3_restore (int version) {
	MMC3_cbSync();
}

void MMC3_power () {
	MMC3_setHandlers();
	MMC3_clear();
}

void MMC3_init (CartInfo *info, void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC3_addExState();
	MMC3_configure(sync, type, prg, chr, read, write);
	info->Power = MMC3_power;
	info->Reset = MMC3_cbSync;
	GameStateRestore = MMC3_restore;
}
