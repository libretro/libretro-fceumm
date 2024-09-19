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
#include "mmc1.h"

static void M155PW(uint16 A, uint16 V) {
	setprg16(A, (MMC1_GetCHRBank(0) & 0x10) | (V & 0x0F));
}

/* Same as mapper 1, without respect for WRAM enable bit. */
void Mapper155_Init(CartInfo *info) {
	MMC1_Init(info, 8, info->battery ? 8 : 0);
    MMC1_pwrap = M155PW;
	mmc1_type = MMC1A;
}
