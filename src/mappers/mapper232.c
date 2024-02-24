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

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 base = (reg[0] >> 1) & 0x0C;

	if (iNESCart.submapper == 1) {
		base =  ((base << 1) & 0x08) | ((base >> 1) & 0x04);
	}
	setprg16(0x8000, base | (reg[1] & 0x03));
	setprg16(0xC000, base | 0x03);
	setchr8(0);
}

static DECLFW(M232Write) {
	reg[(A >> 14) & 0x01] = V;
	Sync();
}

static void M232Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M232Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper232_Init(CartInfo *info) {
	info->Power = M232Power;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, NULL);
}
