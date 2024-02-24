/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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
 */

#include "mapinc.h"

static uint8 reg[3];

static SFORMAT StateRegs[] = {
	{ reg, 3, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if (reg[0] & 0x40) {
		setprg32(0x8000, (reg[0] & 0x0E) | (reg[1] & 0x01));
		setchr8(((reg[0] & 0x0E) << 2) | ((reg[1] >> 4) & 0x07));
	} else {
		setprg32(0x8000, reg[0] & 0x0F);
		setchr8(((reg[0] & 0x0F) << 2) | ((reg[1] >> 4) & 0x03));
	}
	setmirror((reg[0] >> 7) ^ 0x01);
}

static DECLFR(M234Read) {
	uint8 ret = CartBR(A);

	switch (A & 0xFFF8) {
	case 0xFF80:
	case 0xFF88:
	case 0xFF90:
	case 0xFF98:
		if (!reg[0]) {
			reg[0] = ret;
			Sync();
		}
		break;
	case 0xFFC0:
	case 0xFFC8:
	case 0xFFD0:
	case 0xFFD8:
		if (!reg[0]) {
			reg[2] = ret;
			Sync();
		}
		break;
	case 0xFFE8:
	case 0xFFF0:
		reg[1] = ret;
		Sync();
		break;
	}

	return ret;
}

static void M234Reset(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
}

static void M234Power(void) {
	reg[0] = reg[1] = reg[2] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0xFF80, 0xFFFF, M234Read);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper234_Init(CartInfo *info) {
	info->Power = M234Power;
	info->Reset = M234Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
