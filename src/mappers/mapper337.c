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
 */

/* NES 2.0 Mapper 337 - BMC-CTC-12IN1
 * 12-in-1 Game Card multicart
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_337
 */

#include "mapinc.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 bank = reg & 0x1F;

	setprg8(0x6000, 1);
	if (reg & 0x80) { /* UNROM */
		setprg16(0x8000, bank);
		setprg16(0xC000, bank | 0x07);
	} else {
		if (reg & 0x40) { /* NROM-256 */
			setprg32(0x8000, bank >> 1);
		} else { /* NROM-128 */
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	}
	setchr8(0);
	setmirror(((reg >> 5) & 0x01) ^ 0x01);
}

static DECLFW(M337Write) {
	if (A < 0xC000) {
		reg = (reg & 0x07) | (V & ~0x07);
	} else {
		reg = (reg & ~0x07) | (V & 0x07);
	}
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

static void M337Power(void) {
	reg = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M337Write);
}

void Mapper337_Init(CartInfo *info) {
	info->Power = M337Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
