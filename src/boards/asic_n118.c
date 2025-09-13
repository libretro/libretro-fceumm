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
#include "asic_n118.h"

static void (*N118_cbSync)();
static int (*N118_cbGetPRGBank)(uint8);
static int (*N118_cbGetCHRBank)(uint8);
static uint8 N118_index;
static uint8 N118_reg[8];

static SFORMAT N118_state[] = {
	{ N118_reg,   8, "N8RG" },
	{&N118_index, 1, "N8IX" },
	{ 0 }
};

int N118_getPRGBank (uint8 bank) {
	return bank &2? (0xFE | bank &1): N118_reg[6 | bank &1];
}

int N118_getCHRBank (uint8 bank) {
	return bank &4? N118_reg[bank -2]: (N118_reg[bank >>1] &~1 | bank &1);
}

void N118_syncPRG (int AND, int OR) {
	int bank;
	for (bank = 0; bank < 4; bank++) setprg8(0x8000 | bank <<13, N118_cbGetPRGBank(bank) &AND |OR);
}

void N118_syncCHR (int AND, int OR) {
	int bank;
	for (bank = 0; bank < 8; bank++) setchr1(bank <<10, N118_cbGetCHRBank(bank) &AND |OR);
}

DECLFW (N118_writeReg) {
	if (A &1) {
		N118_reg[N118_index &7] = V;
		N118_cbSync();
	} else
		N118_index = V;
}

void N118_clear () {
	N118_reg[0] = 0; N118_reg[1] = 2; N118_reg[2] = 4; N118_reg[3] = 5; N118_reg[4] = 6; N118_reg[5] = 7; N118_reg[6] = 0; N118_reg[7] = 1;
	N118_index = 0;
	N118_cbSync();
}

static void N118_setHandlers () {
	SetReadHandler (0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0x9FFF, N118_writeReg);
}

static void N118_configure (void (*sync)(), int (*prg)(uint8), int (*chr)(uint8)) {
	N118_cbSync = sync;
	N118_cbGetPRGBank = prg? prg: N118_getPRGBank;
	N118_cbGetCHRBank = chr? chr: N118_getCHRBank;
}

void N118_activate (uint8 clear, void (*sync)(), int (*prg)(uint8), int (*chr)(uint8)) {
	N118_configure(sync, prg, chr);
	N118_setHandlers();
	if (clear)
		N118_clear();
	else
		N118_cbSync();
}

void N118_addExState () {
	AddExState(N118_state, ~0, 0, 0);
}

void N118_restore (int version) {
	N118_cbSync();
}

void N118_power () {
	N118_setHandlers();
	N118_clear();
}

void N118_init (CartInfo *info, void (*sync)(), int (*prg)(uint8), int (*chr)(uint8)) {
	N118_addExState();
	N118_configure(sync, prg, chr);
	info->Power = N118_power;
	info->Reset = N118_cbSync;
	GameStateRestore = N118_restore;
}
