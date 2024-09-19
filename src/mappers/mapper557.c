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

/* NES 2.0 Mapper 557 is used for Kaiser's cartridge conversion of the Famicom Disk System game Moero TwinBee: Cinnamon-hakase o Sukue!. */

#include "mapinc.h"
#include "n118.h"

static void M557FixPRG(void) {
    setprg8r(0x10, 0x6000, 0);
    setprg8(0x8000, n118.reg[6] & 0x0F);
    setprg8(0xA000, n118.reg[7] & 0x0F);
    setprg8(0xC000, ~1);
    setprg8(0xE000, ~0);
}

static void M557FixCHR(void) {
	setchr8(0);
    setmirror(((n118.reg[5] >> 5) & 0x01) ^ 0x01);
}

void Mapper557_Init(CartInfo *info) {
	N118_Init(info, 8, 0);
    N118_FixPRG = M557FixPRG;
	N118_FixCHR = M557FixCHR;
}
