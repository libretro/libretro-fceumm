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

/* BlazePro CPLD-based multicarts 

   Unsolved issue: how is CHR RAM write-protection triggered?
*/

#include "mapinc.h"
#include "state.h"

static uint8 submapper;
static uint8 eeprom[16], clock, state, command, output; /* Some strange serial EEPROM */
static uint8 *WRAM;
static uint32 WRAMSIZE;

static int prevSFEXINDEX;
extern int SFEXINDEX;
extern SFORMAT SFMDATA[64];

static uint8 mapper;      /* 5700 MSB >>4 OR'd with submapper <<4 */
static uint8 mapperFlags; /* 5700 LSB */
static uint8 misc;        /* 5601 */
static uint8 misc2;       /* 5702 */
static void (*sync)();

static uint16 prgOR;
static uint8  prgAND;

static uint8  regByte[16];
static int16  regWord[9];

#include "468_mmc1.h"
#include "468_mmc24.h"
#include "468_mmc3.h"
#include "468_vrc1.h"
#include "468_vrc24.h"
#include "468_vrc3.h"
#include "468_vrc6.h"
#include "468_vrc7.h"
#include "468_fme7.h"
#include "468_discrete.h"
#include "468_cnrom.h"
#include "468_if12.h"
#include "468_lf36.h"
#include "468_nanjing.h"

static SFORMAT stateRegs[] = {
	{ &mapper,          1, "SUP0" },
	{ &mapperFlags,     1, "SUP1" },
	{ &misc,            1, "SUP2" },
	{ &misc2,           1, "SUP3" },
	{ &prgOR,           2, "SUP4" },
	{ &prgAND,          1, "SUP5" },
	{  eeprom,          16,"EEPR" },
	{ &clock,           1, "EEP0" },
	{ &state,           1, "EEP1" },
	{ &command,         1, "EEP2" },
	{ &output,          1, "EEP3" },	
	{ regByte,          16,"REGB" },
	{ regWord,          16,"REGW" },
	{ 0 }
};

void setPins(uint8 select, uint8 newClock, uint8 newData) { /* Serial EEPROM */
	if (select)
		state =0;
	else
	if (!clock && newClock) {
		if (state <8) {
			command =command <<1 | newData*1;
			if (++state ==8 && (command &0xF0) !=0x50 && (command &0xF0) !=0xA0) state =0;
		} else {
			int mask =1 <<(15 -state);
			int address =command &0x0F;
			if ((command &0xF0) ==0xA0)
				eeprom[address] =eeprom[address] &~mask | newData*mask;
			else
			if ((command &0xF0) ==0x50)
				output =!!(eeprom[address] &mask);
			
			if (++state ==16) state =0;
		}
	}
	clock =newClock;
}

static DECLFR(readReg);
static DECLFW(writeReg);
static void setMapper(uint8 clearRegs) {
	int i;
	if (clearRegs) {
		for (i =0; i <16; i++) regByte[i] =0;
		for (i =0; i < 8; i++) regWord[i] =0;
		X6502_IRQEnd(FCEU_IQEXT);
	}
	SetReadHandler(0x5000, 0x5FFF, readReg);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	SetWriteHandler(0x6000, 0xFFFF, CartBW);
	MapIRQHook = NULL;
	PPU_hook = NULL;
	GameHBIRQHook = NULL;
	setprg8r(0x10, 0x6000, 0);

	switch(mapper) { /* 5700 MSB >>4 OR'd with submapper <<4 */
	case 0x00: case 0x01: case 0x32:            MMC1_reset(clearRegs); break;
	case 0x0A:                                  MMC2_reset(clearRegs); break;
	case 0x10: case 0x11: case 0x12:            MMC3_reset(clearRegs); break;
	case 0x08:                                  MMC4_reset(clearRegs); break;
	case 0x40:                                  VRC1_reset(clearRegs); break;
	case 0x20: case 0x21: case 0x22: case 0x23: VRC24_reset(clearRegs); break;
	case 0x44:                                  VRC3_reset(clearRegs); break;
	case 0x30: case 0x31:                       VRC6_reset(clearRegs); break;
	case 0x41:                                  VRC7_reset(clearRegs); break;
	case 0x07:                                  LF36_reset(clearRegs); break;
	case 0x50:                                  FME7_reset(clearRegs); break;
	case 0x0E: case 0x1E:                       NANJING_reset(clearRegs); break;
	case 0x09: case 0x0B: case 0x17: case 0x37: UNROM_IF12_reset(clearRegs); break;
	case 0x04: case 0x06: case 0x14: case 0x16: ANROM_BNROM_reset(clearRegs); break;
	case 0x05: case 0x15:                       CNROM_BF9097_reset(clearRegs); break;
	case 0x0C: case 0x0D: case 0x1C: case 0x1D: GNROM_reset(clearRegs); break;
	default:                                    break;
	}
	sync();
}

static DECLFR(readReg) {
	switch(A) {
	case 0x5301: case 0x5601:
		return output? 0x80: 0x00;
	default:
		return 0xFF;
	}
}

static DECLFW(writeReg) {
	switch(A) {
	case 0x5301:
		if (submapper ==0) setPins(!!(V &0x04), !!(V &0x02), !!(V &0x01));
		break;
	case 0x5601:
		if (~misc &0x80) {
			misc =V;
			if (submapper !=1) prgOR =prgOR &~0x2000 | V <<9 &0x2000;
			sync();
		}
		if (submapper ==1) setPins(!!(V &0x10), !!(V &0x02), !!(V &0x01));
		break;
	case 0x5700:
		mapper =V >>4 | submapper <<4;
		mapperFlags =V &0xF;
		prgOR =prgOR &~0x0010 | V <<4 &0x0010;
		setMapper(1);
		break;
	case 0x5701:
		prgOR =prgOR &~0x1FE0 | V <<5 &0x1FE0;
		sync();
		break;
	case 0x5702:
		if (submapper ==1) {
			misc2 =V;
			sync();
		}
		break;
	}
}

static void reset(void) {
	mapper =submapper <<4;
	mapperFlags =0x0F;
	misc =0;
	prgOR =0x7FF0;
	clock =command =output =1;
	command =state =0;
	setMapper(1);
}

static void power(void) {
	int i;
	for (i =0; i <16; i++) eeprom[i] =0;
	reset();
}

static void close(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void stateRestore(int version) {
	setMapper(0);
}

void Mapper468_Init(CartInfo *info) {
	submapper =info->submapper;
	info->Reset =reset;
	info->Power =power;
	info->Close =close;
	GameStateRestore =stateRestore;

	WRAMSIZE =8192;
	WRAM =(uint8*) FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	AddExState(stateRegs, ~0, 0, 0);
	prevSFEXINDEX =SFEXINDEX;
}
