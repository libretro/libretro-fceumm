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
 *
 * 8-in-1  Rockin' Kats, Snake, (PCB marked as "8 in 1"), similar to 12IN1,
 * but with MMC3 on board, all games are hacked the same, Snake is buggy too!
 *
 * no reset-citcuit, so selected game can be reset, but to change it you must use power
 *
 */

/* NES 2.0 Mapper 512 is used for 中國大亨 (Zhōngguó Dàhēng) */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static uint8 *CHRRAM     = NULL;
static uint32 CHRRAMSIZE = 8192;

static void M512MIR(void) {
	if (reg == 1) {
		SetupCartMirroring(4, 0, &CHRRAM[4096]);
	} else {
		setmirror((mmc3.mirr & 0x01) ^ 0x01);
	}
}

static void M512CW(uint16 A, uint16 V) {
	if (reg & 0x02) {
		setchr1r(0x10, A, (V & 0x03));
	} else {
		setchr1(A, V & 0xFF);
	}
}

static void M512PW(uint16 A, uint16 V) {
	setprg8(A, (V & 0x3F));
}

static DECLFW(M512Write) {
	if (A & 0x100) {
		reg = V & 0x03;
		MMC3_FixCHR();
		MMC3_FixMIR();
	}
}

static void M512Close(void) {
	MMC3_Close();
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
	}
	CHRRAM = NULL;
}

static void M512Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x4100, 0x4FFF, M512Write);
}

void Mapper512_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	MMC3_cwrap = M512CW;
	MMC3_pwrap = M512PW;
	MMC3_FixMIR = M512MIR;
	info->Power = M512Power;
	info->Close = M512Close;
	AddExState(&reg, 1, 0, "EXPR");

	CHRRAMSIZE = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
}
