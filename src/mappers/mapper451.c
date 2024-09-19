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

/* NES 2.0 Mapper 451 */
/* Uses flashrom to save high scores. */

#include "mapinc.h"
#include "mmc3.h"
#include "flashrom.h"

static uint8 reg;

static uint8 *FLASHROM = NULL;
static uint32 FLASHROM_size = 0;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M451FixPRG(void) {
	setprg8r(0x10, 0x8000, 0);
	setprg8r(0x10, 0xA000, 0x10 | ((reg << 2) & 0x08) | (reg & 0x01));
	setprg8r(0x10, 0xC000, 0x20 | ((reg << 2) & 0x08) | (reg & 0x01));
	setprg8r(0x10, 0xE000, 0x30);
}

static void M451FixCHR(void) {
	setchr8(reg & 0x01);
}

static DECLFR(M451Read) {
	return FlashROM_Read(A);
}

static DECLFW(M451Write) {
	FlashROM_Write(A, V);
	switch (A & 0xE000) {
    case 0xA000:
        MMC3_CMDWrite(0xA000, A & 0x01);
        break;
    case 0xC000:
        A &= 0xFF;
	    MMC3_IRQWrite(0xC000, A - 1);
	    MMC3_IRQWrite(0xC001, 0);
	    MMC3_IRQWrite(0xE000 + ((A == 0xFF) ? 0x00 : 0x01), 0x00);
        break;
    case 0xE000:
		reg = A & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
        break;
	}
}

static void M451Power(void) {
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M451Read);
	SetWriteHandler(0x8000, 0xFFFF, M451Write);
}

static void M451Close(void) {
    MMC3_Close();
	if (FLASHROM) {
		FCEU_free(FLASHROM);
	}
	FLASHROM = NULL;
}

void Mapper451_Init(CartInfo *info) {
	uint32 w, r;

	MMC3_Init(info, 0, 0);
	info->Power = M451Power;
	info->Close = M451Close;
    MMC3_FixPRG = M451FixPRG;
	MMC3_FixCHR = M451FixCHR;
	MapIRQHook = FlashROM_CPUCyle;
	AddExState(StateRegs, ~0, 0, NULL);

	info->battery = 1;
	FLASHROM_size = PRGsize[0];
	FLASHROM = (uint8 *)FCEU_gmalloc(FLASHROM_size);
	info->SaveGame[0]    = FLASHROM;
	info->SaveGameLen[0] = FLASHROM_size;
	AddExState(FLASHROM, FLASHROM_size, 0, "FLAS");
	/* copy PRG ROM into FLASHROM, use it instead of PRG ROM */
	for (w = 0, r = 0; w < FLASHROM_size; w++) {
		FLASHROM[w] = PRGptr[0][r];
		++r;
	}
	SetupCartPRGMapping(0x10, FLASHROM, FLASHROM_size, 0);
	FlashROM_Init(FLASHROM, FLASHROM_size, 0x37, 0x86, 65536, 0x0555, 0x02AA);
}
