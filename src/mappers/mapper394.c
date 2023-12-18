/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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
#include "hw/mmc3.h"

/* Mapper 394: HSK007 circuit board that can simulate J.Y. ASIC, MMC3, and NROM. */
static uint8 HSK007Reg[4];

/* Forward declarations */
void syncPRG (int AND, int OR);
void syncCHR (int AND, int OR);
void syncNT (int AND, int OR);
void JYASIC_power(void);
void JYASIC_restoreWriteHandlers(void);
void writeALU(uint32 A, uint8 V);
void writePRG(uint32 A, uint8 V);
void writeCHRLow(uint32 A, uint8 V);
void writeCHRHigh(uint32 A, uint8 V);
void writeNT(uint32 A, uint8 V);
void writeIRQ(uint32 A, uint8 V);
void writeMode(uint32 A, uint8 V);
void trapCPUWrite(uint32 A, uint8 V);
void JYASIC_init(CartInfo *info);
uint8 readALU_DIP(uint32 A);
extern writefunc cpuWriteHandlers[0x10000]; /* Actual write handlers for CPU write trapping as a method fo IRQ clocking */
extern uint8 cpuWriteHandlersSet;
extern uint8 allowExtendedMirroring;
extern void (*JYASIC_sync)(void);

void sync394 (void) /* Called when J.Y. ASIC is active */
{
	int prgOR  =HSK007Reg[3] <<1 &0x010 | HSK007Reg[1] <<5 &0x020;
	int chrOR  =HSK007Reg[3] <<1 &0x080 | HSK007Reg[1] <<8 &0x100;
	syncPRG(0x1F, prgOR);
	syncCHR(0xFF, chrOR);
	syncNT (0xFF, chrOR);	
}
static void Mapper394_PWrap(uint32 A, uint8 V)
{
	int prgAND =HSK007Reg[3] &0x10? 0x1F: 0x0F;
	int prgOR  =HSK007Reg[3] <<1 &0x010 | HSK007Reg[1] <<5 &0x020;
	if (HSK007Reg[1] &0x08)
		setprg8(A, V &prgAND | prgOR &~prgAND);
	else
	if (A ==0x8000)
		setprg32(A, (prgOR | HSK007Reg[3] <<1 &0x0F) >>2);
	
}
static void Mapper394_CWrap(uint32 A, uint8 V)
{
	int chrAND =HSK007Reg[3] &0x80? 0xFF: 0x7F;
	int chrOR  =HSK007Reg[3] <<1 &0x080 | HSK007Reg[1] <<8 &0x100;
	setchr1(A, V &chrAND | chrOR &~chrAND);
}
static void Mapper394_Write(uint32 A, uint8 V)
{
	uint8 oldMode =HSK007Reg[1];
	A &=3;
	HSK007Reg[A] =V;
	if (A ==1)
	{		
		if (~oldMode &0x10 &&  V &0x10) JYASIC_power();
		if ( oldMode &0x10 && ~V &0x10)
		{
			JYASIC_restoreWriteHandlers();
			GenMMC3Power();
		}
	}
	else
	{
		if (HSK007Reg[1] &0x10)
			JYASIC_sync();
		else
		{
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
			
	}
}
static void Mapper394_restore (int version)
{
	int i;
	JYASIC_restoreWriteHandlers();
	if (HSK007Reg[1] &0x10)
	{		
		SetWriteHandler(0x5000, 0x5FFF, writeALU);
		SetWriteHandler(0x6000, 0x7fff, CartBW);
		SetWriteHandler(0x8000, 0x87FF, writePRG);     /* 8800-8FFF ignored */
		SetWriteHandler(0x9000, 0x97FF, writeCHRLow);  /* 9800-9FFF ignored */
		SetWriteHandler(0xA000, 0xA7FF, writeCHRHigh); /* A800-AFFF ignored */
		SetWriteHandler(0xB000, 0xB7FF, writeNT);      /* B800-BFFF ignored */
		SetWriteHandler(0xC000, 0xCFFF, writeIRQ);
		SetWriteHandler(0xD000, 0xD7FF, writeMode);    /* D800-DFFF ignored */
		
		for (i =0; i <0x10000; i++) cpuWriteHandlers[i] =GetWriteHandler(i);
		SetWriteHandler(0x0000, 0xFFFF, trapCPUWrite); /* Trap all CPU writes for IRQ clocking purposes */
		cpuWriteHandlersSet =1;
		
		SetReadHandler(0x5000, 0x5FFF, readALU_DIP);
		SetReadHandler(0x6000, 0xFFFF, CartBR);
		JYASIC_sync();
	}
	else
	{
		SetWriteHandler(0x8000, 0xBFFF, MMC3_CMDWrite);
		SetWriteHandler(0xC000, 0xFFFF, MMC3_IRQWrite);
		SetReadHandler(0x8000, 0xFFFF, CartBR);
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}
static void Mapper394_power(void)
{
	HSK007Reg[0] =0x00;
	HSK007Reg[1] =0x0F;
	HSK007Reg[2] =0x00;
	HSK007Reg[3] =0x10;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x5FFF, Mapper394_Write);
}

void Mapper394_Init(CartInfo *info)
{
	allowExtendedMirroring =1;
	JYASIC_sync =sync394;
	JYASIC_init(info);
	GenMMC3_Init(info, 128, 128, 0, 0);
	pwrap =Mapper394_PWrap;
	cwrap =Mapper394_CWrap;
	info->Reset = Mapper394_power;
	info->Power = Mapper394_power;
	AddExState(HSK007Reg, 4, 0, "HSK ");
	GameStateRestore = Mapper394_restore;
}
