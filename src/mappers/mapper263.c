/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

/* NES 2.0 Mapper 263 - UNL-KOF97 */

#include "mapinc.h"
#include "mmc3.h"

static uint32 unscrambleAddr(uint32 A) {
	return ((A & 0xE000) | ((A >> 12) & 0x01));
}

static uint8 unscrambleData(uint8 V) {
	return ((V & 0xD8) | ((V & 0x20) >> 4) | ((V & 0x04) << 3) | ((V & 0x02) >> 1) | ((V & 0x01) << 2));
}

static DECLFW(M263Write) {
	MMC3_Write(unscrambleAddr(A), unscrambleData(V));
}

static void M263Power(void) {
	MMC3_Power();
	SetWriteHandler(0x8000, 0xFFFF, M263Write);
}

void Mapper263_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	info->Power = M263Power;
}
