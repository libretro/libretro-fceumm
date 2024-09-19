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

static void M016PW(uint16 A, uint16 V) {
    setprg16(A, V & 0x0F);
}

static void M016CW(uint16 A, uint16 V) {
    setchr1(A, V);
}

void Mapper016_Init(CartInfo *info) {
    switch (info->submapper) {
    case 4:
        BANDAI_Init(info, EEPROM_NONE, TRUE);
        break;
    case 5:
        if (info->battery || ((info->PRGRamSaveSize > 0) && (info->PRGRamSaveSize <= 256))) {
           BANDAI_Init(info, EEPROM_X24C02, FALSE); break;
        } else {
            BANDAI_Init(info, EEPROM_NONE, FALSE); break;
        }
        break;
    default:
        BANDAI_Init(info, EEPROM_NONE, TRUE);
        break;
    }
    BANDAI_pwrap = M016PW;
    BANDAI_cwrap = M016CW;
}
