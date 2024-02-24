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
#include "mmc1.h"

static int DetectMMC1WRAMSize(CartInfo *info, int *saveRAM) {
	int workRAM = 8;

	if (info->iNES2) {
		workRAM = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
		*saveRAM = info->PRGRamSaveSize / 1024;
		/* we only support sizes between 8K and 32K */
		if (workRAM > 0 && workRAM < 8)
			workRAM = 8;
		if (workRAM > 32)
			workRAM = 32;
		if (*saveRAM > 0 && *saveRAM < 8)
			*saveRAM = 8;
		if (*saveRAM > 32)
			*saveRAM = 32;
		/* save ram cannot be bigger than workram */
		if (*saveRAM > workRAM) {
			*saveRAM = workRAM;
			workRAM = 0;
		}
	} else if (info->battery) {
		*saveRAM = 8;
	}
	if (workRAM > 8) {
		FCEU_printf(" >8KB external WRAM present.  Use NES 2.0 if you hack the ROM image.\n");
	}
	return workRAM;
}

static void M001PW(uint16 A, uint16 V) {
	if (iNESCart.submapper == 5) {
		setprg32(0x8000, 0);
	} else {
		setprg16(A, (MMC1_GetCHRBank(0) & 0x10) | (V & 0x0F));
	}
}

void Mapper001_Init(CartInfo *info) {
	int bs = 0;
	int ws = DetectMMC1WRAMSize(info, &bs);
	MMC1_Init(info, ws, bs);
	MMC1_pwrap = M001PW;
	mmc1_type = (info->submapper == 3) ? MMC1A : MMC1B;
}
