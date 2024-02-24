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

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE = 0;

static void M194CW(uint16 A, uint16 V) {
	if ((V & ~0x01) == 0x00) { /* Dai-2-Ji - Super Robot Taisen (As).nes */
		setchr1r(0x10, A, V & 0x01);
    } else {
		setchr1(A, V);
    }
}

static void M194Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
		CHRRAM = NULL;
	}
}

void Mapper194_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	info->Close = M194Close;
	MMC3_cwrap = M194CW;

	CHRRAMSIZE = 2048;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CHRR");
}