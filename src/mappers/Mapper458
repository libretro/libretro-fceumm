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
static uint8 dipsw;

static void M458CW(uint16 A, uint16 V) {
	uint16 mask = 0x7F;
	uint16 base = reg << 4;

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M458PW(uint16 A, uint16 V) {
	uint8 prg = reg & 0x0F;

	if (reg & 0x10) {
		setprg32(0x8000, prg >> 1);
	} else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg);
	}
}

static DECLFR(M458Read) {
	if ((reg & 0x20) && (dipsw & 0x03)) {
		return CartBR((A & ~0x1F) | (dipsw & 0x1F));
	}
	return CartBR(A);
}

static DECLFW(M458Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M458Reset(void) {
	reg = 0;
	dipsw++;
	MMC3_Reset();
}

static void M458Power(void) {
	reg = 0;
	dipsw = 0;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, M458Read);
	SetWriteHandler(0x6000, 0x7FFF, M458Write);
}

void Mapper458_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M458CW;
	MMC3_pwrap = M458PW;
	info->Reset = M458Reset;
	info->Power = M458Power;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "CMD0");
}
