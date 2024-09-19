/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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
 *
 * KS202 Asic, used for Mapper 142, and Mapper 56 with extra PAL chip
 *
 */

#include "mapinc.h"
#include "ks202.h"

static uint8 IRQa = 0;
static int32 IRQCount, IRQLatch;

static void (*WSync)(void);

KS202 ks202 = { 0 };

static SFORMAT StateRegs[] =
{
	{ &ks202.cmd, 1, "CMD" },
	{ ks202.reg, 8, "REGS" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQCount, 4, "IRQC" },
	{ 0 }
};

DECLFW(KS202_Write) {
/*	FCEU_printf("bs %04x %02x\n",A,V); */
	switch (A & 0xF000) {
	case 0x8000:
		IRQLatch = (IRQLatch & 0xFFF0) | (V & 0x0F);
		break;
	case 0x9000:
		IRQLatch = (IRQLatch & 0xFF0F) | ((V & 0x0F) << 4);
		break;
	case 0xA000:
		IRQLatch = (IRQLatch & 0xF0FF) | ((V & 0x0F) << 8);
		break;
	case 0xB000:
		IRQLatch = (IRQLatch & 0x0FFF) | (V << 12);
		break;
	case 0xC000:
		IRQa = (V & 0xF);
		if (IRQa) {
			IRQCount = IRQLatch;
		}
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xD000:
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xE000:
		ks202.cmd = V & 7;
		break;
	case 0xF000:
		ks202.reg[ks202.cmd] = V;
		WSync();
		break;
	}
}

static void KS202IRQHook(int a) {
	if (IRQa) {
		IRQCount += a;
		if (IRQCount >= 0xFFFF) {
			IRQa = 0;
			IRQCount = IRQLatch;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

void KS202_Power(void) {
	KS202_Reset();
	SetReadHandler(0x6000, 0x7FFF, CartBR);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, KS202_Write);
	if (WRAMSIZE) {
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	}
}

void KS202_Reset(void) {
	ks202.reg[0] = ks202.reg[1] = ks202.reg[2] = ks202.reg[3] = 0;
	ks202.reg[4] = ks202.reg[5] = ks202.reg[6] = ks202.reg[7] = 0;
	ks202.cmd = 0;
	IRQa = IRQCount = IRQLatch = 0;
	WSync();
}

void KS202_Close(void) {
}

void KS202_Restore(int version) {
	WSync();
}

void KS202_Init(CartInfo *info, void (*proc)(void), int wram, int battery) {
	WSync = proc;

	info->Power = KS202_Power;
	info->Close = KS202_Close;
	MapIRQHook = KS202IRQHook;
	GameStateRestore = KS202_Restore;

	AddExState(StateRegs, ~0, 0, NULL);

	if (wram) {
		WRAMSIZE = 8 * 1024;
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		if (battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
}
