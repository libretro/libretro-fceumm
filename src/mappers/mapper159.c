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
#include "eeprom_x24c0x.h"
#include "bandai.h"

static void M159PW(uint16 A, uint16 V) {
    setprg16(A, V & 0x1F); /* map upto 512K PRG for fan translations etc */
}

static void M159CW(uint16 A, uint16 V) {
    setchr1(A, V);
}

void Mapper159_Init(CartInfo *info) {
    BANDAI_Init(info, EEPROM_X24C01, FALSE);
    BANDAI_pwrap = M159PW;
    BANDAI_cwrap = M159CW;
}
