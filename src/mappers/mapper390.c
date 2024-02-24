/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 390 - Realtec 8031 */
/* NOTE: Duplicate of Mapper 236 (CHR-ROM variant */

#include "mapinc.h"

static uint8 reg[2];
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	switch (reg[1] & 0x30) {
	case 0x00:
	case 0x10: /* UNROM */
		setprg16(0x8000, reg[1]);
		setprg16(0xC000, reg[1] | 0x07);
		break;
	case 0x20: /* Maybe unused, NROM-256? */
		setprg32(0x8000, reg[1] >> 1);
		break;
	case 0x30: /* NROM-128 */
		setprg16(0x8000, reg[1]);
		setprg16(0xC000, reg[1]);
		break;
	}
	setchr8(reg[0]);
	setmirror(((reg[0] & 0x20) >> 5) ^ 1);
}

static DECLFR(M390Read) {
	uint8 ret = CartBR(A);
	if ((reg[1] & 0x30) == 0x10)
		ret |= dipsw;
	return ret;
}

static DECLFW(M390Write) {
	reg[(A >> 14) & 0x01] = A & 0x3F;
	Sync();
}

static void M390Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	dipsw = 11; /* hard-coded 150-in-1 menu */
	Sync();
	SetReadHandler(0x8000, 0xFFFF, M390Read);
	SetWriteHandler(0x8000, 0xFFFF, M390Write);
}

static void M390Reset(void) {
	dipsw = 11; /* hard-coded 150-in-1 menu */
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper390_Init(CartInfo *info) {
	info->Reset = M390Reset;
	info->Power = M390Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
