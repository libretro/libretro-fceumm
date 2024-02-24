/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2008 CaH4e3
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

/* T-227-1, 820632, MMC3 based, multimenu, 60000in1 (0010) dip switches */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reset_flag = 0x07;
static uint8 reg;

static void BMCT2271CW(uint16 A, uint16 V) {
	uint32 va = V;
	if (reg & 0x20) {
		va |= 0x200;
		va |= (reg & 0x10) << 4;
	} else {
		va &= 0x7F;
		va |= (reg & 0x18) << 4;
	}
	setchr1(A, va);
}

static void BMCT2271PW(uint16 A, uint16 V) {
	uint32 va = V & 0x3F;
	if (reg & 0x20) {
		va &= 0x1F;
		va |= 0x40;
		va |= (reg & 0x10) << 1;
	} else {
		va &= 0x0F;
		va |= (reg & 0x18) << 1;
	}
	switch (reg & 3) {
	case 0x00: setprg8(A, va); break;
	case 0x02:
	{
		va = (va & 0xFD) | ((reg & 4) >> 1);
		if (A < 0xC000) {
			setprg16(0x8000, va >> 1);
			setprg16(0xC000, va >> 1);
		}
		break;
	}
	case 0x01:
	case 0x03: if (A < 0xC000) setprg32(0x8000, va >> 2); break;
	}
}

static DECLFW(BMCT2271LoWrite) {
	if (!(reg & 0x80))
		reg = A & 0xFF;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static DECLFR(BMCT2271HiRead) {
	uint32 av = A;
	if (reg & 0x40) av = (av & 0xFFF0) | reset_flag;
	return CartBR(av);
}

static void BMCT2271Reset(void) {
	reg = 0x00;
	reset_flag++;
	reset_flag &= 0x0F;
	MMC3_Reset();
}

static void BMCT2271Power(void) {
	reg = 0x00;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, BMCT2271LoWrite);
	SetReadHandler(0x8000, 0xFFFF, BMCT2271HiRead);
}

void BMCT2271_Init(CartInfo *info) {
	MMC3_Init(info, 8, 0);
	MMC3_pwrap = BMCT2271PW;
	MMC3_cwrap = BMCT2271CW;
	info->Power = BMCT2271Power;
	info->Reset = BMCT2271Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
