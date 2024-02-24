/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2008 CaH4e3
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

/* NES 2.0 Mapper 348
 * M-022 MMC3 based 830118C T-106 4M + 4M */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M348CW(uint16 A, uint16 V) {
	uint16 mask = 0x7F;
	uint16 base = (reg << 5) & 0x180;

	setchr1(A, base | (V & mask));
}

static void M348PW(uint16 A, uint16 V) {
	uint8 mask = 0x0F;
	uint8 base = (reg << 2) & 0x30;

	if ((reg & 0x0C) == 0x0C) {
		setprg8(0x8000, base | ((mmc3.reg[6] & ~0x02) & mask));
		setprg8(0xA000, base | ((mmc3.reg[7] & ~0x02) & mask));
		setprg8(0xC000, base | ((mmc3.reg[6] |  0x02) & mask));
		setprg8(0xE000, base | ((mmc3.reg[7] |  0x02) & mask));
	} else {
		setprg8(A, base | (V & mask));
	}
}

static DECLFW(M348Write) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M348Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M348Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6800, 0x68FF, M348Write);
}

void Mapper348_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M348PW;
	MMC3_cwrap = M348CW;
	info->Power = M348Power;
	info->Reset = M348Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
