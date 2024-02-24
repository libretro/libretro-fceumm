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
 *
 * Konami VRC-1
 *
 */

#include "mapinc.h"

static uint8 prg[3], chr[2], mode;

static SFORMAT StateRegs[] = {
	{ &mode, 1, "MODE" },
	{ chr, 2, "CREG" },
	{ prg, 3, "PREG" },
	{ 0 }
};

static void Sync(void) {
	setprg8(0x8000, prg[0]);
	setprg8(0xA000, prg[1]);
	setprg8(0xC000, prg[2]);
	setprg8(0xE000, ~0);

	setchr4(0x0000, (chr[0] & 0x0F) | ((mode & 2) << 3));
	setchr4(0x1000, (chr[1] & 0x0F) | ((mode & 4) << 2));

	if (iNESCart.mirror != MI_4) { /* VS rom conversion uses 4-screen mirroring */
		setmirror((mode & 1) ^ 1);
	}
}

static DECLFW(M75Write) {
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
	case 0xC000:
		prg[(A >> 13) & 0x03] = V;
		Sync();
		break;
	case 0x9000:
		mode = V;
		Sync();
		break;
	case 0xE000:
	case 0xF000:
		chr[(A >> 12) & 0x01] = V;
		Sync();
		break;
	}
}

static void M75Power(void) {
	Sync();
	SetWriteHandler(0x8000, 0xFFFF, M75Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper075_Init(CartInfo *info) {
	info->Power = M75Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}

void Mapper151_Init(CartInfo *info) {
	Mapper075_Init(info);
	info->mirror = MI_4;
	setmirror(MI_4);
}

