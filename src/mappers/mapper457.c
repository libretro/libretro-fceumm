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

static uint8 reg;

static void M457CW(uint16 A, uint16 V) {
	uint32 mask = (reg & 0x08) ? 0xFF : 0x7F;
	uint32 base = reg << 7;

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M457PW(uint16 A, uint16 V) {
	uint32 mask = (reg & 0x08) ? 0x1F : 0x0F;
	uint32 base = reg << 4;

	setprg8(A, (base & ~mask) | (V & mask));
}

static DECLFW(M457Write) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M457Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M457Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M457Write);
}

void Mapper457_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M457CW;
	MMC3_pwrap = M457PW;
	info->Reset = M457Reset;
	info->Power = M457Power;
	AddExState(&reg, 1, 0, "EXPR");
}
