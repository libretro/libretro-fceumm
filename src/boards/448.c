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
#include "vrc2and4.h"

static uint8 reg;

static SFORMAT Mapper448_stateRegs[] ={
	{ &reg, 1, "EXP0" },
	{ 0 }
};

static void sync () {
	if (reg &8) { /* AOROM */
		setprg32(0x8000, VRC24_prg[0] &0x07 | reg <<2 &~0x07);
		setmirror(VRC24_prg[0] &0x10? MI_1: MI_0);
	} else {
		if (reg &4) { /* UOROM */
			setprg16(0x8000, VRC24_prg[0] &0xF | reg <<3 &~0xF);
			setprg16(0xC000,               0xF | reg <<3 &~0xF);
		} else { /* UNROM */
			setprg16(0x8000, VRC24_prg[0] &0x7 | reg <<3 &~0x7);
			setprg16(0xC000,               0x7 | reg <<3 &~0x7);
		}
		VRC24_syncMirror();
	}
	setchr8(0);
}

DECLFW(Mapper448_writeReg) {
	if (VRC24_misc &1) {
		reg =A &0xFF;
		VRC24_Sync();
	}
	CartBW(A, V);
}

DECLFW(Mapper448_writePRG) {
	VRC24_writeReg(reg &8? 0x8000: A, V);
}

void Mapper448_power(void) {
	reg =0;
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, Mapper448_writePRG);
}

void Mapper448_reset(void) {
	reg =0;
	VRC24_Sync();
}	

void Mapper448_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x04, 0x08, 1, 0, 2);
	VRC24_WRAMWrite =Mapper448_writeReg;
	info->Power =Mapper448_power;
	info->Reset =Mapper448_reset;
	AddExState(Mapper448_stateRegs, ~0, 0, 0);
}
