/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2008 -2020 dragon2snow,loong2snow from www.nesbbs.com
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
 *
 * 
 *
 */

#include "mapinc.h"
#include "mmc3.h"


static uint8 *WRAM;
static uint32 WRAMSIZE;

static uint8 mmc3_reg[8];
static uint8 exRegs[8];
static uint8 pointer;
static uint8 locked;
static uint8 readDIP;
static uint16 prgAND;
static uint16 chrAND;
static uint16 prgOR;
static uint16 chrOR;
static uint8 nrom;
static uint8 nrom256;
static uint16 reg;


static SFORMAT NC7000M_StateRegs[] =
{
	{ exRegs, 8, "REGS" },
	{ mmc3_reg, 8, "MREG" },
	{ &pointer, 1, "PNT0" },
	{ &readDIP, 1, "RDIP" },
	{ &prgAND, 2 | FCEUSTATE_RLSB, "PAND" },
	{ &chrAND, 2 | FCEUSTATE_RLSB, "CAND" },
	{ &prgOR, 2 | FCEUSTATE_RLSB, "PROR" },
	{ &chrOR, 2 | FCEUSTATE_RLSB, "CHOR" },
	{ &nrom, 1, "NROM" },
	{ &nrom256, 1, "N256" },
	{ &reg, 2 | FCEUSTATE_RLSB, "REG0" },
	{ 0 }
};

void NC7000MAnalyzeReg()
{
	locked = (reg & 0x80);
	prgAND = (reg & 0x08 ? 0x0F : 0x1F);
	chrAND = (reg & 0x40 ? 0x7F : 0xFF);
	prgOR = (reg << 4 & 0x30);
	chrOR = ((reg << 3) & 0x080) | (reg & 0x100);
	nrom = (reg & 0x20);
	nrom256 = (reg & 0x04);
}

int NC7000MGetPRGBank(int bank)
{
	if ((~bank & 1) && (pointer & 0x40)) bank ^= 2;
	return (bank & 2) ? 0xFE | (bank & 1) : mmc3_reg[6 | (bank & 1)];
}	
		
void NC7000MSyncPRG_GNROM(int A14, int AND, int OR) {
	setprg8(0x8000, ((NC7000MGetPRGBank(0) &~A14) &AND) | OR);
	setprg8(0xA000, ((NC7000MGetPRGBank(1) &~A14) &AND) | OR);
	setprg8(0xC000, ((NC7000MGetPRGBank(0) | A14) &AND) | OR);
	setprg8(0xE000, ((NC7000MGetPRGBank(1) | A14) &AND) | OR);
}

static void NC7000MCW(uint32 A, uint8 V) {

	setchr1(A, (V & chrAND) | (chrOR &~chrAND));
}

static void NC7000MPW(uint32 A, uint8 V) {

	if (nrom)
	{
		NC7000MSyncPRG_GNROM(nrom256 ? 2 : 0, prgAND, prgOR &~prgAND);
	}	
	else
	{
		setprg8(A, (prgOR &~prgAND) | (V & prgAND));
	}
}


static DECLFW(NC7000MWriteHi) {

	A = A & 0xE001;

	if (A < 0xC000) 
	{
		if(A==0x8000)
			pointer = MMC3_cmd ^ V;
		if(A==0x8001)
			mmc3_reg[MMC3_cmd & 0x07] = V;
		
		MMC3_CMDWrite(A, V);
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
	else
	{
		MMC3_IRQWrite(A, V);
	}			
}

static DECLFW(NC7000MWriteLo) {

	if (!(reg & 0x80)) {
		reg = V | (A & 0x100);
		NC7000MAnalyzeReg();
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
	else
	{
		WRAM[A - 0x6000] = V;
	}
}

static void NC7000MPower(void) {
	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;

	reg = 0x0000;
	
	NC7000MAnalyzeReg();
	
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, NC7000MWriteLo);
	SetWriteHandler(0x8000, 0xFFFF, NC7000MWriteHi);
}

static void NC7000MReset(void) {

	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;

	reg = 0x0000;
	
	NC7000MAnalyzeReg();
	
	MMC3RegReset();
}
static void NC7000MClose(void) {
	GenMMC3Close();
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void NC7000M_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 512, 0, 0);
	pwrap = NC7000MPW;
	cwrap = NC7000MCW;
	info->Power = NC7000MPower;
	info->Reset = NC7000MReset;
	info->Close = NC7000MClose;
	
	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	AddExState(EXPREGS, 3, 0, "EXPR");
	AddExState(NC7000M_StateRegs, ~0, 0, 0);
}
