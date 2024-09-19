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

/* NES 2.0 Mapper 345
 * BMC-L6IN1
 * New Star 6-in-1 Game Cartridge
 * https://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_345
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M345PW(uint16 A, uint16 V) {
	uint8 base = reg >> 6;
	
	if (reg & 0x0C) {
		setprg8(A, (base << 4) | (V & 0x0F));
	} else {
		setprg32(0x8000, (base << 2) | (reg & 0x03));
	}
}

static void M345MIR(void) {
	if (reg & 0x20) {
		setmirror(MI_0 + ((reg & 0x10) >> 1));
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static DECLFW(M345Write) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	}
}

static void M345Reset(void) {
	reg = 0;
	MMC3_Reset();
}

static void M345Power(void) {
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M345Write);
}

void Mapper345_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M345PW;
	MMC3_FixMIR = M345MIR;
	info->Power = M345Power;
	info->Reset = M345Reset;
	AddExState(&reg, 1, 0, "EXPR");
}
