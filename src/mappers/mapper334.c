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
 *
 *
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

static void M334PW(uint16 A, uint16 V) {
	setprg32(0x8000, reg >> 1);
}

static DECLFW(M334Write) {
	if (!(A & 0x01) && MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
	}
}

static DECLFR(M334Read) {
	if (A & 0x02) {
		return ((cpu.openbus & 0xFE) | (dipsw & 0x01));
	}
	return cpu.openbus;
}

static void M334Reset(void) {
	dipsw++;
	reg = 0;
	MMC3_Reset();
}

static void M334Power(void) {
	dipsw = 0;
	reg = 0;
	MMC3_Power();
	SetReadHandler(0x6000, 0x7FFF, M334Read);
	SetWriteHandler(0x6000, 0x7FFF, M334Write);
}

void Mapper334_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M334PW;
	info->Power = M334Power;
	info->Reset = M334Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
