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

/* NES 2.0 Mapper 404 - JY012005
 * 1998 Super HiK 8-in-1 (JY-021B)
 */

#include "mapinc.h"
#include "mmc1.h"

static uint8 reg;

static void M404PW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x40) ? 0x07 : 0x0F;
	setprg16(A, ((reg << 3) & ~mask) | (V & mask));
}

static void M404CW(uint16 A, uint16 V) {
	setchr4(A, (reg << 5) | (V & 0x1F));
}

static DECLFW(M404Write) {
	if (!(reg & 0x80)) {
		reg = V;
		MMC1_FixPRG();
		MMC1_FixCHR();
	}
}

static void M404Reset(void) {
	reg = 0;
	MMC1_Reset();
}

static void M404Power(void) {
	reg = 0;
	MMC1_Power();
	SetWriteHandler(0x6000, 0x7FFF, M404Write);
}

void Mapper404_Init(CartInfo *info) {
	MMC1_Init(info, 0, 0);
	info->Power = M404Power;
	info->Reset = M404Reset;
	MMC1_cwrap = M404CW;
	MMC1_pwrap = M404PW;
	AddExState(&reg, 1, 0, "BANK");
}
