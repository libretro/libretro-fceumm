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

/* NES 2.0 Mapper 350 - BMC-891227
 * Super 15-in-1 Game Card
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_350
 */

#include "mapinc.h"
#include "latch.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void Sync(void) {
	uint8 bank = (reg[0] & 0x18) | (reg[1] & 0x07);

	setprg8(0x6000, 1);
	if (reg[0] & 0x40) { /* UNROM */
		if (reg[0] & 0x20) {
			/* 2nd chip only has 128K PRG */
			bank &= 0x07;
		}
		setprg16(0x8000, (reg[0] & 0x20) | bank);
		setprg16(0xC000, (reg[0] & 0x20) | bank | 0x07);
	} else {
		if (reg[0] & 0x20) { /* NROM-256 */
			setprg32(0x8000, bank >> 1);
		} else {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	}
	/* CHR-RAM Protect... kinda */
	SetupCartCHRMapping(0, CHRptr[0], 0x2000, (reg[0] & 0x40) != 0);
	setchr8(0);
	setmirror(((reg[0] >> 7) & 0x01) ^ 0x01);
}

static DECLFW(M350Write) {
	reg[(A >> 14) & 0x01] = V;
	Sync();
}

static void M350Reset(void) {
	reg[0] = reg[1] = 0;
	Sync();
}

static void M350Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M350Write);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper350_Init(CartInfo *info) {
	info->Power = M350Power;
	info->Reset = M350Reset;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
