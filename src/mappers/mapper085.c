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
 *
 *
 */

#include "mapinc.h"
#include "vrc7.h"

static void M085PW(uint16 A, uint16 V) {
    setprg8(A, V & 0x3F);
}

static void M085CW(uint16 A, uint16 V) {
    setchr1(A, V & 0xFF);
}

void Mapper085_Init(CartInfo *info) {
	switch (info->submapper) {
	case 0x01: VRC7_Init(info, 0x08, 0x20); break;
	case 0x02: VRC7_Init(info, 0x10, 0x20); break;
	default: VRC7_Init(info, 0x18, 0x20); break;
	}
    VRC7_pwrap = M085PW;
    VRC7_cwrap = M085CW;
}
