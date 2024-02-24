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
 */

#include "mapinc.h"
#include "mmc4.h"

static void M010PW(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void M010CW(uint16 A, uint16 V) {
	setchr4(A, V & 0x1F);
}

void Mapper010_Init(CartInfo *info) {
	int ws = info->iNES2 ? ((info->PRGRamSize + info->PRGRamSaveSize) / 1024) : (info->battery ? 8 : 0);
	MMC4_Init(info, ws, info->battery);
	MMC4_pwrap = M010PW;
	MMC4_cwrap = M010CW;
}