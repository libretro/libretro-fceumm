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

static void M115PW(uint16 A, uint16 V) {
	if (reg[0] & 0x80) {
		uint8 bank = reg[0] & 0x0F;

		if (reg[0] & 0x20) {
			setprg32(0x8000, bank >> 1);
		} else {
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else {
		setprg8(A, V & 0x3F);
	}
}

static void M115CW(uint16 A, uint16 V) {
	setchr1(A, ((reg[1] << 8) & 0x100) | (V & 0xFF));
}

static DECLFW(M115WriteReg) {
	reg[A & 0x01] = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M115Power(void) {
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M115WriteReg);
}

void Mapper115_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M115CW;
	MMC3_pwrap = M115PW;
	info->Power = M115Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
