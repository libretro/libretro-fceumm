/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* Mapper 053 - BMC-Supervision16in1 */

#include "mapinc.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x6000, 0x04 + (((reg[0] & 0x0F) << 4) | 0x0F));
	if (reg[0] & 0x10) {
		setprg16(0x8000, 0x02 + (((reg[0] & 0x0F) << 3) | (reg[1] & 0x07)));
		setprg16(0xc000, 0x02 + (((reg[0] & 0x0F) << 3) | 0x07));
	} else {
		setprg32(0x8000, 0);
	}
	setchr8(0);
	setmirror(((reg[0] & 0x20) >> 5) ^ 0x01);
}

static DECLFW(M053Write6) {
	if (!(reg[0] & 0x10)) {
		reg[0] = V;
		Sync();
	}
}

static DECLFW(M053Write8) {
	reg[1] = V;
	Sync();
}

static void M053Power(void) {
	SetWriteHandler(0x6000, 0x7FFF, M053Write6);
	SetWriteHandler(0x8000, 0xFFFF, M053Write8);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	reg[0] = reg[1] = 0;
	Sync();
}

static void M053Reset(void) {
	reg[0] = reg[1] = 0;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper053_Init(CartInfo *info) {
	info->Power = M053Power;
	info->Reset = M053Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
