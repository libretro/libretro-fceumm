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
#include "mmc2.h"

static void M009PW(uint16 A, uint16 V) {
    setprg8(A, V & 0x0F);
}

static void M009CW(uint16 A, uint16 V) {
    setchr4(A, V & 0x1F);
}

void Mapper009_Init(CartInfo *info) {
	int ws = info->iNES2 ? ((info->PRGRamSize + info->PRGRamSaveSize) / 1024) : (info->battery ? 8 : 0);
	MMC2_Init(info, ws, info->battery);
    MMC2_pwrap = M009PW;
    MMC2_cwrap = M009CW;
}
