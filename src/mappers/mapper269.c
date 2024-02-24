/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 269
 *   Games Xplosion 121-in-1
 *   15000-in-1
 *   18000-in-1
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];
static uint8 cmd;

static void M269CW(uint16 A, uint16 V) {
	uint32 mask = 0xFF >> (~reg[2] & 0xF);
	uint32 base = ((reg[2] << 4) & 0xF00) | reg[0];

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M269PW(uint16 A, uint16 V) {
	uint32 mask = ~reg[3] & 0x3F;
	uint32 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static DECLFW(M269Write5) {
	if (!(reg[3] & 0x80)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 3;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M269Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Reset();
}

static void M269Power(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M269Write5);
}

static uint8 unscrambleCHR(uint8 data) {
	return ((data & 0x01) << 6) | ((data & 0x02) << 3) | ((data & 0x04) << 0) | ((data & 0x08) >> 3) |
	    ((data & 0x10) >> 3) | ((data & 0x20) >> 2) | ((data & 0x40) >> 1) | ((data & 0x80) << 0);
}

void Mapper269_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_cwrap = M269CW;
	MMC3_pwrap = M269PW;
	info->Power = M269Power;
	info->Reset = M269Reset;
	AddExState(reg, 4, 0, "EXPR");
	AddExState(&cmd, 1, 0, "CMD0");

	if (UNIFchrrama) {
		uint32 i;
		if (ROM.chr.data) {
			FCEU_free(ROM.chr.data);
		}
		ROM.chr.data = (uint8*)FCEU_malloc(PRGsize[0]);
		/* unscramble CHR data from PRG */
		for (i = 0; i < PRGsize[0]; i++) {
			ROM.chr.data[i] = unscrambleCHR(ROM.prg.data[i]);
		}
		SetupCartCHRMapping(0, ROM.chr.data, PRGsize[0], 0);
	}
}
