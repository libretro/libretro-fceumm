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

static uint32 count;
static uint32 count_target = 0x28000000;

static void M105IRQHook(int a) {
	while (a--) {
		if (mmc1.reg[1] & 0x10) {
			count = 0;
			X6502_IRQEnd(FCEU_IQEXT);
		} else {
			if (++count == count_target) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
			if ((count % 1789773) == 0) {
				uint32 seconds = (count_target - count) / 1789773;
				FCEU_DispMessage(RETRO_LOG_INFO, 1000, "Time left: %02i:%02i\n", seconds / 60, seconds % 60);
			}
		}
	}
}

static void M105CW(uint16 A, uint16 V) {
    setchr8r(0, 0);
}

static void M105PW(uint16 A, uint16 V) {
	if (mmc1.reg[1] & 0x08) {
		setprg16(A, 8 | (V & 0x7));
	} else {
		setprg32(0x8000, (mmc1.reg[1] >> 1) & 0x03);
	}
}

static void M105Power(void) {
    count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	MMC1_Power();
}

static void M105Reset(void) {
	count_target = 0x20000000 | ((uint32)GameInfo->cspecial << 25);
	MMC1_Reset();
}

void Mapper105_Init(CartInfo *info) {
	MMC1_Init(info, 8, 0);
	MMC1_cwrap = M105CW;
	MMC1_pwrap = M105PW;
	MapIRQHook = M105IRQHook;
	info->Power = M105Power;
	info->Reset = M105Reset;
    AddExState(&count, 4, 0, "IRQC");
}
