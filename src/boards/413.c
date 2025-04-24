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

static uint8 reg[4];
static uint32 pcmAddress, pcmAddressMask;
static uint8 pcmControl;
static uint8 scanlineCounter;
static uint8 scanlineReload;
static uint8 scanlineIRQ;
static uint32 lasttime;
extern uint32 timestamp;

static SFORMAT stateRegs[] ={
	{ reg, 4, "REGS" },
	{ &pcmAddress, 4, "PCMA" },
	{ &pcmControl, 1, "PCMC" },
	{ &scanlineCounter, 1, "SLCN" },
	{ &scanlineReload,  1, "SLRL" },
	{ &scanlineIRQ,     1, "SLEN" },
	{ 0 }
};

static void sync() {
	setprg4(0x5000, 1);
	setprg8(0x6000, reg[0]);
	setprg8(0x8000, reg[1]);
	setprg8(0xA000, reg[2]);
	setprg4(0xD000, 7);
	setprg8(0xE000, 4);
	setchr4(0x0000, reg[3]);
	setchr4(0x1000, ~2);
}

static DECLFR(readPCM) {
	uint8 result =MiscROM[pcmAddress &pcmAddressMask];
	if (timestamp <lasttime || timestamp >(lasttime +6)) {
		if (pcmControl &2) ++pcmAddress;
		lasttime =timestamp;
	}
	return result;
}

static DECLFW(writeReg) {
	switch(A >>12 &7) {
		case 0:	scanlineReload =V;
			break;
		case 1:	scanlineCounter =0;
			break;
		case 2:	scanlineIRQ =0;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3:	scanlineIRQ =1;
			break;
		case 4:	pcmAddress =pcmAddress <<1 | V >>7;
			break;
		case 5:	pcmControl =V;
			break;
		case 6:
		case 7:	reg[V >>6] =V &0x3F;
			sync();
			break;
	}
}

static void horizontalBlanking () {
	scanlineCounter =!scanlineCounter? scanlineReload: --scanlineCounter;
	if (!scanlineCounter && scanlineIRQ) X6502_IRQBegin(FCEU_IQEXT);
}

static void power (void) {
	pcmAddress =pcmControl =lasttime =0;
	scanlineCounter =scanlineReload =scanlineIRQ =0;
	sync();
	SetReadHandler(0x5000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
	SetReadHandler(0xC000, 0xCFFF, readPCM);	
}

void Mapper413_Init(CartInfo *info) {
	pcmAddressMask =info->miscROMSize -1;
	GameStateRestore =sync;
	GameHBIRQHook =horizontalBlanking;
	info->Power = power;
	AddExState(stateRegs, ~0, 0, 0);
}
