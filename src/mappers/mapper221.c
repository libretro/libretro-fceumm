/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR reg[0] PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Mapper 221 - UNL-N625092
 * 700in1 and 400in1 carts
 * 1000-in-1
 */

#include "mapinc.h"

static uint16 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint16 prg = ((reg[0] >> 3) & 0x40) | ((reg[0] >> 2) & 0x38) | (reg[1] & 0x07);

	if (!(reg[0] & 0x02)) {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	} else {
		if (reg[0] & 0x100) {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg | 0x07);
		} else {
			setprg16(0x8000, prg & ~1);
			setprg16(0xC000, prg |  1);
		}
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], !(reg[1] & 0x08));
	setchr8(0);
	setmirror((reg[0] & 0x01) ^ 0x01);
}

static DECLFW(M221Write) {
	reg[(A >> 14) & 0x01] = A;
	Sync();
}

static void M221Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M221Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper221_Init(CartInfo *info) {
	info->Power = M221Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
