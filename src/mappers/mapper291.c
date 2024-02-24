/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M291CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 2) & 0x100) | (V & 0xFF));
}

static void M291PW(uint16 A, uint16 V) {
	if (reg & 0x20) {
		setprg32(0x8000, ((reg >> 4) & 0x04) | ((reg >> 1) & 0x03));
	} else {
		setprg8(A, ((reg >> 2) & 0x10) | (V & 0x0F));
	}
}

static DECLFW(M291Write) {
	/* The Outer Bank Register responds even when the MMC3 clone's WRAM bit is clear. */
	reg = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M291Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M291Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M291Write);
}

void Mapper291_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M291CW;
	MMC3_pwrap = M291PW;
	info->Power = M291Power;
	info->Reset = M291Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
