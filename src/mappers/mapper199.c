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

#include "mapinc.h"
#include "mmc3.h"

static void M199CW(uint16 A, uint16 V) {
	setchr8(0);
}

static void M199Power(void) {
	MMC3_Power();
	setprg4r(0x10, 0x5000, 2);
	SetReadHandler(0x5000, 0x5FFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, CartBW);
}

void Mapper199_Init(CartInfo *info) {
	MMC3_Init(info, 16, info->battery);
	MMC3_cwrap = M199CW;
	info->Power = M199Power;
	info->Reset = MMC3_Reset;
}
