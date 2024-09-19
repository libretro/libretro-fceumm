/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2012 CaH4e3
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
 * Oeka-kids board
 *
 * I might want to add some code to the mapper 96 PPU hook function
 * to not change CHR banks if the attribute table is being accessed,
 * if I make emulation a little more accurate in the future.
 *
 */

#include "mapinc.h"
#include "latch.h"

static uint8 ppulatch;
static uint16 lastAddr;

static SFORMAT StateRegs[] =
{
	{ &ppulatch, 1, "PPUL" },
	{ &lastAddr, 2, "LADR" },
	{ 0 }
};

static void Sync(void) {
	setmirror(MI_0);
	setprg32(0x8000, latch.data & 3);
	setchr4(0x0000, (latch.data & 4) | ppulatch);
	setchr4(0x1000, (latch.data & 4) | 3);
}

static void M096PPUHook(uint32 A) {
	uint16 addr = A & 0x3000;
	if ((lastAddr != 0x2000) && ((A & 0x3000) == 0x2000)) {
		ppulatch = (A >> 8) & 3;
		Sync();
	}
	lastAddr = addr;
}

static void M096Power(void) {
	ppulatch = 0;
	lastAddr = 0;
	Latch_Power();
}

void Mapper096_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, TRUE);
	info->Power = M096Power;
	PPU_hook = M096PPUHook;
	AddExState(StateRegs, ~0, 0, NULL);
}
