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

/*
 * Mapper 366 (GN-45):
 *  K-3131GS
 *  K-3131SS
*/	

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M366PW(uint16 A, uint16 V) {
	setprg8(A, reg | (V & 0x0F));
}

static void M366CW(uint16 A, uint16 V) {
	setchr1(A, (reg << 3) | (V & 0x7F));
}

static void M366Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static DECLFW(M366Write) {
	CartBW(A, V);
	if (!(reg & 0x80)) {
		reg = A & 0xF0;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}	
}

static void M366Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M366Write);
}

void Mapper366_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M366PW;
	MMC3_cwrap = M366CW;
	info->Power = M366Power;
	info->Reset = M366Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
