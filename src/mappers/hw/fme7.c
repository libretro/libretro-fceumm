/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2023-2024 negativeExponent
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
#include "fme7.h"
#include "s5bsound.h"

static uint8 IRQa;
static int32 IRQCount;

FME7 fme7;

void (*FME7_pwrap)(uint16 A, uint16 V);
void (*FME7_cwrap)(uint16 A, uint16 V);

void (*FME7_FixWRAM)(void);
void (*FME7_FixPRG)(void);
void (*FME7_FixCHR)(void);
void (*FME7_FixMIR)(void);

static SFORMAT StateRegs[] = {
	{ fme7.prg, 4, "PREG" },
	{ fme7.chr, 8, "CREG" },
	{ &fme7.cmd, 1, "CMDR" },
	{ &fme7.mirr, 1, "MIRR" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V);
}

static void GENFIXWRAM(void) {
	if (fme7.prg[0] & 0x40) {
		setprg8r(0x10, 0x6000, fme7.prg[0] & 0x3F);
	} else {
		setprg8(0x6000, fme7.prg[0] & 0x3F);
	}
}

static void GENFIXPRG(void) {
	FME7_pwrap(0x8000, fme7.prg[1]);
	FME7_pwrap(0xA000, fme7.prg[2]);
	FME7_pwrap(0xC000, fme7.prg[3]);
	FME7_pwrap(0xE000, ~0);
}

static void GENFIXCHR(void) {
	FME7_cwrap(0x0000, fme7.chr[0]);
	FME7_cwrap(0x0400, fme7.chr[1]);
	FME7_cwrap(0x0800, fme7.chr[2]);
	FME7_cwrap(0x0C00, fme7.chr[3]);
	FME7_cwrap(0x1000, fme7.chr[4]);
	FME7_cwrap(0x1400, fme7.chr[5]);
	FME7_cwrap(0x1800, fme7.chr[6]);
	FME7_cwrap(0x1C00, fme7.chr[7]);
}

static void GENFIXMIR(void) {
	switch (fme7.mirr & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(FME7_WRAMWrite) {
	if ((fme7.prg[0] & 0xC0) == 0xC0) {
		CartBW(A, V);
	}
}

static DECLFR(FME7_WRAMRead) {
	if ((fme7.prg[0] & 0xC0) == 0x40) {
		return cpu.openbus;
	}
	return CartBR(A);
	
}

DECLFW(FME7_WriteIndex) {
	fme7.cmd = V;
}

DECLFW(FME7_WriteReg) {
	switch (fme7.cmd & 0x0F) {
	case 0x00: case 0x01: case 0x02: case 0x03:
	case 0x04: case 0x05: case 0x06: case 0x07:
		fme7.chr[fme7.cmd] = V;
		FME7_FixCHR();
		break;
	case 0x08: case 0x09: case 0x0A: case 0x0B:
		fme7.prg[fme7.cmd & 0x03] = V;
		FME7_FixPRG();
		FME7_FixWRAM();
		break;
	case 0x0C:
		fme7.mirr = V;
		FME7_FixMIR();
		break;
	case 0x0D:
		IRQa = V;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x0E:
		IRQCount = (IRQCount & 0xFF00) | (V & 0xFF);
		break;
	case 0x0F:
		IRQCount = (IRQCount & 0x00FF) | (V << 8);
		break;
	}
}

void FME7_Reset(void) {
	fme7.prg[0] = 0;
	fme7.prg[1] = 0;
	fme7.prg[2] = 1;
	fme7.prg[3] = ~1;
	fme7.chr[0] = 0;
	fme7.chr[1] = 1;
	fme7.chr[2] = 2;
	fme7.chr[3] = 3;
	fme7.chr[4] = 4;
	fme7.chr[5] = 5;
	fme7.chr[6] = 6;
	fme7.chr[7] = 7;
	fme7.cmd = 0;
	IRQCount = ~0;
	IRQa = 0;

	FME7_FixPRG();
	FME7_FixCHR();
	FME7_FixMIR();
	FME7_FixWRAM();
}

void FME7_Power(void) {
	SetReadHandler(0x8000, 0xFFFF, CartBR);

	SetWriteHandler(0x8000, 0x9FFF, FME7_WriteIndex);
	SetWriteHandler(0xA000, 0xBFFF, FME7_WriteReg);

	SetReadHandler(0x6000, 0x7FFF, FME7_WRAMRead);
	SetWriteHandler(0x6000, 0x7FFF, FME7_WRAMWrite);

	SetWriteHandler(0xC000, 0xDFFF, S5BSound_Write);
	SetWriteHandler(0xE000, 0xFFFF, S5BSound_Write);

	if (WRAM) {
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	}

	FME7_Reset();
}

static void FME7_Close(void) {
}

static void FME7_IRQHook(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQa = 0;
			IRQCount = 0xFFFF;
		}
	}
}

static void StateRestore(int version) {
	FME7_FixWRAM();
	FME7_FixPRG();
	FME7_FixCHR();
	FME7_FixMIR();
}

void FME7_Init(CartInfo *info, int wram, int battery) {
	FME7_FixPRG = GENFIXPRG;
	FME7_FixCHR = GENFIXCHR;
	FME7_FixMIR = GENFIXMIR;
	FME7_FixWRAM = GENFIXWRAM;

	FME7_pwrap = GENPWRAP;
	FME7_cwrap = GENCWRAP;

	if (wram) {
		WRAMSIZE = 8192;
		if (info->iNES2) {
			WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
		}
	}
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}

	info->Power = FME7_Power;
	info->Close = FME7_Close;
	MapIRQHook = FME7_IRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	S5BSound_ESI();
	S5BSound_AddStateInfo();
}
