/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 CaH4e3
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

#include "mapinc.h"

#define CARD_EXTERNAL_INSERED 0x80

static uint8 prg_reg;
static uint8 chr_reg;
static SFORMAT StateRegs[] =
{
	{ &prg_reg, 1, "PREG" },
	{ &chr_reg, 1, "CREG" },
	{ 0 }
};

static void Sync(void) {
	setprg32(0x8000, prg_reg);
	setchr8(chr_reg);
}

static void StateRestore(int version) {
	Sync();
}

static DECLFW(M216WriteHi) {
	prg_reg = A & 1;
	chr_reg = (A & 0x0E) >> 1;
	Sync();
}

static DECLFW(M216Write5000) { }

static DECLFR(M216Read5000) {
	return 0;
}

static void Power(void) {
	prg_reg = 0;
	chr_reg = 0;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M216WriteHi);
	SetWriteHandler(0x5000, 0x5000, M216Write5000);
	SetReadHandler(0x5000, 0x5000, M216Read5000);
}


void Mapper216_Init(CartInfo *info) {
	info->Power = Power;
	GameStateRestore = StateRestore;
	AddExState(&StateRegs, ~0, 0, 0);
}
