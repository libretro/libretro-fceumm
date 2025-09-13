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
#include "cartram.h"

static uint8 reg;
static uint8 dip;

static SFORMAT stateRegs[] = {
	{ &reg, 1, "EXP0" },
	{ &dip, 1, "DIPS" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x0F, reg <<4);
	VRC24_syncCHR(0x7F, reg <<7);
	VRC24_syncMirror();
	VRC24_syncWRAM(0);
}

static int getPRGBank (uint8 bank) {
	if (reg &4) {
		if (~reg &2)
			return VRC24_getPRGBank(bank &1) &~2 | bank &2;
		else
			return VRC24_getPRGBank(bank &1);
	} else
		return VRC24_getPRGBank(bank);
}

static DECLFR (readPRG) {
	return CartBR(reg &8? (A &~3 | dip &3): A);
}

static DECLFW (writeReg) {
	if (~reg &1) {
		reg = A &0xFF;
		sync();
	}
	CartBW(A, V);
}

static void power (void) {
	reg = 0;
	dip = 0;
	VRC24_power();
	SetReadHandler(0x8000, 0xFFFF, readPRG);
}

static void reset (void) {
	reg = 0;
	dip++;
	sync();
}

void Mapper447_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x04, 0x08, 0, getPRGBank, NULL, NULL, writeReg, NULL );
	WRAM_init(info, 2);
	info->Power = power;
	info->Reset = reset;
	AddExState(stateRegs, ~0, 0, 0);
}
