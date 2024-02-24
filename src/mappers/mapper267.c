/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 267 - 8-in-1 JY-119 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M267CW(uint16 A, uint16 V) {
	uint16 base = ((reg & 0x20) >> 2) | (reg & 0x06);

	setchr1(A, (base << 6) | (V & 0x7F));
}

static void M267PW(uint16 A, uint16 V) {
	uint16 base = ((reg & 0x20) >> 2) | (reg & 0x06);

	setprg8(A, (base << 4) | (V & 0x1F));
}

static DECLFW(M267Write) {
	if (MMC3_WramIsWritable()) {
		if (!(reg & 0x80)) {
			reg = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M267Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M267Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x6FFF, M267Write);
}

void Mapper267_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M267CW;
	MMC3_pwrap = M267PW;
	info->Reset = M267Reset;
	info->Power = M267Power;
	AddExState(&reg, 1, 0, "EXPR");
}
