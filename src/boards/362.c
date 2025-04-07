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

static uint8 game;

static SFORMAT Mapper362_stateRegs[] ={
	{ &game, 1, "GAME" },
	{ 0 }
};

static void sync () {
	if (game &1) {
		VRC24_syncPRG(0x00F, 0x040);
		VRC24_syncCHR(0x1FF, 0x200);
		VRC24_syncMirror();
	} else {
		VRC24_syncPRG(0x00F, VRC24_chr[0] >>3 &0x30);
		VRC24_syncCHR(0x07F, VRC24_chr[0] &0x180);
		VRC24_syncMirror();
	}
}

void Mapper362_power(void) {
	game =0;
	VRC24_power();
}

void Mapper362_reset(void) {
	game ^=1;
	VRC24_Sync();
}	

void Mapper362_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 1, 0, 0);
	info->Power =Mapper362_power;
	if (PRGsize[0] >512*1024) {
		info->Reset =Mapper362_reset;
		AddExState(Mapper362_stateRegs, ~0, 0, 0);
	}
}
