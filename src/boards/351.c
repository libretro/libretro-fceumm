/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022 NewRisingSun
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
#include "asic_mmc1.h"
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8 reg[4], dip;
static uint8 *CHRRAM = NULL;
static uint8 *PRGCHR = NULL;
static int prgMask_CHRROM; /* PRG-ROM bank mask when CHR-ROM is active (outside of PRG address space */
static int prgMask_CHRRAM; /* PRG-ROM bank mask when CHR-RAM is active (CHR-ROM becomes part of PRG address space) */

static SFORMAT stateRegs[] = {
	{ reg,  4, "REGS" },
	{ &dip, 1, "DIPS" },
	{ 0 }
};

static void sync () {
	int prgAND = reg[2] &0x10? 0x00: reg[2] &0x04? 0x0F: 0x1F; /* No inner bank in NROM mode, 128K or 256K for others */
	int chrAND = reg[2] &0x40? 0x00: reg[2] &0x20? 0x7F: reg[2] &0x10? 0x1F: 0xFF; /* No inner bank in (C)NROM mode, 128K or 256K for others */
	int prgOR  = reg[1] >>1 &(reg[2] &0x01 && CHRRAM? prgMask_CHRRAM: prgMask_CHRROM) &~prgAND;
	int chrOR  = reg[0] <<1 &~chrAND;
	
	if (reg[2] &0x10) { /* NROM mode */
		if (reg[2] &0x08) { /* NROM-64 */
			setprg8(0x8000, prgOR);
			setprg8(0xA000, prgOR);
			setprg8(0xC000, prgOR);
			setprg8(0xE000, prgOR);
		} else
		if (reg[2] &0x04) { /* NROM-128 */
			setprg16(0x8000, prgOR >>1);
			setprg16(0xC000, prgOR >>1);
		} else      /* NROM-256 */
			setprg32(0x8000, prgOR >>2);
	} else
	if (~reg[0] &0x02)
		MMC3_syncPRG(prgAND, prgOR);
	else
	if (reg[0] &0x01)
		VRC24_syncPRG(prgAND, prgOR);
	else
		MMC1_syncPRG(prgAND >>1, prgOR >>1);
		
	if (reg[2] &0x01 && CHRRAM)
		setchr8r(0x10, 0);
	else
	if (reg[2] &0x40)
		setchr8(chrOR >>3);
	else
	if (~reg[0] &0x02)
		MMC3_syncCHR(chrAND, chrOR);
	else
	if (reg[0] &0x01)
		VRC24_syncCHR(chrAND, chrOR);
	else
		MMC1_syncCHR(chrAND >>2, chrOR >>2);
	
	if (~reg[0] &0x02)
		MMC3_syncMirror();
	else
	if (reg[0] &0x01)
		VRC24_syncMirror();
	else
		MMC1_syncMirror();
}

DECLFW(VRC24_trapWriteReg) { /* When A11 is set, VRC4's A0 and A1 are swapped */
	if (A &0x800) A = A &~0xF | A >>1 &0x5 | A <<1 &0xA;
	VRC24_writeReg(A, V);
}

static void applyMode (uint8 clear) {
	PPU_hook = NULL;
	MapIRQHook = NULL;
	GameHBIRQHook = NULL;
	if (~reg[0] &0x02)
		MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
	else
	if (reg[0] &0x01) {
		if (reg[2] &0x04)
			VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
		else
			VRC4_activate(clear, sync, 0x02, 0x04, 1, NULL, NULL, NULL, NULL, NULL);
		SetWriteHandler(0x8000, 0xFFFF, VRC24_trapWriteReg);
	} else
		MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
}

static void Mapper351_restore (int version) {
	applyMode(0);
	sync();
}

static DECLFR(readDIP) {
	return dip;
}

static DECLFW(writeReg) {
	reg[A &3] = V;
	applyMode(A == 2);
}

static DECLFW(writeFDSMirroring) {
	MMC3_writeReg(0xA000, V >>3 &1);
}

static void Mapper351_power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip = 0;
	SetReadHandler(0x5000, 0x5FFF, readDIP);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	SetWriteHandler(0x4025, 0x4025, writeFDSMirroring);
	applyMode(1);
}

static void Mapper351_reset (void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip++;
	applyMode(1);
}

static void Mapper351_close(void) {
	if (CHRRAM) FCEU_gfree(CHRRAM);
	if (PRGCHR) FCEU_gfree(PRGCHR);
	CHRRAM = NULL;
	PRGCHR = NULL;
}

void Mapper351_Init (CartInfo *info) {
	int CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;
	MMC1_addExState();
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = Mapper351_reset;
	info->Power = Mapper351_power;
	info->Close = Mapper351_close;
	GameStateRestore = Mapper351_restore;
	AddExState(stateRegs, ~0, 0, 0);	

 	/* When CHR-RAM is enabled, CHR-ROM becomes part of PRG-ROM address space. */
	prgMask_CHRROM = prgMask_CHRRAM = PRGsize[0] /8192 -1;
	if (CHRRAMSIZE) {
		CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");		
		prgMask_CHRRAM = (PRGsize[0] +CHRsize[0]) /8192 -1;		
		uint8* newROM = (uint8*)FCEU_gmalloc(PRGsize[0] +CHRsize[0]);
		memcpy(newROM,              ROM, info->PRGRomSize);
		memcpy(newROM +PRGsize[0], VROM, info->CHRRomSize);
		FCEU_gfree(ROM);
		ROM = newROM;
		SetupCartPRGMapping(0, ROM, PRGsize[0] +CHRsize[0], 0);
	}
}

