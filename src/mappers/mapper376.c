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

static uint8 reg[2];

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ 0 }
};

static void M376CW(uint16 A, uint16 V) {
	uint16 base = ((reg[1] << 8) & 0x100) | ((reg[0] << 1) & 0x80);

	setchr1(A, base | (V & 0x7F));
}

static void M376PW(uint16 A, uint16 V) {
	uint16 base = ((reg[1] << 4) & 0x10) | ((reg[0] >> 3) & 0x08) | (reg[0] & 0x07);

	if (reg[0] & 0x80) {
		if (reg[0] & 0x20) {
			setprg32(0x8000, base >> 1);
		} else {
			setprg16(0x8000, base);
			setprg16(0xC000, base);
		}
	} else {
		setprg8(A, ((base << 1) & ~0x0F) | (V & 0x0F));
	}
}

static DECLFW(M376Write) {
	reg[A & 0x01] = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M376Power(void) {
	reg[0] = 0;
	reg[1] = 0;
	MMC3_Power();
	SetWriteHandler(0x7000, 0x7FFF, M376Write);
}

void Mapper376_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M376PW;
	MMC3_cwrap = M376CW;
	info->Power = M376Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
