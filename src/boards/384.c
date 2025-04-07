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

static SFORMAT Mapper384_stateRegs[] ={
	{ &reg, 1, "EXP0" },
	{ 0 }
};

static void sync () {
	VRC24_syncPRG(0x0F, reg <<4);
	VRC24_syncCHR(0x7F, reg <<7);
	VRC24_syncMirror();
	VRC24_syncWRAM(0);
}

DECLFW(Mapper384_writeReg) {
	if (A &0x800 && ~reg &0x08) {
		reg =V;
		VRC24_Sync();
	}
	CartBW(A, V);
}

void Mapper384_power(void) {
	reg =0;
	VRC24_power();
}

void Mapper384_reset(void) {
	reg =0;
	VRC24_Sync();
}	

void Mapper384_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x04, 0x08, 1, 0, 2);
	VRC24_WRAMWrite =Mapper384_writeReg;
	info->Power =Mapper384_power;
	info->Reset =Mapper384_reset;
	AddExState(Mapper384_stateRegs, ~0, 0, 0);
}
