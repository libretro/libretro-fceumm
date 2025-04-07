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

static uint8  prg;

static SFORMAT UNLCITYFIGHT_stateRegs[] ={
	{ &prg, 1, "PRG8" },
	{ 0 }
};

static void sync () {
	setprg32(0x8000, prg);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(UNLCITYFIGHT_externalSelect) {
	if (A &0x800)
		(GetWriteHandler(0x4011))(0x4011, V <<3 &0x78);
	else {
		prg =V >>2;
		VRC24_Sync();
	}
}

DECLFW(UNLCITYFIGHT_unscrambleAddress) {
	VRC24_writeReg(A &~0x6000 | A <<1 &0x4000 | A >>1 &0x2000, V);
}

void UNLCITYFIGHT_power(void) {
	prg =0;
	VRC24_power();
	SetWriteHandler(0x8000, 0xFFFF, UNLCITYFIGHT_unscrambleAddress);
}

void UNLCITYFIGHT_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x04, 0x08, 1, 1, 0);
	VRC24_ExternalSelect =UNLCITYFIGHT_externalSelect;
	AddExState(UNLCITYFIGHT_stateRegs, ~0, 0, 0);
	info->Power =UNLCITYFIGHT_power;
}
