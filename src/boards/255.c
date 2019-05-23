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
 * Mapper 255
 * https://wiki.nesdev.com/w/index.php/INES_Mapper_225
 * 115-in-1 [p1][!] CRC32 0xb39d30b4
 * Seems to handle up to last game in the multicarts than m225 but causes
 * graphics garbage past it, else games works fine */

#include "mapinc.h"

static uint8 preg, creg, mode, mirr;

static SFORMAT StateRegs[] =
{
	{ &preg, 1, "PRG0" },
	{ &creg, 1, "CHR0" },
	{ &mode, 1, "MODE" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, preg & ~mode);
	setprg16(0xC000, preg | mode);
	setchr8(creg);
	setmirror(mirr ^ 1);
}

static DECLFW(M255Write) {
	uint32 bank = (A >> 8 & 0x40);
	preg = bank | ((A >> 6) & 0x3F);
	creg = bank | (A & 0x3F);
	mirr = (A >> 13) & 1;
	mode = ((~A) >> 12 & 1);
	Sync();
}

static void M255Power(void) {
	preg = 0;
	mode = 1;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M255Write);
}

static void M255Reset(void) {
	preg = 0;
	mode = 1;
	Sync();
}

static void StateRestore(int version) {
	Sync();
}

void Mapper255_Init(CartInfo *info) {
	info->Reset = M255Reset;
	info->Power = M255Power;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
