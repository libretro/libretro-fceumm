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

static void M467PW(uint16 A, uint16 V) {
	if (reg & 0x20) {
		uint8 mask = (reg & 0x40) ? 0x0F : 0x03;
		uint8 base = reg << 1;
		
		setprg8(A, (base & ~mask) | (V & mask));
	} else {
		setprg16(0x8000, reg & 0x1F);
		setprg16(0xC000, reg & 0x1F);
	}
}

static void M467CHR(void) {
	uint16 base = (reg << 2) & 0x100;

	if (reg & 0x40) {
		setchr2(0x0000, base | (mmc3.reg[0] & ~0x01));
		setchr2(0x0800, base | (mmc3.reg[0] |  0x01));
		setchr2(0x1000, base | mmc3.reg[2]);
		setchr2(0x1800, base | mmc3.reg[3]);
	} else {
		setchr2(0x0000, base | (mmc3.reg[0] & ~0x03) | 0);
		setchr2(0x0800, base | (mmc3.reg[0] & ~0x03) | 1);
		setchr2(0x1000, base | (mmc3.reg[2] & ~0x03) | 2);
		setchr2(0x1800, base | (mmc3.reg[3] & ~0x03) | 3);
	}
}

static void M467MIR(void) {
	setmirror(((reg >> 7) & 0x01) ^ 0x01);
}

static DECLFW(M467Write) {
	switch (A & 0xF000) {
	case 0x9000:
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
		break;
	default:
		switch (A & 0xE001) {
		case 0x8000:
			mmc3.cmd = V & 0x3F;
			break;
		case 0x8001:
			mmc3.reg[mmc3.cmd & 0x07] = V;
			if (mmc3.cmd < 6) MMC3_FixCHR();
			else MMC3_FixPRG();
			break;
		case 0xA000:
			break;
		}
	}
}


static void M467Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M467Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x8000, 0xBFFF, M467Write);
}

void Mapper467_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_FixCHR = M467CHR;
	MMC3_FixMIR = M467MIR;
	MMC3_pwrap = M467PW;
	info->Power = M467Power;
	info->Reset = M467Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
