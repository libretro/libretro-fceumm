/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2015 CaH4e3
 *  Copyright (C) 2020 dragon2snow,loong2snow from www.nesbbs.com
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

/* This source code file only applies to Sanchez' original UNIF file with the incorrect bank order.
   The correctly-specified mapper, used for all NES 2.0 ROM files, is implemented in 319.c. */

static uint8 regs[2];

static SFORMAT StateRegs[] =
{
	{ regs, 2, "REGS" },
	{ 0 }
};

static void HP898FSync(void) {	
	uint8 chr = (regs[0] >> 4) & 7;
	uint8 prg = (regs[1] >> 3) & 7;
	uint8 dec = (regs[1] >> 4) & 4;
	setchr8(chr & (~(((regs[0] & 1) << 2) | (regs[0] & 2))));
	setprg16(0x8000, prg & (~dec));
	setprg16(0xC000, prg | dec);
	setmirror(regs[1] >> 7);
}

static void HP898FWrite(uint32 A, uint8 V) {
	if((A & 0x6000) == 0x6000) {
		regs[(A & 4) >> 2] = V;
		HP898FSync();
	}
}

static void HP898FPower(void) {
	regs[0] = regs[1] = 0;
	HP898FSync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0xFFFF, HP898FWrite);
}

static void HP898FReset(void) {
	regs[0] = regs[1] = 0;
	HP898FSync();
}

static void HP898FStateRestore(int version) {
	HP898FSync();
}

void BMCHP898F_Init(CartInfo *info) {
	info->Reset = HP898FReset;
	info->Power = HP898FPower;
	GameStateRestore = HP898FStateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
