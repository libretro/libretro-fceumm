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
static uint16 irqCounter;

static SFORMAT Mapper183_stateRegs[] ={
	{ &prg, 1, "PRG6" },
	{ 0 }
};

static void sync () {
	setprg8(0x6000, prg);
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

DECLFW(Mapper183_writePRG) {
	prg =A &0xFF;
	VRC24_Sync();
}

void Mapper183_power(void) {
	prg =0;
	VRC24_power();
}

void Mapper183_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x04, 0x08, 1, 1, 0);
	VRC24_WRAMRead =CartBR;
	VRC24_WRAMWrite =Mapper183_writePRG;
	AddExState(Mapper183_stateRegs, ~0, 0, 0);
	info->Power =Mapper183_power;
}
