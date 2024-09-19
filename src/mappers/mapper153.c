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

static uint8 outer;

/* Famicom jump 2:
 * 0-7: Lower bit of data selects which 256KB PRG block is in use.
 * This seems to be a hack on the developers' part, so I'll make emulation
 * of it a hack(I think the current PRG block would depend on whatever the
 * lowest bit of the CHR bank switching register that corresponds to the
 * last CHR address read).
 */

static SFORMAT StateRegs[] = {
    { &outer, 1, "OUTB" },
    { 0 }
};

static void M153PW(uint16 A, uint16 V) {
    setprg16(A, ((outer << 4) & 0x10) | (V & 0x0F));
}

static void M153CW(uint16 A, uint16 V) {
    setchr8(0);
}

static DECLFW(M153Write) {
    if ((A & 0x0F) <= 0x03) {
        outer = V;
        BANDAI_FixPRG();
    }
    BANDAI_Write(A, V);
}

static void M153Power(void) {
	BANDAI_Power();
    SetWriteHandler(0x8000, 0xFFFF, M153Write);
    if (WRAMSIZE) {
	    setprg8r(0x10, 0x6000, 0);
	    SetReadHandler(0x6000, 0x7FFF, CartBR);
	    SetWriteHandler(0x6000, 0x7FFF, CartBW);
	    FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
    }
}

static void M153Close(void) {
}

void Mapper153_Init(CartInfo *info) {
    BANDAI_Init(info, EEPROM_NONE, FALSE);
	info->Power = M153Power;
	info->Close = M153Close;
    BANDAI_pwrap = M153PW;
    BANDAI_cwrap = M153CW;
    AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = 8192;
    if (info->iNES2) {
        WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
    }
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}
}
