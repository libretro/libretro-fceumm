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
 */

#include "mapinc.h"
#include "mmc3.h"
#include "crc32.h"

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
static uint8 nrom128;
static uint8 dipswitch;

static SFORMAT BS5652_StateRegs[] =
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
	{ &nrom128, 1, "N128" },
	{ &dipswitch, 1, "DIP0" },
	{ 0 }
};

void Bs5652AnalyzeReg()
{
	locked = exRegs[0] & 0x80;
	readDIP = exRegs[0] & 0x40;
	prgAND = exRegs[1] & 0x04 ? 0x0F : 0x1F;
	chrAND = exRegs[1] & 0x40 ? 0x7F : 0xFF;
	prgOR = (exRegs[1] & 0x03) << 4;
	chrOR = (exRegs[1] & 0x30) << 3 ;
	nrom = exRegs[0] & 0x08;
	nrom128 = exRegs[1] & 0x08;
}

int Bs5652GetPRGBank(int bank)
{
	if ((~bank & 1) && (pointer & 0x40)) bank ^= 2;
	return (bank & 2) ? 0xFE | (bank & 1) : mmc3_reg[6 | (bank & 1)];
}	
		
void Bs5652SyncPRG_GNROM(int A14, int AND, int OR) {
	setprg8(0x8000, ((Bs5652GetPRGBank(0) &~A14) &AND) | OR);
	setprg8(0xA000, ((Bs5652GetPRGBank(1) &~A14) &AND) | OR);
	setprg8(0xC000, ((Bs5652GetPRGBank(0) | A14) &AND) | OR);
	setprg8(0xE000, ((Bs5652GetPRGBank(1) | A14) &AND) | OR);
}

static void Bs5652CW(uint32 A, uint8 V) {
	
	if (exRegs[0] & 0x08)
		setchr8((exRegs[2] & 0x0F) | (exRegs[4] & 0x03) | (((exRegs[1] >> 4) & 7) << 4));
	else
		setchr1(A, (V & chrAND) | chrOR );
}

static void Bs5652PW(uint32 A, uint8 V) {
	if (nrom)
	{
		if (exRegs[3] & 0x8) /* 20190504 up2 */
		{
			if ((exRegs[1] >> 3) & 0x01)
			{
				uint8 _bank = ((exRegs[2] >> 1) & 0x07) | ((exRegs[1] & 3) << 3);
				setprg16(0x8000, _bank);
				setprg16(0xC000, _bank);
	
			}
			else
			{
				setprg32(0x8000,((exRegs[2] >> 2) & 0x03) | ((exRegs[1] & 3) << 2));
			}
		}
		else
		{
			Bs5652SyncPRG_GNROM(nrom128 ? 0 : 2, prgAND, prgOR);
		}
		
	}	
	else
	{
		if (((exRegs[1] >> 7) & 0x01))
		{
				setprg32(0x8000,((Bs5652GetPRGBank(0) >> 2) & 0x03) | ((exRegs[1] & 3) << 2));
		}
		else
			setprg8(A, prgOR | (V & prgAND));
	}
}


static DECLFW(Bs5652WriteHi) {

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

static DECLFW(Bs5652WriteLo) {
	
	if (!locked) {
		exRegs[A & 3] = V;
		Bs5652AnalyzeReg();
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
	else
	{
		if ((exRegs[0] & 0x08))
		{
			exRegs[4] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
		else
		{
			WRAM[A - 0x6000] = V;
		}
	}
}
static DECLFR(Bs5652ReadHi)
{
	if (readDIP)
		return dipswitch;
   return CartBR(A);
}

static void Bs5652Power(void)
{
	int i=0;

	dipswitch = 0;
	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;

	for(i=0;i<4;i++)
		exRegs[i]=0;
	
	Bs5652AnalyzeReg();
	
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, Bs5652WriteLo);
	SetWriteHandler(0x8000, 0xFFFF, Bs5652WriteHi);
	SetReadHandler(0x8000, 0xFFFF, Bs5652ReadHi);
}

static void Bs5652Reset(void)
{
	int i=0;

	dipswitch++;
	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;
	
	for(i=0;i<4;i++)
		exRegs[i]=0;
	
	Bs5652AnalyzeReg();
	
	MMC3RegReset();
}

static void Bs5652Close(void)
{
	GenMMC3Close();
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

void Bs5652_Init(CartInfo *info)
{
   uint32 unif_crc;

	GenMMC3_Init(info, 512, 512, 0, 0);
	pwrap = Bs5652PW;
	cwrap = Bs5652CW;
	info->Power = Bs5652Power;
	info->Reset = Bs5652Reset;
	info->Close = Bs5652Close;
	
	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	unif_crc = CalcCRC32(0, PRGptr[0], PRGsize[0]);

	if (unif_crc == 0xb97641b5) /* Fix my own error, unif CHR 0 error */
	{
		if ((CHRsize[0] == 0x2000) && (CHRsize[1] > 0x2000))
		{
			CHRsize[0] = CHRsize[1];
			CHRptr[0] = (uint8*)FCEU_gmalloc(CHRsize[1]);
			memcpy(CHRptr[0], CHRptr[1], CHRsize[1]);
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 0);
		}
	}

	AddExState(EXPREGS, 3, 0, "EXPR");
	AddExState(BS5652_StateRegs, ~0, 0, 0);
}
