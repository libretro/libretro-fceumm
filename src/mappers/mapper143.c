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

/* iNES Mapper 143 - TC-A01 */

#include "mapinc.h"

static DECLFR(M143Read) {
	if ((A & 0x100) == 0x100) {
		return (cpu.openbus & 0xC0) | ((~A) & 0x3F);
    }
	return cpu.openbus;
}

static void M143Power(void) {
	setprg32(0x8000, 0);
	setchr8(0);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x4100, 0x5FFF, M143Read);
}

void Mapper143_Init(CartInfo *info) {
	info->Power = M143Power;
}
