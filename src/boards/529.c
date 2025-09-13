/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
#include "asic_vrc2and4.h"
#include "eeprom_93Cx6.h"

static uint8 eeprom_data[256];

static SFORMAT stateRegs[] ={
        { eeprom_data, 256, "EEPR" },
        { 0 }
};

static void sync () {
	setprg16(0x8000, VRC24_getPRGBank(1));
	setprg16(0xC000, 0xFF);
	VRC24_syncCHR(0x1FF, 0x000);
	VRC24_syncMirror();
}

static DECLFR (readEEPROM) {
	return eeprom_93Cx6_read()? 0x01: 0x00;
}
	
static DECLFW (writeEEPROM) {
	eeprom_93Cx6_write(A &0x04, A &0x02, A &0x01);
}

static void power (void) {
	VRC24_power();
	eeprom_93Cx6_init(256, 16);
	SetReadHandler(0x5000, 0x5FFF, readEEPROM);
	SetWriteHandler(0xF800, 0xFFFF, writeEEPROM);
}

void UNLT230_Init (CartInfo *info) {
	VRC4_init(info, sync, 0x04, 0x08, 1, NULL, NULL, NULL, NULL, NULL);
	if (info->PRGRamSaveSize) {
		info->Power =power;
		AddExState(stateRegs, ~0, 0, 0);
		eeprom_93Cx6_storage = eeprom_data;
		info->battery = 1;
		info->SaveGame[0] = eeprom_data;
		info->SaveGameLen[0] = 256;
	}
}
