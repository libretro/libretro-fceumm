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
static uint8 mask;
static uint8 compare;

extern uint32 RefreshAddr;
static writefunc writePPU;

static SFORMAT stateRegs[] ={
	{ &mask, 1, "CHRM" },
	{ &compare, 1, "CHRC" },
        { 0 }
};

static void sync () {
	int bank;
	VRC24_syncWRAM(0);
	VRC24_syncPRG(0x01F, 0x000);
	for (bank =0; bank <8; bank++) setchr1r((VRC24_chr[bank] &mask) ==compare? 0x10: 0x00, bank <<10, VRC24_chr[bank]);
	VRC24_syncMirror();
}

static DECLFW(Mapper252_253_interceptPPUWrite) {
	if (~RefreshAddr &0x2000) {
		int bank =VRC24_chr[RefreshAddr >>10 &7];
		switch(bank) {
			case 0x88: mask =0xFC; compare =0x4C; VRC24_Sync(); break;
			case 0xC2: mask =0xFE; compare =0x7C; VRC24_Sync(); break;
			case 0xC8: mask =0xFE; compare =0x04; VRC24_Sync(); break;
		}
	}
	writePPU(A, V);
}

void Mapper252_power (void) {
	mask =0xFE;
	compare =0x06;
	VRC24_power();
	writePPU =GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, Mapper252_253_interceptPPUWrite);
}

void Mapper253_power (void) {
	mask =0xFE; /* There are two board revisions, the earlier one with a non-switchable mask/compare FE/04 and a later switchable one that starts with FC/28 */
	compare =0x04;
	VRC24_power();
	writePPU =GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, Mapper252_253_interceptPPUWrite);
}

void Mapper252_253_close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
		CHRRAM =NULL;
	}
}

void Mapper252_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x4, 0x8, 1, 1, 0);
	info->Power =Mapper252_power;
	info->Close =Mapper252_253_close;
	AddExState(stateRegs, ~0, 0, 0);

	CHRRAMSize =info->iNES2? (info->CHRRamSize +info->CHRRamSaveSize): 2048;
	CHRRAM =(uint8*)FCEU_gmalloc(CHRRAMSize);
	AddExState(CHRRAM, CHRRAMSize, 0, "CRAM");
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);	
}

void Mapper253_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x4, 0x8, 1, 1, 0);
	info->Power =Mapper253_power;
	info->Close =Mapper252_253_close;
	AddExState(stateRegs, ~0, 0, 0);

	CHRRAMSize =info->iNES2? (info->CHRRamSize +info->CHRRamSaveSize): 2048;
	CHRRAM =(uint8*)FCEU_gmalloc(CHRRAMSize);
	AddExState(CHRRAM, CHRRAMSize, 0, "CRAM");
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);	
}
