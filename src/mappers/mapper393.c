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

static uint8 reg[2];

static uint8 *CHRRAM;
static uint32 CHRRAMSIZE;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M393CW(uint16 A, uint16 V) {
	if (reg[0] & 0x08) {
		setchr8r(0x10, 0);
	} else {
		setchr1(A, (reg[0] << 8) | (V & 0xFF));
	}
}

static void M393PW(uint16 A, uint16 V) {
	if (reg[0] & 0x20) {
		if (reg[0] & 0x10) {
			setprg16(0x8000, (reg[0] << 3) | (reg[1] & 0x07));
			setprg16(0xC000, (reg[0] << 3) | 0x07);
		} else {
			setprg32(0x8000, (reg[0] << 2) | ((mmc3.reg[6] >> 2) & 0x03));
		}
	} else {
		setprg8(A, (reg[0] << 4) | (V & 0x0F));
	}
}

static DECLFW(M393Write) {
	reg[1] = V;
	switch (A & 0xE001) {
	case 0x8001:
		mmc3.reg[mmc3.cmd & 0x07] = V;
		if ((mmc3.cmd & 0x07) < 6) {
			MMC3_FixCHR();
		}
		MMC3_FixPRG();
		break;
	default:
		MMC3_Write(A, V);
		MMC3_FixPRG();
		break;
	}
}

static DECLFW(M393WriteReg) {
	if (MMC3_WramIsWritable()) {
		reg[0] = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M393Power(void) {
	reg[0] = reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M393WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M393Write);
}

static void M393Reset(void) {
	reg[0] = reg[1] = 0;
	MMC3_Reset();
}

static void M393lose(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
	}
	CHRRAM = NULL;
}

void Mapper393_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M393PW;
	MMC3_cwrap = M393CW;
	info->Power = M393Power;
	info->Reset = M393Reset;
	info->Close = M393lose;
	AddExState(StateRegs, ~0, 0, NULL);

	CHRRAMSIZE = 8192;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}
