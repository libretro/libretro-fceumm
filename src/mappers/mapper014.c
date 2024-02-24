/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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
 * SL1632 2-in-1 protected board, similar to SL12
 * Samurai Spirits Rex (Full)
 *
 */

#include "mapinc.h"
#include "mmc3.h"
#include "vrc2and4.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 },
};

static uint8 GetChrBase(uint16 A) {
	if (A & 0x1000) {
		if (A & 0x800) {
			return ((reg & 0x80) >> 7);
		} else {
			return ((reg & 0x20) >> 5);
		}
	}
	return ((reg & 0x08) >> 3);
}

static void M014MMC3CW(uint16 A, uint16 V) {
	uint16 mask = 0xFF;
	uint16 base = GetChrBase(A) << 8;

	setchr1(A, (base & ~mask) | (V & mask));
}

static void M014VRC24CW(uint16 A, uint16 V) {
	uint16 mask = 0xFF;
	uint16 base = GetChrBase(A) << 8;

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW(M014Write) {
	if (A == 0xA131) {
		reg = V;
		if (reg & 0x02) {
			MMC3_FixCHR();
		} else {
			VRC24_FixCHR();
		}
	}
	if (reg & 0x02) {
		MMC3_Write(A, V);
	} else {
		VRC24_Write(A, V);
	}
}

static void StateRestore(int version) {
	if (reg & 0x02) {
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else {
		VRC24_FixPRG();
		VRC24_FixCHR();
		VRC24_FixMIR();
	}
}

static void M014Power(void) {
	reg = 0;
	VRC24_Power();
	SetWriteHandler(0x8000, 0xFFFF, M014Write);
}

void Mapper014_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M014MMC3CW;

	VRC24_Init(info, VRC2, 0x01, 0x02, FALSE, TRUE);
	VRC24_cwrap = M014VRC24CW;

	info->Power = M014Power;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
