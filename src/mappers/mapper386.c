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
#include "jyasic.h"

static uint32 GetPRGBank(uint32 V) {
	return (((jyasic.mode[3] << 3) & 0x40) | ((jyasic.mode[3] << 4) & 0x20) | (V & 0x1F));
}

static uint32 GetCHRBank(uint32 V) {
    if (jyasic.mode[3] & 0x20) {
        return (((jyasic.mode[3] << 7) & 0x600) | (V & 0x1FF));    
    } else {
        return (((jyasic.mode[3] << 7) & 0x600) | ((jyasic.mode[3] << 8) & 0x100) | (V & 0x0FF));
    }
}

void Mapper386_Init(CartInfo *info) {
	/* Multicart */
	JYASIC_Init(info, TRUE);
    JYASIC_GetPRGBank = GetPRGBank;
	JYASIC_GetCHRBank = GetCHRBank;
}
