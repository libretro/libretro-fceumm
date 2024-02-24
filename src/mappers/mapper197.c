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

static void M197PW(uint16 A, uint16 V) {
	uint8 mask = (reg & 0x08) ? 0x0F : 0x1F;

	switch (iNESCart.submapper) {
	case 3:
		setprg8(A, (reg << 4) | (V & mask));
		break;
	default:
		setprg8(A, V & 0x3F);
		break;
	}
}

static void M197CHR(void) {
	switch (iNESCart.submapper) {
	case 1:
		setchr2(0x0000, mmc3.reg[1] & ~0x01);
		setchr2(0x0800, mmc3.reg[1] |  0x01);
		setchr2(0x1000, mmc3.reg[4]);
		setchr2(0x1800, mmc3.reg[5]);
		break;
	case 2:
		setchr2(0x0000, mmc3.reg[0] & ~0x01);
		setchr2(0x0800, mmc3.reg[1] |  0x01);
		setchr2(0x1000, mmc3.reg[2]);
		setchr2(0x1800, mmc3.reg[5]);
		break;
	case 0:
	case 3:
	default:
		setchr2(0x0000, mmc3.reg[0] & ~0x01);
		setchr2(0x0800, mmc3.reg[0] |  0x01);
		setchr2(0x1000, mmc3.reg[2]);
		setchr2(0x1800, mmc3.reg[3]);
		break;
	}
}

static DECLFW(M197WriteReg) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
	}
}

static DECLFW(M197Write) {
	switch (A & 0xE001) {
	case 0x8001:
		switch (mmc3.cmd & 0x07) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			mmc3.reg[mmc3.cmd & 0x07] = V;
			MMC3_FixCHR();
			break;
		default:
			MMC3_CMDWrite(A, V);
			break;
		}
		break;
	default:
		MMC3_CMDWrite(A, V);
		break;
	}
}

static void M197Reset(void) {
	reg = 0;
	MMC3_FixCHR();
	MMC3_FixPRG();
	MMC3_FixMIR();
}

static void M197Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M197WriteReg);
	SetWriteHandler(0x8000, 0x9FFF, M197Write);
}

void Mapper197_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	info->Power = M197Power;
	info->Reset = M197Reset;
	MMC3_FixCHR = M197CHR;
	MMC3_pwrap = M197PW;
	AddExState(StateRegs, ~0, 0, NULL);
}
