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

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M047PW(uint16 A, uint16 V) {
	setprg8(A, (reg << 4) | (V & 0x0F));
}

static void M047CW(uint16 A, uint16 V) {
	setchr1(A, (reg << 7) | (V & 0x7F));
}

static DECLFW(M047Write) {
    if (MMC3_WramIsWritable()) {
	    reg = V;
	    MMC3_FixPRG();
	    MMC3_FixCHR();
    }
}

static void M047Reset(void) {
	MMC3_Reset();
}

static void M047Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M047Write);
}

void Mapper047_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M047PW;
	MMC3_cwrap = M047CW;
	info->Power = M047Power;
	info->Reset = M047Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
