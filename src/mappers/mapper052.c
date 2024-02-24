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

/* Submapper 13/14 - CHR-ROM + CHR-RAM */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static void M052PW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x08) ? 0x0F : 0x1F;
	uint8 base = (reg << 4) & 0x70;

	setprg8(A, (base & ~mask) | (V & mask));
}

static void M052CW(uint16 A, uint16 V) {
	uint8 chrram = ((iNESCart.submapper == 13) && ((reg & 0x03) == 0x03)) ||
	               ((iNESCart.submapper == 14) && (reg & 0x20));

	if (chrram) {
		setchr8r(0x10, 0);
	} else {
		uint16 mask = (reg & 0x40) ? 0x7F : 0xFF;
		uint16 base = (((reg << 3) & 0x180) | ((reg << 7) & 0x200));

		if (iNESCart.CRC32 == 0x68FE207F) {
			/* Mario 7-in-1 (YH-705) with wrong bank order */
			base = ((reg & 0x20) << 4) | ((reg & 0x04) << 6) |
			       (reg & 0x40 ? (reg & 0x10) << 3 : 0x00);
		} else if (iNESCart.submapper == 14) {
			/* Well 8-in-1 (AB128) (Unl) (p1) */
			base = ((reg << 3) & 0x080) | ((reg << 7) & 0x300);
		}

		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static DECLFW(M052Write) {
	if (MMC3_WramIsWritable()) {
		if (!(reg & 0x80)) {
			reg = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
		} else {
			CartBW(A, V);
		}
	}
}

static void M052Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
		CHRRAM = NULL;
	}
}

static void M052Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M052Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M052Write);
}

void Mapper052_Init(CartInfo *info) {
	uint8 ws = info->iNES2 ? (info->PRGRamSize + info->PRGRamSaveSize) / 1024 : 8;

	MMC3_Init(info, ws, info->battery);
	MMC3_cwrap = M052CW;
	MMC3_pwrap = M052PW;

	info->Reset = M052Reset;
	info->Power = M052Power;
	info->Close = M052Close;
	AddExState(&reg, 1, 0, "EXPR");

	if (info->CRC32 == 0xA874E216 && info->submapper != 13) {
		info->submapper = 13; /* (YH-430) 97-98 Four-in-One */
	} else if (info->CRC32 == 0xCCE8CA2F && info->submapper != 14) {
		/* Well 8-in-1 (AB128) (Unl) (p1), with 1024 PRG and CHR is incompatible with submapper 13.
		 * This is reassigned to submapper 14 instead. */
		info->submapper = 14;
	}

	if ((info->submapper == 13) || (info->submapper == 14)) {
		CHRRAMSIZE = info->CHRRamSize ? info->CHRRamSize : 8192;
		CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
	}
}
