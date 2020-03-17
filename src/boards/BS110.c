/* FCE Ultra - NES/Famicom Emulator
*
* Copyright notice for this file:
* Copyright (C) 2008 -2020 dragon2snow,loong2snow from www.nesbbs.com
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
*/

#include "mapinc.h"
#include "mmc3.h"

static uint8 *WRAM;
static uint32 WRAMSIZE;

static uint8 mmc3_reg[8];
static uint8 exRegs[8];
static uint8 pointer;
static uint8 readDIP;
static uint16 prgAND;
static uint16 chrAND;
static uint16 prgOR;
static uint16 chrOR;
static uint8 nrom;
static uint8 nrom128;
static uint8 dipswitch;


static SFORMAT BS110_StateRegs[] =
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

int BS110GetPRGBank(int bank)
{
	if ((~bank & 1) && (pointer & 0x40)) bank ^= 2;
	return (bank & 2) ? 0xFE | (bank & 1) : mmc3_reg[6 | (bank & 1)];
}

void BS110SyncPRG_GNROM(int A14, int AND, int OR) {
	setprg8(0x8000, ((BS110GetPRGBank(0) &~A14) &AND) | OR);
	setprg8(0xA000, ((BS110GetPRGBank(1) &~A14) &AND) | OR);
	setprg8(0xC000, ((BS110GetPRGBank(0) | A14) &AND) | OR);
	setprg8(0xE000, ((BS110GetPRGBank(1) | A14) &AND) | OR);
}

static void BS110CW(uint32 A, uint8 V) {

	uint8 block = ((exRegs[1]) & 0x03);
	uint8 mask = 0x7F;
	setchr1(A, (block << 7) | (V & mask));

}

static void BS110PW(uint32 A, uint8 V) {
	if ((exRegs[1] >> 2) & 0x01)
	{
		uint8 mask = 0x0F;
		uint8 block = (exRegs[1] & 3) << 4;
		if ((exRegs[1] >> 3) & 0x01)
		{
			setprg8(0x8000, ((BS110GetPRGBank(0)) & mask) | block);
			setprg8(0xA000, ((BS110GetPRGBank(1)) & mask) | block);
			setprg8(0xC000, ((BS110GetPRGBank(0)) & mask) | block);
			setprg8(0xE000, ((BS110GetPRGBank(1)) & mask) | block);
		}
		else
		{

			setprg8(0x8000, ((BS110GetPRGBank(0)) & mask) | block);
			setprg8(0xA000, ((BS110GetPRGBank(1)) & mask) | block);
			setprg8(0xC000, ((BS110GetPRGBank(0) | 2) & mask) | block);
			setprg8(0xE000, ((BS110GetPRGBank(1) | 2) & mask) | block);
		}
	}
	else
	{
		uint8 mask = 0x0F;
		uint8 block = (exRegs[1] & 3) << 4;
		setprg8(A, block | (V & mask));
	}
}


static DECLFW(BS110WriteHi) {

	A = A & 0xE001;

	if (A < 0xC000)
	{
		if (A == 0x8000)
			pointer = MMC3_cmd ^ V;
		if (A == 0x8001)
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

static DECLFW(BS110WriteLo) {

	exRegs[1] = A;
	exRegs[0] = V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
	
}
static DECLFR(BS110ReadHi)
{
	if ((A == 0xCB00) && ((exRegs[1] & 0x0F) == 0x08))
		return dipswitch;
	else
		return CartBR(A);
		
}

static void BS110Power(void)
{
   int i = 0;
	dipswitch = 0;
	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;

	for (i = 0;i<4;i++)
		exRegs[i] = 0;

	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, BS110WriteLo);
	SetWriteHandler(0x8000, 0xFFFF, BS110WriteHi);
	SetReadHandler(0x8000, 0xFFFF, BS110ReadHi);
}

static void BS110Reset(void)
{
	int i = 0;

	dipswitch++;
	mmc3_reg[0] = 0x00; mmc3_reg[1] = 0x02;
	mmc3_reg[2] = 0x04; mmc3_reg[3] = 0x05; mmc3_reg[4] = 0x06; mmc3_reg[5] = 0x07;
	mmc3_reg[6] = 0x00; mmc3_reg[7] = 0x01;

	for (i = 0;i<4;i++)
		exRegs[i] = 0;

	MMC3RegReset();
}
static void BS110Close(void) {
	GenMMC3Close();
	if (WRAM)
		FCEU_free(WRAM);
	WRAM = NULL;
}

void BS110_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 512, 0, 0);
	pwrap = BS110PW;
	cwrap = BS110CW;
	info->Power = BS110Power;
	info->Reset = BS110Reset;
	info->Close = BS110Close;

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	AddExState(EXPREGS, 3, 0, "EXPR");
	AddExState(BS110_StateRegs, ~0, 0, 0);
}
