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
static uint8 dipsw;

static SFORMAT StateRegs[] = {
	{ reg, 2, "REGS" },
	{ &dipsw, 1, "DPSW" },
	{ 0 }
};

static uint32 M114_addr[2][8] = {
	{ 0xA001, 0xA000, 0x8000, 0xC000, 0x8001, 0xC001, 0xE000, 0xE001 }, /* 0: Aladdin, The Lion King */
	{ 0xA001, 0x8001, 0x8000, 0xC001, 0xA000, 0xC000, 0xE000, 0xE001 }  /* 1: Boogerman */
};

static uint8 M114_index[2][8] = {
	{ 0, 3, 1, 5, 6, 7, 2, 4 }, /* 0: Aladdin, The Lion King */
	{ 0, 2, 5, 3, 6, 1, 7, 4 }, /* 1: Boogerman */
};

static void M114PW(uint16 A, uint16 V) {
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

static void M114CW(uint16 A, uint16 V) {
	setchr1(A, ((reg[1] << 8) & 0x100) | (V & 0x0FF));
}

static DECLFW(M114WriteReg) {
	reg[A & 0x01] = V;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static DECLFW(M114Write) {
	A = M114_addr[iNESCart.submapper & 0x01][((A >> 12) & 0x06) | (A & 0x01)];
	switch (A & 0xE001) {
	case 0x8000:
		V = (V & 0xF8) | M114_index[iNESCart.submapper & 0x01][V & 0x07];
		MMC3_CMDWrite(A, V);
		break;
	default:
		MMC3_Write(A, V);
		break;
	}
}

static DECLFR(M114ReadDip) {
	if ((A & 0x03) == 0x02) {
		return (cpu.openbus & ~0x07) | (dipsw & 0x07);
	}
	return cpu.openbus;
}

static void M114Power(void) {
	reg[0] = reg[1] = 0;
	MMC3_Power();
	SetReadHandler(0x6000, 0x7FFF, M114ReadDip);
	SetWriteHandler(0x6000, 0x7FFF, M114WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M114Write);
}

static void M114Reset(void) {
	reg[0] = reg[1] = 0;
	dipsw++;
	MMC3_Reset();
}

void Mapper114_Init(CartInfo *info) {
	isRevB = 0;
	/* Use NES 2.0 submapper to identify scrambling pattern, otherwise CRC32 for Boogerman and test rom */
	if (!info->iNES2) {
		if ((info->CRC32 == 0x80eb1839) || (info->CRC32 == 0x071e4ee8)) {
			info->submapper = 1;
		}
	}

	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M114PW;
	MMC3_cwrap = M114CW;
	info->Power = M114Power;
	info->Reset = M114Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
