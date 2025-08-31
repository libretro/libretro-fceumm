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
#include "wram.h"

static uint8 *WRAMData = NULL;
uint32 WRAMSize = 0;

void WRAM_close (void) {
	if (WRAMData) {
		FCEU_gfree(WRAMData);
		WRAMData =NULL;
	}
}

void WRAM_init (CartInfo *info, uint8 defaultWRAMSizeKiB) {
	WRAMSize =info->iNES2? (info->PRGRamSize +info->PRGRamSaveSize): (defaultWRAMSizeKiB *1024);
	if (WRAMSize) {
		info->Close =WRAM_close;
		WRAMData =(uint8*)FCEU_gmalloc(WRAMSize);
		SetupCartPRGMapping(0x10, WRAMData, WRAMSize, 1);
		AddExState(WRAMData, WRAMSize, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] =WRAMData;
			info->SaveGameLen[0] =WRAMSize;
		}
	}
}
