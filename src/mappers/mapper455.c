/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

static uint8 reg[2];

static void M455PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x01) ? 0x1F : 0x0F;
	uint16 base = ((reg[0] >> 2) & 0x10) | ((reg[1] << 1) & 0x08) | ((reg[0] >> 2) & 0x07);

	if (reg[0] & 0x01) {
		if (reg[0] & 0x02) {
			setprg32(0x8000, base >> 1);
		} else {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		}
	} else {
		setprg8(A, ((base << 1) & ~mask) | (V & mask));
	}
}

static void M455CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x02) ? 0xFF : 0x7F;
	uint16 base = ((reg[0] >> 2) & 0x10) | ((reg[1] << 1) & 0x08) | ((reg[0] >> 2) & 0x07);

	setchr1(A, ((base << 4) & ~mask) | (V & mask));
}

static DECLFW(M455Write) {
	if (A & 0x100) {
		reg[0] = V;
		reg[1] = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M455Reset(void) {
	reg[0] = 1;
	reg[1] = 0;
	MMC3_Reset();
}

static void M455Power(void) {
	reg[0] = 1;
	reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x4100, 0x5FFF, M455Write);
}

void Mapper455_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M455CW;
	MMC3_pwrap = M455PW;
	info->Power = M455Power;
	info->Reset = M455Reset;
	AddExState(reg, 2, 0, "EXPR");
}
