/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright (C) 2019 Libretro Team
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

/* added 2019-5-23
 * UNIF: BMC-60311C:
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_289
 */

#include "mapinc.h"

static uint8 inner_bank, outer_bank, mode;

static SFORMAT StateRegs[] =
{
	{ &inner_bank, 1, "INB0" },
	{ &outer_bank, 1, "OUTB" },
	{ &mode, 1, "MODE" },
	{ 0 }
};

static void Sync(void) {
	uint8 bbank = (mode & 4) ? 0 : (inner_bank & 7);
	uint8 bank = outer_bank | bbank;
	uint8 preg[2];

	/* 0: NROM-128: Same inner/outer 16 KiB bank at CPU $8000-$BFFF
	 *    and $C000-$FFFF
	 * 1: NROM-256: 32 kiB bank at CPU $8000-$FFFF (Selected inner/outer bank SHR 1)
	 * 2: UNROM: Inner/outer bank at CPU $8000-BFFF,
	 *    fixed inner bank 7 within outer bank at $C000-$FFFF
	 * 3: Unknown
	 *
	 * The combined inner/outer bank is simply the inner bank, selected by the
	 * latch at $8000-$FFFF (or 0 if the latch is disabled) ORed with the
	 * outer bank selected by $6001, without any bit shifting.
	 */

	preg[0] = bank;
	preg[1] = 0;

	switch (mode & 3) {
	case 0x00:
	case 0x01:
		preg[1] = bank | ((mode & 1) ? 1 : 0);
		break;
	case 0x02:
		preg[1] = outer_bank | 7;
	case 0x03:
		break;
	}

	setchr8(0);
	setprg16(0x8000, preg[0]);
	setprg16(0xC000, preg[1]);
	setmirror(((mode & 8) >> 3) ^ 1);
}

static DECLFW(Write0) {
	mode = V;
	Sync();
}

static DECLFW(Write1) {
	outer_bank = V;
	Sync();
}

static DECLFW(Write8) {
	inner_bank = V;
	Sync();
}

static void BMC60311CPower(void) {
	inner_bank = outer_bank = mode = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x6000, Write0);
	SetWriteHandler(0x6001, 0x6001, Write1);
	SetWriteHandler(0x8000, 0xFFFF, Write8);
}

static void BMC60311CReset(void) {
	inner_bank = outer_bank = mode = 0;
	Sync();
}

static void BMC60311CRestore(int version) {
	Sync();
}

void BMC60311C_Init(CartInfo *info) {
	info->Power = BMC60311CPower;
	info->Reset = BMC60311CReset;
	GameStateRestore = BMC60311CRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
