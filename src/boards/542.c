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

static SFORMAT stateRegs[] ={
        { &reg, 1, "EXP0" },
        { 0 }
};

static void sync () {
	setprg8(0x6000, 0x0F);
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
	if (reg &1) setchr1r(0x10, 0x0C00, 1);
}

DECLFW(Mapper542_writeExtra) {
	if (A &0x800) {
		reg =A >>12;
		VRC24_Sync();
	} else
		VRC24_writeReg(A, V);
}


void Mapper542_power (void) {
	reg =0;
	VRC24_power();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetWriteHandler(0xD000, 0xEFFF, Mapper542_writeExtra);
}

void Mapper542_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 1, 1, 0);
	info->Power =Mapper542_power;
	AddExState(stateRegs, ~0, 0, 0);
	SetupCartCHRMapping(0x10, NTARAM, 0x200, 1);
}
