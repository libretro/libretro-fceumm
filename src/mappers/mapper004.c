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
#include "mmc6.h"

static void M004PW(uint16 A, uint16 V) {
	setprg8(A, V & 0xFF);
}

static void M004CW(uint16 A, uint16 V) {
	setchr1(A, V & 0xFF);
}

void Mapper004_Init(CartInfo *info) {
	int ws = 8;

	if (info->submapper == 1) {
		MMC6_Init(info);
		return;
	}

	if (info->submapper == 5) {
		Mapper249_Init(info);
		return;
	}

	if (!info->iNES2) {
		if ((info->CRC32 == 0x93991433) || (info->CRC32 == 0xaf65aa84)) {
			FCEU_printf(
			    "Low-G-Man can not work normally in the iNES format.\nThis game has been recognized by its CRC32 "
			    "value, and the appropriate changes will be made so it will run.\nIf you wish to hack this game, "
			    "you should use the UNIF format for your hack.\n\n");
			ws = 0;
		}
	} else {
		ws = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
	}

	if ((info->submapper == 4) || (info->CRC32 == 0x97b6cb19)) {
		isRevB = 0; /* MMC3A */
	}

	if ((info->CRC32 == 0x762653B1) && (ws < 8)) {
		/* Mickey Mouse III - Dream Balloon [NikcDC v1.1] */
		ws = 8;
	}

	MMC3_Init(info, ws, info->battery);
	MMC3_pwrap = M004PW;
	MMC3_cwrap = M004CW;
}
