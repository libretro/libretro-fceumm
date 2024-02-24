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

/* NES 2.0 Mapper 323 - UNIF FARID_SLROM_8-IN-1 */

#include "mapinc.h"
#include "mmc1.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M323PW(uint16 A, uint16 V) {
	uint8 mask = 0x07;
    uint8 base = reg >> 1;

	setprg16(A, (base & ~mask) | (V & mask));
}

static void M323CW(uint16 A, uint16 V) {
	uint16 mask = 0x1F;
    uint16 base = reg << 1;

	setchr4(A, (base & ~mask) | (V & mask));
}

static DECLFW(M323Write) {
	if (!(mmc1.reg[3] & 0x10) && !(reg & 0x08)) {
		reg = V;
		MMC1_FixCHR();
		MMC1_FixPRG();
        MMC1_FixMIR();
	}
}

static void M323Power(void) {
	reg = 0;
	MMC1_Power();
	SetWriteHandler(0x6000, 0x7FFF, M323Write);
}

static void M323Reset(void) {
    reg = 0;
	MMC1_Reset();
}

void Mapper323_Init(CartInfo *info) {
	MMC1_Init(info, 0, 0);
	MMC1_cwrap = M323CW;
	MMC1_pwrap = M323PW;
	info->Power = M323Power;
	info->Reset = M323Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
