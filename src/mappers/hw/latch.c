/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
#include "latch.h"

static uint8 bus_conflict;
static void (*WSync)(void);
static readfunc defread;

LATCH latch;

DECLFW(Latch_Write) {
	/*	FCEU_printf("bs %04x %02x\n",A,V); */
	if (bus_conflict) {
		V &= CartBR(A);
	}
    latch.addr = A;
    latch.data = V;
	WSync();
}

void Latch_RegReset(void) {
	latch.addr = 0;
	latch.data = 0;
	WSync();
}

void Latch_Power(void) {
	Latch_RegReset();
	if (WRAM) {
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	}
	SetReadHandler(0x8000, 0xFFFF, defread);
	SetWriteHandler(0x8000, 0xFFFF, Latch_Write);
}

void Latch_Close(void) {
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
	info->Power = Latch_Power;
	info->Close = Latch_Close;
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
