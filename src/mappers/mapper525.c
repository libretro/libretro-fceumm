/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* NES Mapper 525 - UNL-M525
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_525
 * NES 2.0 Mapper 525 is used for a bootleg version of versions of Contra and 月風魔伝 (Getsu Fūma Den).
 * Its similar to Mapper 23 Submapper 3) with non-nibblized CHR-ROM bank registers.
 */

#include "mapinc.h"

static uint8 prg;
static uint8 chr[8];
static uint8 mirr;

static SFORMAT StateRegs[] = {
	{ chr, 8, "CHRR" },
	{ &prg, 1, "PRGR" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg16(0x8000, prg >> 1);
	setprg16(0xC000, ~0);

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirror((mirr & 0x01) ^ 0x01);
}

static DECLFW(M525WritePRG) {
	prg = V;
	Sync();
}

static DECLFW(M525WriteMIR) {
	prg = V;
	Sync();
}

static DECLFW(M525WriteCHR) {
	chr[A & 0x07] = V;
	Sync();
}

static void M525Power(void) {
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x8FFF, M525WritePRG);
	SetWriteHandler(0x9000, 0x9FFF, M525WriteMIR);
	SetWriteHandler(0xB000, 0xBFFF, M525WriteCHR);
}

static void StateRestore(int version) {
	Sync();
}

void Mapper525_Init(CartInfo *info) {
	info->Power = M525Power;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
