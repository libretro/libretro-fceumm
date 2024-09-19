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

/* NES 2.0 Mapper 360 - Bit Corp's 31-in-1 multicart (3150) */

#include "mapinc.h"

static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void Sync(void) {
	/* dip 0 and 1 is the same game SMB) */
	if (dipsw < 2) {
		setprg32(0x8000, dipsw >> 1);
	} else {
		setprg16(0x8000, dipsw);
		setprg16(0xC000, dipsw);
	}
	setchr8(dipsw);
	setmirror(((dipsw & 0x10) >> 4) ^ 1);
}

static void M360Power(void) {
	dipsw = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0XFFFF, CartBW);
}

static void M360Reset(void) {
	dipsw = (dipsw + 1) & 31;
	Sync();
	FCEU_printf("dipsw = %d\n", dipsw);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper360_Init(CartInfo *info) {
	info->Reset = M360Reset;
	info->Power = M360Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
