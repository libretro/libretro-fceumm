/* FCEUmm - NES/Famicom Emulator
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
#include "mmc3.h"

static uint8 submapper;
static uint8 *CHRRAM =NULL;
static uint32 CHRRAMSIZE;

static void PRGWrap0(uint32 A, uint8 V) {
	int prgAND =EXPREGS[0] &0x20? 0x1F: 0x0F;
	setprg8(A, V &prgAND | EXPREGS[0] <<4 &~prgAND);
}

static void CHRWrap0(uint32 A, uint8 V) {
	int chrAND =EXPREGS[0] &0x20? 0xFF: 0x7F;
	setchr1(A, V &chrAND | EXPREGS[0] <<7 &~chrAND);
}

static void PRGWrap1(uint32 A, uint8 V) {
	int prgAND =(EXPREGS[0] &0x1F) ==0x1F? 0x1F: 0x0F;
	setprg8(A, V &prgAND | EXPREGS[0] <<4 &~prgAND);
}

static void CHRWrap1(uint32 A, uint8 V) {
	int chrAND =(EXPREGS[0] &0x1F) ==0x19? 0xFF: 0x7F;
	if (EXPREGS[0] &0x20)
		setchr8r(0x10, 0);
	else
		setchr1(A, V &chrAND | EXPREGS[0] <<7 &~chrAND);
}

static void PRGWrap2(uint32 A, uint8 V) {
	int prgAND =(EXPREGS[0] &0x0E) ==0x0E? 0x1F: 0x0F;
	if (EXPREGS[0] &0x20)
		setprg32(0x8000, EXPREGS[1] &3 | EXPREGS[0] <<2 &~3);
	else
		setprg8(A, V &prgAND | EXPREGS[0] <<4 &~prgAND);
}

static void CHRWrap2(uint32 A, uint8 V) {
	int chrAND =(EXPREGS[0] &0x0F) ==0x03 || (EXPREGS[0] &0x0F) ==0x0F? 0xFF: 0x7F;
	if (EXPREGS[0] &0x10) {
		setchr2(0x0000, DRegBuf[0] &0xFE | 0x400);
		setchr2(0x0800, DRegBuf[1] |0x01 | 0x400);
		setchr2(0x1000, DRegBuf[2]       | 0x400);
		setchr2(0x1800, DRegBuf[5]       | 0x400);
	} else
		setchr1(A, V &chrAND | EXPREGS[0] <<7 &~chrAND);
}

static DECLFW(WriteReg) {
	if (submapper ==2 && EXPREGS[0] &0x30)
		EXPREGS[1] =A &0x100? (A >>4): (V <<1 &2 | V >>4 &1);
	else
		EXPREGS[0] =A &0xFF;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void Power(void) {
	EXPREGS[0] =0;
	EXPREGS[1] =3;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, WriteReg);
}

static void Reset(void) {
	EXPREGS[0] =0;
	EXPREGS[1] =3;
	MMC3RegReset();
}

static void Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	CHRRAM =NULL;
}

void Mapper480_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 256, 256, info->iNES2? (info->PRGRamSize + info->PRGRamSaveSize) /1024: 8, info->battery);
	pwrap = submapper ==2? PRGWrap2: submapper==1? PRGWrap1: PRGWrap0;
	cwrap = submapper ==2? CHRWrap2: submapper==1? CHRWrap1: CHRWrap0;
	info->Power = Power;
	info->Reset = Reset;
	info->Close = Close;
	AddExState(EXPREGS, 2, 0, "EXPR");
	if (submapper ==1) {
		CHRRAMSIZE = 8192;
		CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");		
	}
}
