/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

static void NROMClose(void) {
}

static void NROMPower(void) {
	if (ROM.prg.size == 3) { /* NROM-368 */
		setprg16(0x4000, 0);
		setprg16(0x8000, 1);
		setprg16(0xC000, 2);
		SetReadHandler(0x4800, 0xFFFF, CartBR);
	} else {
		setprg32(0x8000, 0);
		SetReadHandler(0x8000, 0xFFFF, CartBR);

		if (WRAMSIZE) {
			setprg8r(0x10, 0x6000, 0); /* Famili BASIC (v3.0) need it (uses only 4KB), FP-BASIC uses 8KB */
			SetReadHandler(0x6000, 0x7FFF, CartBR);
			SetWriteHandler(0x6000, 0x7FFF, CartBW);
			FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
		}
	}

	setchr8(0);
}

void Mapper000_Init(CartInfo *info) {
	info->Power = NROMPower;
	info->Close = NROMClose;

	WRAMSIZE = 8192;

	if (info->submapper) {
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

	if (!UNIFchrrama && info->CHRRamSize) {
		/* A variant of Wild Ball with both chr-rom and chr-ram indicated in header. */
		/* Chr-rom in this case should be remapped as chr-ram */
		SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], 1);
	}
}
