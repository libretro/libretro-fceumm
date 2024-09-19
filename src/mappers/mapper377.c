/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2023
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

/* NES 2.0 Mapper 377 - NES 2.0 Mapper 377 is used for the
 * 1998 Super Game 8-in-1 (JY-111) pirate multicart. It works similarly to Mapper 267 except it has an outer 256KiB
 * PRG-ROM bank.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M377CW(uint16 A, uint16 V) {
	uint16 base = ((reg & 0x20) >> 2) | (reg & 0x06);

	setchr1(A, (base << 6) | (V & 0x7F));
}

static void M377PW(uint16 A, uint16 V) {
	uint16 base = ((reg & 0x20) >> 2) | (reg & 0x06);

	setprg8(A, (base << 3) | (V & 0x0F));
}

static DECLFW(M377Write) {
	if (!(reg & 0x80)) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M377Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M377Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M377Write);
}

void Mapper377_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M377CW;
	MMC3_pwrap = M377PW;
	info->Reset = M377Reset;
	info->Power = M377Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
