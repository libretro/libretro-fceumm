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

/* NES 2.0 Mapper 315
 * BMC-830134C
 * Used for multicarts using 820732C- and 830134C-numbered PCBs such as 4-in-1 Street Blaster 5
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_315
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M315CCW(uint16 A, uint16 V) {
	uint16 mask = 0xFF;
	uint16 base = reg << 8;

	V = ((reg << 6) & 0x80) | ((reg << 3) & 0x40) | (V & 0xFF);
	setchr1(A, (base & ~mask) | (V & mask));
}

static void M315CPW(uint16 A, uint16 V) {
	uint16 mask = 0x0F;
	uint16 base = reg << 3;

	if ((reg & 0x06) == 0x06) { /* GNROM-like */
		setprg8(0x8000, (base & ~mask) | ((mmc3.reg[6] & ~0x02) & mask));
		setprg8(0xA000, (base & ~mask) | ((mmc3.reg[7] & ~0x02) & mask));
		setprg8(0xC000, (base & ~mask) | ((mmc3.reg[6] |  0x02) & mask));
		setprg8(0xE000, (base & ~mask) | ((mmc3.reg[7] |  0x02) & mask));
	} else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static DECLFW(M315CWrite) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M315CReset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M315CPower(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M315CWrite);
}

void Mapper315_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M315CPW;
	MMC3_cwrap = M315CCW;
	info->Power = M315CPower;
	info->Reset = M315CReset;
	AddExState(&reg, 1, 0, "EXPR");
}
