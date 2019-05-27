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
 * BMC-WS Used for  Super 40-in-1 multicart
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_332 */

#include "mapinc.h"

static uint8 preg, creg;

static SFORMAT StateRegs[] =
{
	{ &preg, 1, "PREG" },
	{ &creg, 1, "CREG" },
	{ 0 }
};

static void Sync(void) {
	if (preg & 8) {
		setprg16(0x8000, preg & 7);
		setprg16(0xc000, preg & 7);
	}
	else
		setprg32(0x8000, (preg & 6) >> 1);
	setchr8(creg);
	setmirror(((preg >> 4) & 1) ^ 1);
}

static DECLFW(BMCWSWrite) {
	if (preg & 0x20)
		return;

	switch (A & 1) {
	case 0: preg = V; Sync(); break;
	case 1: creg = V; Sync(); break;
	}
}

static void MBMCWSPower(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x6001, BMCWSWrite);
}

static void BMCWSReset(void) {
}

static void StateRestore(int version) {
	Sync();
}

void BMCWS_Init(CartInfo *info) {
	info->Reset = BMCWSReset;
	info->Power = MBMCWSPower;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
