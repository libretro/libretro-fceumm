/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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
#include "latch.h"

static uint8 bus_conflict;
static uint8 *WRAM = NULL;
static uint32 WRAMSIZE;
static void (*WSync)(void);
static readfunc defread;

LATCH latch;

DECLFW(LatchWrite) {
	/*	FCEU_printf("bs %04x %02x\n",A,V); */
	if (bus_conflict)
		V &= CartBR(A);
    latch.addr = A;
    latch.data = V;
	WSync();
}

void LatchHardReset() {
	latch.addr = 0;
	latch.data = 0;
	WSync();
}

void LatchPower(void) {
	LatchHardReset();
	WSync();
	if (WRAM) {
		SetReadHandler(0x6000, 0xFFFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	} else {
		SetReadHandler(0x8000, 0xFFFF, defread);
	}
	SetWriteHandler(0x8000, 0xFFFF, LatchWrite);
}

void LatchClose(void) {
	if (WRAM)
		FCEU_gfree(WRAM);
	WRAM = NULL;
}

static void LatchReset(void) {
	WSync();
}

static void StateRestore(int version) {
	WSync();
}

void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func,
    uint8 wram, uint8 busc) {
	bus_conflict = busc;
	WSync        = proc;
	if (func != NULL)
		defread = func;
	else
		defread = CartBROB;
	info->Power = LatchPower;
	info->Close = LatchClose;
	info->Reset = LatchReset;
	GameStateRestore = StateRestore;
	if (wram) {
		WRAMSIZE = 8192;
		WRAM     = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		if (info->battery) {
			info->SaveGame[0]    = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
	AddExState(&latch.addr, 2, 0, "ADDR");
	AddExState(&latch.data, 1, 0, "DATA");
}
