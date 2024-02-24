/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

/* NES 2.0 Mapper 322
 * BMC-K-3033
 * 35-in-1 (K-3033)
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_322
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M322CW(uint16 A, uint16 V) {
	if (reg & 0x20) {
		uint16 base = ((reg >> 4) & 0x04) | ((reg >> 3) & 0x03);

		if (reg & 0x80) {
			setchr1(A, (base << 8) | (V & 0xFF));
		} else {
			setchr1(A, (base << 7) | (V & 0x7F));
		}
	} else {
		setchr1(A, (V & 0x7F));
	}
}

static void M322PW(uint16 A, uint16 V) {
	uint16 base = ((reg >> 4) & 0x04) | ((reg >> 3) & 0x03);

	if (reg & 0x20) {
		if (reg & 0x80) {
			setprg8(A, (base << 5) | (V & 0x1F));
		} else {
			setprg8(A, (base << 4) | (V & 0x0F));
		}
	} else {
		if (reg & 0x03) {
			setprg32(0x8000, (base << 3) | ((reg >> 1) & 0x03));
		} else {
			setprg16(0x8000, (base << 3) | (reg & 0x07));
			setprg16(0xC000, (base << 3) | (reg & 0x07));
		}
	}
}

static DECLFW(M322Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0xFF;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M322Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M322Write);
}

static void M322Reset(void) {
	reg = 0;
	MMC3_Reset();
}

void Mapper322_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M322PW;
	MMC3_cwrap = M322CW;
	info->Power = M322Power;
	info->Reset = M322Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
