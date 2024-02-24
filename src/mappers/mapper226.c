/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2009 qeed
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

/* Updated 2019-07-12
 * Mapper 226 - Updated and combine UNIF Ghostbusters63in1 board (1.5 MB carts), different bank order
 * - some 1MB carts can switch game lists using Select
 */

#include "mapinc.h"

static uint8 reg[2], dipsw;
static SFORMAT StateRegs[] = {
	{ &dipsw, 1, "RST" },
	{ reg, 2, "LATC" },
	{ 0 }
};

static void Sync(void) {
	uint8 base = (reg[0] >> 7) | ((reg[1] & 0x01) << 1);
	uint8 prg  = reg[0] & 0x1F;

	/* 1536KiB PRG roms have different bank order */
	if (((ROM.prg.size * 16) == 1536) && base > 0) {
		base--;
	}

	if (reg[0] & 0x20) {
		setprg16(0x8000, (base << 5) | prg);
		setprg16(0xC000, (base << 5) | prg);
	} else {
		setprg32(0x8000, ((base << 5) | prg) >> 1);
	}

	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], !(reg[1] & 0x02));
	setchr8(0);
	setmirror((reg[0] >> 6) & 0x01);
}

static DECLFW(M226Write) {
	reg[A & 0x01] = V;
	Sync();
}

static void M226Power(void) {
	reg[0] = reg[1] = 0;
	Sync();
	SetWriteHandler(0x8000, 0xFFFF, M226Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void StateRestore(int version) {
	Sync();
}

static void M226Reset(void) {
	reg[0] = reg[1] = 0;
	Sync();
}

void Mapper226_Init(CartInfo *info) {
	info->Power = M226Power;
	info->Reset = M226Reset;
	AddExState(StateRegs, ~0, 0, NULL);
	GameStateRestore = StateRestore;
}
