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

/* NES 2.0 Mapper 274 is used for the 90-in-1 Hwang Shinwei multicart.
 * Its UNIF board name is BMC-80013-B.
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_274 */

 /* 2020-03-22 - Update support for Cave Story II, FIXME: Arabian does not work for some reasons... */

#include "mapinc.h"

static uint8 reg[2], extraChip;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &extraChip, 1, "CHIP" },
	{ 0 }
};

static void Sync(void) {
	if (extraChip) {
		setprg16(0x8000, 0x80 | (reg[0] & ((ROM.prg.size - 1) & 0x0F)));
	} else {
		setprg16(0x8000, (reg[1] & 0x70) | (reg[0] & 0x0F));
	}
	setprg16(0xC000, reg[1] & 0x7F);
	setchr8(0);
	setmirror(((reg[0] >> 4) & 0x01) ^ 0x01);
}

static DECLFW(M274Write8) {
	reg[0] = V;
	Sync();
}

static DECLFW(M274WriteA) {
	reg[1] = V;
	extraChip = (A & 0x4000) == 0;
	Sync();
}

static void M274Power(void) {
	reg[0] = reg[1] = 0;
	extraChip = 1;
	Sync();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x9FFF, M274Write8);
	SetWriteHandler(0xA000, 0xFFFF, M274WriteA);
}

static void M274Reset(void) {
	reg[0] = reg[1] = 0;
	extraChip = 1;
	Sync();
}

static void M274Restore(int version) {
	Sync();
}

void Mapper274_Init(CartInfo *info) {
	info->Power = M274Power;
	info->Reset = M274Reset;
	GameStateRestore = M274Restore;
	AddExState(StateRegs, ~0, 0, NULL);
}
