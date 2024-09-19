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

/* NES 2.0 Mapper 344
 * BMC-GN-26
 * Kuai Da Jin Ka Zhong Ji Tiao Zhan 3-in-1 (3-in-1,6-in-1,Unl)
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint8 prg_bank_order[2][4] = {
	{ 0, 1, 2, 3 }, /* normal bank order */
	{ 0, 3, 1, 2 }  /* wrong bank order, added for compatibility */
};

static void M344CW(uint16 A, uint16 V) {
	uint16 mask = (reg & 0x02) ? 0x7F : 0xFF;
	uint16 base = (reg & 0x03) << 7;

	setchr1(A, base | (V & mask));
}

static void M344PW(uint16 A, uint16 V) {
	uint8 base = prg_bank_order[(iNESCart.PRGCRC32 == 0xAB2ACA46)][reg & 0x03];

	if (reg & 0x04) {
		setprg32(0x8000, (base << 2) | ((mmc3.reg[6] & 0x0F) >> 2));
	} else {
		setprg8(A, (base << 4) | (V & 0x0F));
	}
}

static DECLFW(M344Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M344Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M344Power(void) {
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M344Write);
}

void Mapper344_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M344PW;
	MMC3_cwrap = M344CW;
	info->Power = M344Power;
	info->Reset = M344Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
