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

#include "mapinc.h"
#include "mmc3.h"

static uint8 submapper;

static void PRGWrap(uint32 A, uint8 V) {
	if (submapper ==1) {
		setprg8(0x6000, ~1);
		setprg8(0x8000, DRegBuf[6] <<1 |0);
		setprg8(0xA000, DRegBuf[6] <<1 |1);
	} else {
		setprg8(0x8000, 0);
		setprg8(0xA000, DRegBuf[6]);		
	}
	setprg8(0xC000, DRegBuf[7]);
	setprg8(0xE000, ~0);
}

static void CHRWrap(uint32 A, uint8 V) {
	setchr4(0x0000, DRegBuf[0]);
	setchr4(0x1000, DRegBuf[1]);
}

static DECLFW(writeReg) {
	if (submapper ==1) A =(A +0x2000) &0xFFFF | 0x8000;
	switch(A &0xE001) {
		case 0x8000:
			DRegBuf[V >>7 |0] =V;
			FixMMC3CHR(MMC3_cmd);
			break;
		case 0x8001:
			DRegBuf[V >>7 |6] =V;
			FixMMC3PRG(MMC3_cmd);
			break;
		case 0xA000: case 0xA001:
			MMC3_CMDWrite(A, V);
			break;
		case 0xC000: case 0xC001: case 0xE000: case 0xE001:
			MMC3_IRQWrite(A, V);
			break;
	}
}

static void power(void) {
	GenMMC3Power();
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeReg);
}

void Mapper399_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 256, 32, info->iNES2? (info->PRGRamSize + info->PRGRamSaveSize) /1024: 8, info->battery);
	cwrap =CHRWrap;
	pwrap =PRGWrap;
	info->Power =power;
}
