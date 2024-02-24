/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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
 */

/* NES 2.0 mapper 335 is used for a 10-in-1 multicart.
 * Its UNIF board name is BMC-CTC-09.
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_335 */

#include "mapinc.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	if (reg[1] & 0x10) {
		setprg16(0x8000, ((reg[1] & 0x07) << 1) | ((reg[1] >> 3) & 0x01));
		setprg16(0xC000, ((reg[1] & 0x07) << 1) | ((reg[1] >> 3) & 0x01));
	} else {
		setprg32(0x8000, reg[1] & 0x07);
	}
	setchr8(reg[0] & 0x0F);
	setmirror(((reg[1] >> 5) & 0x01) ^ 0x01);
}

static DECLFW(M335Write) {
	switch (A & 0xE000) {
	case 0x8000:
	case 0xA000:
		reg[0] = V;
		Sync();
		break;
	case 0xC000:
	case 0xE000:
		reg[1] = V;
		Sync();
		break;
	}
}

static void M335Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M335Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper335_Init(CartInfo *info) {
	info->Power = M335Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
