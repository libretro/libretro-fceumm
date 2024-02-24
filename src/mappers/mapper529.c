/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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

/* NES 2.0 Mapper 529 - YY0807/J-2148/T-230
 * UNIF UNL-T-230
 */

#include "mapinc.h"
#include "vrc2and4.h"
#include "eeprom_93Cx6.h"

static uint8 haveEEPROM;
static uint8 eeprom_data[256];

static void M529PW(uint16 A, uint16 V) {
	setprg16(0x8000, vrc24.prg[1]);
	setprg16(0xC000, ~0);
}

static DECLFR(M529EEPROMRead) {
	if (haveEEPROM) {
		return eeprom_93Cx6_read() ? 0x01 : 0x00;
	}
	return 0x01;
}

static DECLFW(M529Write) {
	if (A & 0x800) {
		if (haveEEPROM) {
			eeprom_93Cx6_write(!!(A & 0x04), !!(A & 0x02), !!(A & 0x01));
		}
	} else {
		VRC24_Write(A, V);
	}
}

static void M529Power(void) {
	VRC24_Power();
	SetReadHandler(0x5000, 0x5FFF, M529EEPROMRead);
	SetWriteHandler(0x8000, 0xFFFF, M529Write);
}

void Mapper529_Init(CartInfo *info) {
	haveEEPROM = (info->PRGRamSaveSize & 0x100) != 0;
	VRC24_Init(info, VRC4, 0x04, 0x08, !haveEEPROM, 1);
	info->Power = M529Power;
	VRC24_pwrap = M529PW;
	if (haveEEPROM) {
		eeprom_93Cx6_init(eeprom_data, 256, 16);
		info->battery = 1;
		info->SaveGame[0] = eeprom_data;
		info->SaveGameLen[0] = 256;
	}
}
