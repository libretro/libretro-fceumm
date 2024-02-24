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

/* NES 2.0 Mapper 406 */
/* Uses flashrom to save high scores. */

#include "mapinc.h"
#include "mmc3.h"
#include "flashrom.h"

static uint8 *FLASHROM_data = NULL;
static uint32 FLASHROM_size = 0;

static void M406PW(uint16 A, uint16 V) {
	setprg8r(0x10, A, V & 0x3F);
}

static DECLFR(M406Read) {
	return FlashROM_Read(A);
}

static DECLFW(M406Write) {
	FlashROM_Write(A, V);
	if (iNESCart.submapper == 0) {
		A = (A & 0xFFFC) | ((A << 1) & 2) | ((A >> 1) & 1);
	} else if ((A <= 0x9000) || (A >= 0xE000)) {
		A = A ^ 0x6000;
	}
	MMC3_Write(A, V);
}

static void M406Power(void) {
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M406Read);
	SetWriteHandler(0x8000, 0xFFFF, M406Write);
}

static void M406Close(void) {
	MMC3_Close();
	if (FLASHROM_data) {
		FCEU_free(FLASHROM_data);
	}
	FLASHROM_data = NULL;
}

void Mapper406_Init(CartInfo *info) {
	uint32 w, r, id;

	MMC3_Init(info, 0, 0);
	info->Power = M406Power;
	info->Close = M406Close;
	MMC3_pwrap = M406PW;
	MapIRQHook = FlashROM_CPUCyle;

	info->battery = 1;
	FLASHROM_size = PRGsize[0];
	FLASHROM_data = (uint8 *)FCEU_gmalloc(FLASHROM_size);
	info->SaveGame[0]    = FLASHROM_data;
	info->SaveGameLen[0] = FLASHROM_size;
	AddExState(FLASHROM_data, FLASHROM_size, 0, "FROM");
	/* copy PRG ROM into FLASHROM_data, use it instead of PRG ROM */
	for (w = 0, r = 0; w < FLASHROM_size; w++) {
		FLASHROM_data[w] = PRGptr[0][r];
		++r;
	}
	SetupCartPRGMapping(0x10, FLASHROM_data, FLASHROM_size, 0);

	id = (info->submapper == 0) ? 0xC2 : 0x01;
	FlashROM_Init(FLASHROM_data, FLASHROM_size, id, 0xA4, 65536, 0x5555, 0x02AAA);
}
