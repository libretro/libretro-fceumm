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

static uint8 nt[4];
static uint8 prg;

static SFORMAT stateRegs[] ={
        { nt, 4, "EXPN" },
	{ &prg, 1, "PRGC" },
        { 0 }
};

static void sync () {
	int bank;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(0x01F, 0x000);
	VRC24_syncCHR(0x1FF, 0x000);
	setmirrorw(nt[0] &1, nt[1] &1, nt[2] &1, nt[3] &1);
}

int Mapper559_getPRGBank(int bank) {
	return bank ==2? prg: VRC24_getPRGBank(bank);
}

DECLFW(Mapper559_externalSelect) {
	if (A &4)
		nt[A &3] =V;
	else
		prg =V;
	VRC24_Sync();
}

DECLFW(Mapper559_nibblizeData) {
	VRC24_writeReg(A, V >>(A &0x400? 4: 0));
}

void Mapper559_power (void) {
	nt[0] =nt[1] =0xE0;
	nt[2] =nt[3] =0xE1;
	prg =0xFE;
	VRC24_power();
	SetWriteHandler(0xB000, 0xFFFF, Mapper559_nibblizeData);
}

void Mapper559_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x400, 0x800, 1, 1, 0);
	info->Power =Mapper559_power;
	VRC24_GetPRGBank =Mapper559_getPRGBank;
	VRC24_ExternalSelect =Mapper559_externalSelect;
	AddExState(stateRegs, ~0, 0, 0);
}
