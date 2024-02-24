/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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
 * Mapper 55 - UNL-MARIO1-M0552
 * FDS Conversion
 *
 */

#include "mapinc.h"

static void M055Power(void) {
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetReadHandler(0x6000, 0x67FF, CartBR);
	SetReadHandler(0x7000, 0x77FF, CartBR);
	SetWriteHandler(0x7000, 0x77FF, CartBW);

	setprg2(0x6000, 16);
	setprg2r(0x10, 0x7000, 0);
	setprg32(0x8000, 0);
	setchr8(0);
}

void Mapper055_Init(CartInfo *info) {
	info->Power = M055Power;
	if (info->iNES2) {
		WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	} else if (info->battery) {
		WRAMSIZE = 2048;
	}
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}
}
