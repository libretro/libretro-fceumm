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
#include "asic_mmc1.h"

static void (*MMC1_cbSync)();
static int (*MMC1_cbGetPRGBank)(uint8);
static int (*MMC1_cbGetCHRBank)(uint8);
static DECLFR ((*MMC1_cbReadWRAM));
static DECLFW ((*MMC1_cbWriteWRAM));
static uint8 MMC1_type;
static uint8 MMC1_bits;
static uint8 MMC1_shift;
static uint8 MMC1_filter;
static uint8 MMC1_reg[4];

static SFORMAT MMC1_state[] = {
	{ MMC1_reg,    4, "MC1R" },
	{&MMC1_bits,   1, "MC1B" },
	{&MMC1_shift,  1, "MC1S" },
	{&MMC1_filter, 1, "MC1F" },
	{ 0 }
};

void MMC1_syncWRAM (int OR) {
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, OR);
}

int MMC1_getPRGBank (uint8 bank) {
	int result;
	if (MMC1_reg[0] &0x08)
		result = MMC1_reg[0] &0x04? (MMC1_reg[3] |bank*0xF): (MMC1_reg[3] &bank*0xF);
	else
		result = MMC1_reg[3] &~1 |bank;

	if (MMC1_reg[3] &0x10 && MMC1_type == MMC1_TYPE_MMC1A)
		return result &0x07 | MMC1_reg[3] &0x08;
	else
		return result &0x0F;
}

int MMC1_getCHRBank (uint8 bank) {
	if (MMC1_reg[0] &0x10)
		return MMC1_reg[1 +bank];
	else
		return MMC1_reg[1] &~1 |bank;
}

DECLFR (MMC1_readWRAM) {
	if (MMC1_type == MMC1_TYPE_MMC1A || ~MMC1_reg[3] &0x10)
		return MMC1_cbReadWRAM? MMC1_cbReadWRAM(A): CartBR(A);
	else
		return A >>8;
}

DECLFW (MMC1_writeWRAM) {
	if (MMC1_type == MMC1_TYPE_MMC1A || ~MMC1_reg[3] &0x10) {
		CartBW(A, V);
		if (MMC1_cbWriteWRAM) MMC1_cbWriteWRAM(A, V);
	}
}

void MMC1_syncPRG (int AND, int OR) {
	setprg16(0x8000, MMC1_cbGetPRGBank(0) &AND |OR);
	setprg16(0xC000, MMC1_cbGetPRGBank(1) &AND |OR);
}

void MMC1_syncCHR (int AND, int OR) {
	setchr4(0x0000, MMC1_cbGetCHRBank(0) &AND |OR);
	setchr4(0x1000, MMC1_cbGetCHRBank(1) &AND |OR);
}

void MMC1_syncMirror () {
	setmirror(MMC1_reg[0] &2? (MMC1_reg[0] &1? MI_H: MI_V): (MMC1_reg[0] &1? MI_1: MI_0));
}

void FP_FASTAPASS(1) MMC1_cpuCycle (int a) {
	while (a--) if (MMC1_filter) MMC1_filter--;
}

DECLFW (MMC1_writeReg) {
	if (V &0x80) {
		MMC1_reg[0] |= 0x0C;
		MMC1_shift = 0;
		MMC1_bits = 0;
		MMC1_cbSync();
	} else
	if (!MMC1_filter) {
		MMC1_shift |= (V &1) <<MMC1_bits++;
		if (MMC1_bits == 5) {
			MMC1_reg[A >>13 &3] = MMC1_shift;
			MMC1_shift = 0;
			MMC1_bits = 0;
			MMC1_cbSync();
		}
	}
	MMC1_filter = 2;
}

void MMC1_clear () {
	MMC1_reg[0] = 0x0C; MMC1_reg[1] = 0; MMC1_reg[2] = 0; MMC1_reg[3] = 0; /* "Bad News Baseball" is sensitive to the initial CHR bank register content. 0/0 seems to work. */
	MMC1_bits = 0; MMC1_shift = 0; MMC1_filter = 0;
	MMC1_cbSync();
}

static void MMC1_setHandlers () {
	SetReadHandler (0x6000, 0x7FFF, MMC1_readWRAM);
	SetWriteHandler(0x6000, 0x7FFF, MMC1_writeWRAM);
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, MMC1_writeReg);
	MapIRQHook = MMC1_cpuCycle;
}

static void MMC1_configure (void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC1_type = type;
	MMC1_cbSync = sync;
	MMC1_cbGetPRGBank = prg? prg: MMC1_getPRGBank;
	MMC1_cbGetCHRBank = chr? chr: MMC1_getCHRBank;
	MMC1_cbReadWRAM = read;
	MMC1_cbWriteWRAM = write;
}

void MMC1_activate (uint8 clear, void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC1_configure(sync, type, prg, chr, read, write);
	MMC1_setHandlers();
	if (clear)
		MMC1_clear();
	else
		MMC1_cbSync();
}

void MMC1_addExState () {
	AddExState(MMC1_state, ~0, 0, 0);
}

void MMC1_restore (int version) {
	MMC1_cbSync();
}

void MMC1_power () {
	MMC1_setHandlers();
	MMC1_clear();
}

void MMC1_init (CartInfo *info, void (*sync)(), uint8 type, int (*prg)(uint8), int (*chr)(uint8), DECLFR((*read)), DECLFW((*write))) {
	MMC1_addExState();
	MMC1_configure(sync, type, prg, chr, read, write);
	info->Power = MMC1_power;
	info->Reset = MMC1_cbSync;
	GameStateRestore = MMC1_restore;
}
