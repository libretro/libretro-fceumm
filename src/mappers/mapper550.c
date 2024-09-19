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

/* NES 2.0 Mapper 550 - 7-in-1 1993 Chess Series (JY-015) */

#include "mapinc.h"
#include "mmc1.h"

static uint8 latch;
static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &latch, 1, "LATC" },
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M550PW(uint16 A, uint16 V) {
	if ((reg & 0x06) == 0x06) {
		setprg16(A, (reg << 2) | (V & 0x07));
	} else {
		setprg32(0x8000, (reg << 1) | ((latch >> 4) & 0x01));
	}
}

static void M550CW(uint16 A, uint16 V) {
	if ((reg & 0x06) == 0x06) {
		setchr4(A, ((reg << 2) & 0x18) | (V & 0x07));
	} else {
		setchr8(((reg << 1) & 0x0C) | (latch & 0x03));
	}
}

static DECLFW(M550Write7) {
	if (!(reg & 0x08)) {
		reg = A & 0x0F;
		MMC1_FixPRG();
		MMC1_FixCHR();
	}
}

static DECLFW(M550Write8) {
	latch = V;
	if ((reg & 0x06) == 0x06) {
		MMC1_Write(A, V);
	}
	MMC1_FixPRG();
	MMC1_FixCHR();
}

static void M550Reset(void) {
	latch = 0;
	reg = 0;
	MMC1_Reset();
}

static void M550Power(void) {
	latch = 0;
	reg = 0;
	MMC1_Power();
	SetWriteHandler(0x7000, 0x7FFF, M550Write7);
	SetWriteHandler(0x8000, 0xFFFF, M550Write8);
}

void Mapper550_Init(CartInfo *info) {
	MMC1_Init(info, 8, 0);
	info->Power = M550Power;
	info->Reset = M550Reset;
	MMC1_cwrap = M550CW;
	MMC1_pwrap = M550PW;
	AddExState(StateRegs, ~0, 0, NULL);
}
