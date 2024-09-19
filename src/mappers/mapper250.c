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

static DECLFW(M250Write) {
    MMC3_Write(((A & 0xE000) | ((A & 0x400) >> 10)), (A & 0xFF));
}

static void M250Power(void) {
	MMC3_Power();
    SetWriteHandler(0x8000, 0xFFFF, M250Write);
}

void Mapper250_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	info->Power = M250Power;
}
