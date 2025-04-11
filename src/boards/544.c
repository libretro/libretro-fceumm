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

static uint8 *CHRRAM;
static uint32 CHRRAMSize;
static uint8 nt[4];
static uint8 prg;
static uint8 mask;
static uint8 compare;

extern uint32 RefreshAddr;
static writefunc writePPU;

static SFORMAT stateRegs[] ={
        { nt, 4, "EXPN" },
	{ &prg, 1, "PRGC" },
	{ &mask, 1, "CHRM" },
	{ &compare, 1, "CHRC" },
        { 0 }
};

static void sync () {
	int bank;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(0x01F, 0x000);
	for (bank =0; bank <8; bank++) setchr1r((VRC24_chr[bank] &mask) ==compare? 0x10: 0x00, bank <<10, VRC24_chr[bank]);
	setmirrorw(nt[0] &1, nt[1] &1, nt[2] &1, nt[3] &1);
}

static const uint8 compares[8] = { 0x28, 0x00, 0x4C, 0x64, 0x46, 0x7C, 0x04, 0xFF };
static DECLFW(Mapper544_interceptPPUWrite) {
	if (~RefreshAddr &0x2000) {
		int bank =VRC24_chr[RefreshAddr >>10 &7];
		if (bank &0x80) {
			if (bank &0x10) {
				mask =0x00;
				compare =0xFF;
			} else {
				int index =(bank &0x02? 1: 0) | (bank &0x08? 2: 0) | (bank &0x40? 4: 0);
				mask =bank &0x40? 0xFE: 0xFC;
				compare =compares[index];
			}
			VRC24_Sync();
		}
	}
	writePPU(A, V);
}

int Mapper544_getPRGBank(int bank) {
	return bank ==2? prg: VRC24_getPRGBank(bank);
}

DECLFW(Mapper544_externalSelect) {
	if (A &4)
		nt[A &3] =V;
	else
		prg =V;
	VRC24_Sync();
}


void Mapper544_power (void) {
	mask =0xFC;
	compare =0x28;
	nt[0] =nt[1] =0xE0;
	nt[2] =nt[3] =0xE1;
	prg =0xFE;
	VRC24_power();
	writePPU =GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, Mapper544_interceptPPUWrite);
}

void Mapper544_close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
		CHRRAM =NULL;
	}
}

void Mapper544_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x400, 0x800, 1, 1, 0);
	info->Power =Mapper544_power;
	info->Close =Mapper544_close;
	VRC24_GetPRGBank =Mapper544_getPRGBank;
	VRC24_ExternalSelect =Mapper544_externalSelect;
	AddExState(stateRegs, ~0, 0, 0);

	CHRRAMSize =info->CHRRamSize +info->CHRRamSaveSize;
	CHRRAM =(uint8*)FCEU_gmalloc(CHRRAMSize);
	AddExState(CHRRAM, CHRRAMSize, 0, "CRAM");
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);	
}
