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

static uint8 reg[4];

static void M420CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x80) ? 0x7F : 0xFF;
	uint16 base = ((reg[1] << 1) & 0x100) | ((reg[1] << 5) & 0x80);

	setchr1(A, base | (V & mask));
}

static void M420PW(uint16 A, uint16 V) {
	if (reg[0] & 0x80) {
		setprg32(0x8000, ((reg[2] >> 2) & 0x08) | ((reg[0] >> 1) & 0x07));
	} else {
		uint8 mask = (reg[0] & 0x20) ? 0x0F : ((reg[3] & 0x20) ? 0x1F : 0x3F);
		uint8 base = (reg[3] << 3) & 0x20;

		setprg8(A, base | (V & mask));
	}
}

static DECLFW(M420Write) {
	/* writes possible regardless of MMC3 wram state */
	CartBW(A, V);
	reg[A & 0x03] = V;
	MMC3_FixPRG();        
	MMC3_FixCHR();
}

static void M420Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
}

static void M420Power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M420Write);
}

void Mapper420_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M420CW;
	MMC3_pwrap = M420PW;
	info->Reset = M420Reset;
	info->Power = M420Power;
	AddExState(reg, 4, 0, "EXPR");
}
