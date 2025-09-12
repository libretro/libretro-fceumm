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

static SFORMAT stateRegs[] ={
	{ &reg, 1, "EXP0" },
	{ 0 }
};

static void sync () {
	if (reg &8) { /* AOROM */
		setprg32(0x8000, VRC24_getPRGBank(0) &0x07 | reg <<2 &~0x07);
		setmirror(VRC24_getPRGBank(0) &0x10? MI_1: MI_0);
	} else {
		if (reg &4) { /* UOROM */
			setprg16(0x8000, VRC24_getPRGBank(0) &0xF | reg <<3 &~0xF);
			setprg16(0xC000,                      0xF | reg <<3 &~0xF);
		} else { /* UNROM */
			setprg16(0x8000, VRC24_getPRGBank(0) &0x7 | reg <<3 &~0x7);
			setprg16(0xC000,                      0x7 | reg <<3 &~0x7);
		}
		VRC24_syncMirror();
	}
	setchr8(0);
}

static DECLFW (writeReg) {
	reg =A &0xFF;
	sync();
	CartBW(A, V);
}

static DECLFW (writePRG) {
	VRC24_writeReg(reg &8? 0x8000: A, V);
}

static void power (void) {
	reg =0;
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, writePRG);
}

static void reset (void) {
	reg =0;
	VRC24_clear();
}

void Mapper448_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x04, 0x08, 0, NULL, NULL, NULL, writeReg, NULL);
	WRAM_init(info, 2);
	info->Power =power;
	info->Reset =reset;
	AddExState(stateRegs, ~0, 0, 0);
}
