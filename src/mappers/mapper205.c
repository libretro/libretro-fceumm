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

/* iNES Mapper 205
 * UNIF boardname BMC-JC-016-2
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M205PW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x02) ? 0x0F : 0x1F;

	setprg8(A, (reg << 4) | (V & mask));
}

static void M205CW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x02) ? 0x7F : 0xFF;

	setchr1(A, (reg << 7) | (V & mask));
}

static DECLFW(M205Write) {
	CartBW(A, V);
	reg = V & 0x03;
	if ((V & 0x01) && dipsw) {
		reg |= 0x02;
	}
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M205Reset(void) {
	reg = 0;
	dipsw = (dipsw + 1) & 0x01; /* solder pad */
	MMC3_Reset();
}

static void M205Power(void) {
	reg = dipsw = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M205Write);
}

void Mapper205_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = M205PW;
	MMC3_cwrap = M205CW;
	info->Power = M205Power;
	info->Reset = M205Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
