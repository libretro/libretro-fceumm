/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright (C) 2019 Libretro Team
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
 *
 */

/* NES 2.0 mapper 339 is used for a 21-in-1 multicart.
 * Its UNIF board name is BMC-K-3006. 
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_339
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M339CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 4) & ~0x7F) | (V & 0x7F));
}

static void M339PW(uint16 A, uint16 V) {
	uint16 base = reg & 0x1F;

	if (reg & 0x20) { /* MMC3 mode */
		setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F));
	} else {
		if ((reg & 0x07) == 0x06) { /* NROM-256 */
			setprg32(0x8000, base >> 1);
		} else { /* NROM-128 */
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		}
	}
}

static DECLFW(M339Write) {
	if (MMC3_WramIsWritable()) {
		reg = A & 0x3F;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M339Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M339Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M339Write);
}

void Mapper339_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M339PW;
	MMC3_cwrap = M339CW;
	info->Power = M339Power;
	info->Reset = M339Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
