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

/* Mapper 395 - Realtec 8210
 * Super Card 12-in-1 (SPC002)
 * Super Card 13-in-1 (SPC003)
 * Super Card 14-in-1 (King006)
 * Super Card 14-in-1 (King007)
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M395CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x40) ? 0x7F : 0xFF;
	uint16 base = ((reg[0] << 4) & 0x300) | ((reg[1] << 5) & 0x400) | ((reg[1] << 3) & 0x80);

	setchr1(A, base | (V & mask));
}

static void M395PW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x08) ? 0x0F : 0x1F;
	uint16 base = ((reg[0] << 4) & 0x80) | ((reg[0] << 1) & 0x60) | ((reg[1] << 4) & 0x10);

	setprg8(A, base | (V & mask));
}

static DECLFW(M395Write) {
	if (!(reg[1] & 0x80)) {
		reg[(A >> 4) & 0x01] = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M395Reset(void) {
	reg[0] = reg[1] = 0;
	MMC3_Reset();
}

static void M395Power(void) {
	reg[0] = reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M395Write);
}

void Mapper395_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M395CW;
	MMC3_pwrap = M395PW;
	info->Power = M395Power;
	info->Reset = M395Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
