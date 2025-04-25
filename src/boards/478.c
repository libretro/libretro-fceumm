/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

/* WE7HGX. Very preliminary. */

#include "mapinc.h"
#include "mmc3.h"

static void PRGWrap(uint32 A, uint8 V) {
	int prgAND =EXPREGS[0] &8 && EXPREGS[0] &4? 0x03: 0x0F;
	setprg8(A, V &prgAND | EXPREGS[0] <<2 &~prgAND);
}

static void CHRWrap(uint32 A, uint8 V) {
	int chrAND =EXPREGS[0] &8 && EXPREGS[0] &4? 0x1F: 0x7F;
	setchr1(A, V &chrAND | EXPREGS[0] <<5 &~chrAND);
}

static DECLFW(WriteReg) {
	if (EXPREGS[1] &0x60) {
		EXPREGS[0] =A &0xFF;
		EXPREGS[1] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static DECLFW(WriteASIC) {
	if (EXPREGS[1] &0x80) {
		if (A &0x4000)
			MMC3_IRQWrite(A, V);
		else
			MMC3_CMDWrite(A, V);
	} else {
		int i;
		for (i =0; i <6; i++) DRegBuf[i] =DRegBuf[i] &~0x18 | V <<3 &0x18;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void Power(void) {
	EXPREGS[0] =0;
	EXPREGS[1] =0xF0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, WriteASIC);
}

static void Reset(void) {
	EXPREGS[0] =0;
	EXPREGS[1] =0xF0;
	MMC3RegReset();
}

void Mapper478_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, info->iNES2? (info->PRGRamSize + info->PRGRamSaveSize) /1024: 8, info->battery);
	pwrap = PRGWrap;
	cwrap = CHRWrap;
	info->Power = Power;
	info->Reset = Reset;
	AddExState(EXPREGS, 2, 0, "EXPR");
}
