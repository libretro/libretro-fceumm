/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2014 CaitSith2, 2022 Cluster
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

/*
 * Roms still using NES 1.0 format should be loaded as 8K CHR RAM.
 * Roms defined under NES 2.0 should use the VRAM size field, defining 7, 8 or 9, based on how much VRAM should be
 * present. UNIF doesn't have this problem, because unique board names can define this information. The UNIF names are
 * UNROM-512-8K, UNROM-512-16K and UNROM-512-32K
 *
 * The battery flag in the NES header enables flash,  Mirrror mode 2 Enables MI_0 and MI_1 mode.
 * Known games to use this board are:
 *    Battle Kid 2: Mountain of Torment (512K PRG, 8K CHR RAM, Horizontal Mirroring, Flash disabled)
 *    Study Hall (128K PRG (in 512K flash chip), 8K CHR RAM, Horizontal Mirroring, Flash enabled)
 *    Nix: The Paradox Relic (512 PRG, 8K CHR RAM, Vertical Mirroring, Flash enabled)
 * Although Xmas 2013 uses a different board, where LEDs can be controlled (with writes to the $8000-BFFF space),
 * it otherwise functions identically.
  */

#include "mapinc.h"
#include "latch.h"
#include "flashrom.h"

#define ROM_CHIP   0x00
#define FLASH_CHIP 0x10

static uint8 flash_save;
static uint8 *flash_data;

static void M030Sync(void) {
	int chip = flash_save ? FLASH_CHIP : ROM_CHIP;

	setprg16r(chip, 0x8000, latch.data & 0x1F);
	setprg16r(chip, 0xC000, ~0);
	setchr8((latch.data >> 5) & 0x03);
	switch (iNESCart.submapper) {
	case 1:
		/* Mega Man II (30th Anniversary Edition) */
		setmirror((latch.data >> 7) & 0x01);	
		break;
	default:
		setmirror(MI_0 + ((latch.data >> 7) & 0x01));
		break;
	}
}

static void M030CPUHook(int a) {
	FlashROM_CPUCyle(a);
}

static DECLFR(M030Read) {
	if ((A < 0xC000) && flash_save) {
		return FlashROM_Read(A);
	}
	return CartBR(A);
}

static DECLFW(M030Write) {
	if ((A < 0xC000) && flash_save) {
		FlashROM_Write(A, V);
	} else {
		Latch_Write(A, V);
	}
}

static void M030Power(void) {
	Latch_Power();
	SetReadHandler(0x8000, 0xFFFF, M030Read);
	SetWriteHandler(0x8000, 0xBFFF, M030Write);
}

static void M030Close(void) {
	Latch_Close();
	if (flash_data) {
		FCEU_gfree(flash_data);
	}
	flash_data = NULL;
}

void Mapper030_Init(CartInfo *info) {
	flash_save = info->battery;
	Latch_Init(info, M030Sync, NULL, 0, !flash_save);

	if (!info->submapper && (info->PRGCRC32 == 0x891C14BC)) {
		info->submapper = 0x01;
	}

	if (!(info->submapper & 1)) {
		switch (info->mirror2bits) {
		case 0: /* hard horizontal, internal */
			SetupCartMirroring(MI_H, 1, NULL);
			break;
		case 1: /* hard vertical, internal */
			SetupCartMirroring(MI_V, 1, NULL);
			break;
		case 2: /* switchable 1-screen, internal (flags: 4-screen + horizontal) */
			SetupCartMirroring(MI_0, 0, NULL);
			break;
		case 3: /* hard four screen, last 8k of 32k RAM (flags: 4-screen + vertical) */
			SetupCartMirroring(4, 1, ROM.chr.data + (info->CHRRamSize - 8192));
			break;
		}
	}

	info->Power = M030Power;
	info->Close = M030Close;

	if (flash_save) {
		uint32 i, ssize;
		/* Allocate memory for flash */
		ssize = PRGsize[0];
		flash_data = (uint8 *)FCEU_gmalloc(ssize);
		/* Copy ROM to flash data */
		for (i = 0; i < ssize; i++) {
			flash_data[i] = PRGptr[ROM_CHIP][i % ssize];
		}
		SetupCartPRGMapping(FLASH_CHIP, flash_data, ssize, 1);
		AddExState(flash_data, ssize, 0, "FLSH");
		info->SaveGame[0] = flash_data;
		info->SaveGameLen[0] = ssize;

		FlashROM_Init(flash_data, ssize, 0xBF, 0xB7, 4096, 0x5555, 0x2AAA);
		MapIRQHook = M030CPUHook;
	}
}
