/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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
 *
 * TXC/Micro Genius simplified mapper
 * updated 06-2019 http://wiki.nesdev.com/w/index.php/INES_Mapper_036
 *
 * Known games:
 * - Strike Wolf (Asia) (Unl)
 * - Policeman (Gluk Video) (unl)
 * - F-15 City War (Spain) (Gluk Video) (Unl)
 */

#include "mapinc.h"

static uint8 regs[5];

static SFORMAT StateRegs[] =
{
	{ regs, 5, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, regs[0] >> 4);
	setchr8(regs[4] & 0x0F);
}

static DECLFW(M36Write4100) {
	switch(A & 0xE103) {
	case 0x4100:
		if(regs[3] & 0x10)
			regs[0]++;
		else
			regs[0] = regs[2];
		break;
	case 0x4101:
	case 0x4102:
	case 0x4103:
		regs[A & 0x03] = V;
		break;
	}
}

static DECLFW(M36Write4200) {
	regs[4] = V;
	Sync();
}

static DECLFW(M36WriteHi) {
	Sync();
}

static DECLFR(M36Read) {
	return ((X.DB & 0xCF) | (regs[0] & 0x30));
}

static void M36Power(void) {
	Sync();
	SetReadHandler(0x4100, 0x4100, M36Read);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x4103, M36Write4100);
	SetWriteHandler(0x4200, 0x4200, M36Write4200);
	SetWriteHandler(0x8000, 0xFFFF, M36WriteHi);
}

static void M36Restore(int version) {
	Sync();
}

void Mapper36_Init(CartInfo *info) {
	info->Power = M36Power;
	GameStateRestore = M36Restore;
	AddExState(StateRegs, ~0, 0, 0);
}
