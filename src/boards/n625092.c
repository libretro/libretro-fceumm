/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
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
 * 700in1 and 400in1 carts
 * 1000-in-1
 */

#include "mapinc.h"

static uint16 reg[2];
static uint8 submapper;

static SFORMAT StateRegs[] ={
	{ reg, 4 | FCEUSTATE_RLSB, "REGS" },
	{ 0 }
};

static DECLFR(Mapper221_ReadOB)
{
   return X.DB;
}

static void sync(void) {
	uint8 prg =reg[0] >>(submapper ==1? 2: 3) &0x40 | reg[0] >>2 &0x38 | reg[1] &0x07;
	SetReadHandler(0x8000, 0xFFFF, prg <<14 >=PRGsize[0]? Mapper221_ReadOB: CartBR); /* Selecting unpopulated banks results in open bus */
	if (reg[0] &(submapper ==1? 0x200: 0x100)) { /* UNROM */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg |7);
	} else
	if (reg[0] &0x0002) /* NROM-256 */
		setprg32(0x8000, prg >>1);
	else { /* NROM-128 */
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
	setchr8(0);
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, submapper ==1? !(reg[0] &0x0400): !(reg[1] &0x0008));
	setmirror(reg[0] &0x0001? MI_H: MI_V);
}

static DECLFW(Mapper221Write) {
	reg[A >>14 &1] =A;
	sync();
}

static void UNLN625092Power(void) {
	reg[0] =reg[1] =0;
	sync();	
	SetWriteHandler(0x8000, 0xFFFF, Mapper221Write);
}

static void UNLN625092Reset(void) {
	reg[0] =reg[1] =0;
	sync();
}

static void StateRestore(int version) {
	sync();
}

void UNLN625092_Init(CartInfo *info) {
	submapper =info->submapper;
	info->Reset = UNLN625092Reset;
	info->Power = UNLN625092Power;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
