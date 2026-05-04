/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

/* 850335C PCB (submapper 0) 
   840415C/43-170 PCB (submapper 1) */

#include "mapinc.h"
#include "mmc3.h"

static uint8_t submapper;

static void Mapper441_PRGWrap(uint32_t A, uint8_t V) {
	int prgAND = EXPREGS[0] &0x08? 0x0F: 0x1F;
	int prgOR  = EXPREGS[0] <<4 &0x30;
	if (EXPREGS[0] &0x04) {
		if (~A &0x4000) {
			setprg8(A,         ~2 &V &prgAND | prgOR &~prgAND);
			setprg8(A |0x4000,  2 |V &prgAND | prgOR &~prgAND);
		}
	} else
		setprg8(A, V &prgAND | prgOR &~prgAND);
}

static void Mapper441_CHRWrap(uint32_t A, uint8_t V) {
	int chrAND = EXPREGS[0] &0x40? 0x7F: 0xFF;
	int chrOR  = EXPREGS[0] <<3 &0x180;
	setchr1(A, V &chrAND | chrOR &~chrAND);
}

static DECLFW(Mapper441_Write) {
	if (~EXPREGS[0] &0x80) {
		if (submapper == 1)
			EXPREGS[0] = V &~4 | A &4;
		else
			EXPREGS[0] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
	CartBW(A, V);
}

static void Mapper441_Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void Mapper441_Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, Mapper441_Write);
}

void Mapper441_Init(CartInfo *info) {
	submapper = info->submapper;
	GenMMC3_Init(info, 256, 256, 8, 0);
	cwrap = Mapper441_CHRWrap;
	pwrap = Mapper441_PRGWrap;
	info->Power = Mapper441_Power;
	info->Reset = Mapper441_Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
