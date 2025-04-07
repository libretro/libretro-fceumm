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

static SFORMAT Mapper398_stateRegs[] ={
	{ &reg, 1, "EXP0" },
	{ 0 }
};

static void sync () {
	if (reg &0x80) {
		setprg32(0x8000, reg >>5 &6 | VRC24_chr[0] >>2 &1);
		setchr8(0x40 | reg >>3 &8 | VRC24_chr[0] &7);
	} else {
		VRC24_syncPRG(0x0F, 0x00);
		VRC24_syncCHR(0x1FF, 0x000);
	}
	VRC24_syncMirror();
}

DECLFW(Mapper398_writeReg) {
	reg =A &0xFF;	
	VRC24_Sync();
	VRC24_writeReg(A, V);
}

void Mapper398_power(void) {
	reg =0xC0;
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, Mapper398_writeReg);
}

void Mapper398_reset(void) {
	reg =0xC0;
	VRC24_Sync();
}	

void Mapper398_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 1, 1, 0);
	info->Power =Mapper398_power;
	info->Reset =Mapper398_reset;
	AddExState(Mapper398_stateRegs, ~0, 0, 0);
}
