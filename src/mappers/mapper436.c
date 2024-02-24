/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2008 CaH4e3
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 436: 820401/T-217 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M436PW(uint16 A, uint16 V) {
	if (reg & 0x01) {
		setprg8(A, ((reg >> 2) & 0x30) | (V & 0x0F));
	} else {
		setprg32(0x8000, (reg >> 4));
	}
}

static void M436CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 1) & ~0x7F) | (V & 0x7F));
}

static DECLFW(M436Write) {
	reg = A & 0xFF;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M436Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M436Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M436Write);
}

void Mapper436_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M436PW;
	MMC3_cwrap = M436CW;
	info->Power = M436Power;
	info->Reset = M436Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
