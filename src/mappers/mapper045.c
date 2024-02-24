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

static uint8 reg[4];
static uint8 cmd;
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 4, "REGS" },
	{ &cmd, 1, "CMD0" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static void M045CW(uint16 A, uint16 V) {
	if (UNIFchrrama) {
		/* assume chr-ram, 4-in-1 Yhc-Sxx-xx variants */
		setchr8(0);
	} else {
		uint32 mask = 0xFF >> (~reg[2] & 0x0F);
		uint32 base = ((reg[2] << 4) & 0xF00) | reg[0];

		setchr1(A, (base & ~mask) | (V & mask));
	}
}

static void M045PW(uint16 A, uint16 V) {
	uint32 mask = ~reg[3] & 0x3F;
	uint32 base = ((reg[2] << 2) & 0x300) | reg[1];

	setprg8(A, (base & ~mask) | (V & mask));
}

static DECLFR(M045ReadCart) {
    /* Some multicarts select between five different menus by connecting one of the higher address lines to PRG /CE.
	The menu code selects between menus by checking which of the higher address lines disables PRG-ROM when set. */
	if (((PRGsize[0] < 0x200000) && (dipsw == 1) && (reg[1] & 0x80)) ||
	    ((PRGsize[0] < 0x200000) && (dipsw == 2) && (reg[2] & 0x40)) ||
	    ((PRGsize[0] < 0x100000) && (dipsw == 3) && (reg[1] & 0x40)) ||
	    ((PRGsize[0] < 0x100000) && (dipsw == 4) && (reg[2] & 0x20))) {
		return cpu.openbus;
	}
	return CartBR(A);
}

static DECLFW(M045WriteReg) {
	CartBW(A, V);
	if (!(reg[3] & 0x40)) {
		reg[cmd] = V;
		cmd = (cmd + 1) & 0x03;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static DECLFR(M045ReadDIP) {
	uint32 addr = 1 << (dipsw + 4);
	if (A & (addr | (addr - 1))) {
		return 0x01;
	}
	return 0x00;
}

static void M045Reset(void) {
	reg[0] = reg[1] = reg[3] = cmd = 0;
	reg[2] = 0x0F;
	dipsw++;
	dipsw &= 7;
	MMC3_Reset();
}

static void M045Power(void) {
	reg[0] = reg[1] = reg[3] = cmd = dipsw = 0;
	reg[2] = 0x0F;
	MMC3_Power();
    SetReadHandler(0x8000, 0xFFFF, M045ReadCart);
	SetWriteHandler(0x6000, 0x7FFF, M045WriteReg);
	SetReadHandler(0x5000, 0x5FFF, M045ReadDIP);
}

void Mapper045_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	MMC3_cwrap = M045CW;
	MMC3_pwrap = M045PW;
	info->Reset = M045Reset;
	info->Power = M045Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
