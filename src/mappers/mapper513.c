/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

/* NES 2.0 Mapper 513 - Sachen UNL-SA-9602B */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M513CW(uint16 A, uint16 V) {
	setchr1(A, V & 0x3F);
}

static void M513PW(uint16 A, uint16 V) {
	if (!(A & 0x4000)) {
		setprg8(A, (reg & 0xC0) | (V & 0x3F));
	} else {
		setprg8(A, (V & 0x3F));
	}
}

static DECLFW(M513Write) {
	switch (A & 0xE001) {
	case 0x8000:
		mmc3.cmd = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		break;
	case 0x8001:
		mmc3.reg[mmc3.cmd & 0x07] = V;
		switch (mmc3.cmd & 0x07) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			reg = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
			break;
		default:
			MMC3_FixPRG();
			break;
		}
	default:
		break;
	}
}

static void M513Power(void) {
	reg = 0;
	MMC3_Power();
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0x9FFF, M513Write);
}

void Mapper513_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M513PW;
	MMC3_cwrap = M513CW;
	mmc3.opts |= 2;
	info->SaveGame[0] = UNIFchrrama;
	info->SaveGameLen[0] = 32 * 1024;
	info->Power = M513Power;
	AddExState(&reg, 1, 0, "EXPR");
}
