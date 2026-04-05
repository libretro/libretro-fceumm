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
#include "cartram.h"

static uint8 reg[2];
static uint8 chr;

static void sync () {
	setprg8r(0x10, 0x6000, 0);
	if (reg[1] &0x04)
		setprg32(0x8000, reg[0]);
	else {
		setprg16(0x8000, reg[0]);
		setprg16(0xC000, 0);
	}
	setchr4(0x0000, reg[1] &0x02? chr: 0);
	setchr4(0x1000, 1);
	setmirror(reg[1] &0x01? MI_H: MI_V);
}

static void FP_FASTAPASS(1) trapPPUAddressChange (uint32 A) {
	if (A &0x2000 && (A &0x23C0) < 0x23C0) {
		chr = A >>(10 +(reg[1] &0x01? 1: 0)) &1;
		setchr4(0x0000, reg[1] &0x02? chr: 0);
	}
}

static DECLFR (readReg) {
	switch(A >>8 &7) {
		case 3: /* LPC state */
			return 0x8F;
		case 6: /* FDC */
			return 0x00;
		default:
			return X.DB;
	}
}

static DECLFW (writeReg) {
	switch(A >>8 &7) {
		case 0: case 2:
			reg[A >>9 &1] = V;
			sync();
			break;
		case 3: /* LPC data */
			break;
		case 5: /* FDC */
			break;
	}
}

static void reset () {
	reg[0] = reg[1] = chr = 0;
	sync();
}

static void power () {
	SetReadHandler(0x5000, 0x5FFF, readReg);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	SetWriteHandler(0x6000, 0xFFFF, CartBW);
	reset();
}

static void restore (int version) {
	sync();
}

void Mapper518_Init (CartInfo *info) {
	WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	GameStateRestore = restore;
	PPU_hook = trapPPUAddressChange;
	AddExState(reg, 2, 0, "REGS");
	AddExState(&chr, 1, 0, "CHRB");
}
