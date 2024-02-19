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

/* NES 2.0 Mapper 398 - PCB YY840820C
 * 1995 Super HiK 5-in-1 - 新系列米奇老鼠組合卡 (JY-048)
 */

#include "mapinc.h"
#include "vrc2and4.h"

static uint8 reg;
static uint8 PPUCHRBus;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &PPUCHRBus, 1, "PPUC" },
	{ 0 },
};

static void M398PW(uint16 A, uint16 V) {
	if (reg & 0x80) {
        /* GNROM-like */
		setprg32(0x8000, ((reg >> 5) & 0x06) | ((vrc24.chr[PPUCHRBus] >> 2) & 0x01));
	} else {
		setprg8(A, V & 0x0F);
	}
}

static void M398CW(uint16 A, uint16 V) {
	if (reg & 0x80) {
        /* GNROM-like */
		setchr8(0x40 | ((reg >> 3) & 0x08) | (vrc24.chr[PPUCHRBus] & 0x07));
	} else {
		setchr1(A, V & 0x1FF);
	}
}

static DECLFW(M398WriteLatch) {
	reg = A & 0xFF;
	VRC24_FixPRG();
	VRC24_FixCHR();
	VRC24_Write(A, V);
}

static void M398PPUHook(uint32 A) {
	uint8 bank = (A & 0x1FFF) >> 10;
	if ((PPUCHRBus != bank) && ((A & 0x3000) != 0x2000)) {
		PPUCHRBus = bank;
		VRC24_FixPRG();
		VRC24_FixCHR();
	}
}

static void M398Reset(void) {
	reg = 0xC0;
	VRC24_FixPRG();
	VRC24_FixCHR();
}

static void M398Power(void) {
	PPUCHRBus = 0;
    reg = 0xC0;
	VRC24_Power();
	SetWriteHandler(0x8000, 0xFFFF, M398WriteLatch);
}

void Mapper398_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x01, 0x02, 0, 1);
	info->Reset = M398Reset;
	info->Power = M398Power;
	PPU_hook = M398PPUHook;
	VRC24_pwrap = M398PW;
	VRC24_cwrap = M398CW;
	AddExState(StateRegs, ~0, 0, NULL);
}
