/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 * NES 2.0 Mapper 530 - UNL-AX5705
 * Super Bros. Pocker Mali (VRC4 mapper)
 */

#include "mapinc.h"
#include "vrc2and4.h"

static DECLFW(UNLAX5705Write) {
	A |= (A & 0x0008) ? 0x1000 : 0x0000;
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		V = ((V & 0x02) << 2) | ((V & 0x08) >> 2) | (V & 0x05);
		break;
	case 0xB000:
	case 0xC000:
	case 0xD000:
	case 0xE000:
		if (A & 0x0001) {
			V = ((V & 0x04) >> 1) | ((V & 0x02) << 1) | (V & 0x09);
		}
		break;
	}
	VRC24_Write(A, V);
}

static void M530Power(void) {
	VRC24_Power();
	SetWriteHandler(0x8000, 0xFFFF, UNLAX5705Write);
}

void Mapper530_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x01, 0x02, 0, 1);
	info->Power = M530Power;
}
