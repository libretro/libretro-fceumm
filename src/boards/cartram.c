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
#include "cartram.h"

static uint8_t *CHRRAMData = NULL;
static uint8_t *WRAMData = NULL;
uint32_t CHRRAMSize = 0;
uint32_t WRAMSize = 0;

void CartRAM_close (void) { /* Need to combine this in one function to avoid the problem of having to properly cascade two separate Close() functions for WRAM and CHR-RAM each */
	if (WRAMData) {
		FCEU_gfree(WRAMData);
		WRAMData = NULL;
	}
	if (CHRRAMData) {
		FCEU_gfree(CHRRAMData);
		CHRRAMData = NULL;
	}
}

void CartRAM_init (CartInfo *info, uint8_t defaultWRAMSizeKiB, uint8_t defaultCHRRAMSizeKiB) {
	WRAMSize = info->iNES2? (info->PRGRamSize +info->PRGRamSaveSize): (defaultWRAMSizeKiB *1024);
	if (WRAMSize) {
		WRAMData = (uint8_t*)FCEU_gmalloc(WRAMSize);
		SetupCartPRGMapping(0x10, WRAMData, WRAMSize, 1);
		AddExState(WRAMData, WRAMSize, 0, "WRAM");
		if (info->battery && (info->PRGRamSaveSize || !info->iNES2)) {
			info->SaveGame[0] = WRAMData;
			info->SaveGameLen[0] = info->iNES2? (uint32_t)info->PRGRamSaveSize: WRAMSize;
		}
	}
	CHRRAMSize = info->iNES2? (info->CHRRamSize +info->CHRRamSaveSize): (defaultCHRRAMSizeKiB *1024);
	if (ROM_size == 0) CHRRAMSize = 0; /* If there is no CHR-ROM, then any CHR-RAM will not be "extra" and therefore will be handled by ines.c, not here. */
	if (CHRRAMSize) {
		CHRRAMData = (uint8_t*)FCEU_gmalloc(CHRRAMSize);
		SetupCartCHRMapping(0x10, CHRRAMData, CHRRAMSize, 1);
		AddExState(CHRRAMData, CHRRAMSize, 0, "CRAM");
		if (info->battery && (info->CHRRamSaveSize || !info->iNES2)) {
			info->SaveGame[info->SaveGameLen[0]? 1: 0] = CHRRAMData;
			info->SaveGameLen[info->SaveGameLen[0]? 1: 0] = info->iNES2? (uint32_t)info->CHRRamSaveSize: CHRRAMSize;
		}
	}
	if (WRAMSize || CHRRAMSize) info->Close = CartRAM_close;
}

void CHRRAM_init (CartInfo *info, uint8_t defaultCHRRAMSizeKiB) {
	CartRAM_init(info, 0, defaultCHRRAMSizeKiB);
}

void WRAM_init (CartInfo *info, uint8_t defaultWRAMSizeKiB) {
	CartRAM_init(info, defaultWRAMSizeKiB, 0);
}
