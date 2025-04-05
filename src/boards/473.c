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

static void Mapper473_PRGWrap(uint32 A, uint8 V) {
	int prgAND =EXPREGS[0] &0x20? (0xFF >> (7 -(EXPREGS[0] &7))): 0x00;
	int prgOR  =EXPREGS[0] &0x20? (EXPREGS[1] | EXPREGS[2] <<8): 0x3F;
	setprg8(A, V &prgAND | prgOR &~prgAND);
	SetWriteHandler(0x8000, 0xBFFF, EXPREGS[0] &0x40? CartBW: MMC3_CMDWrite);
	SetWriteHandler(0xC000, 0xFFFF, EXPREGS[0] &0x40? CartBW: MMC3_IRQWrite);
}

static void Mapper473_CHRWrap(uint32 A, uint8 V) {
	if (~EXPREGS[0] &0x80)
		setchr8(EXPREGS[3]);
	else
		setchr1(A, V);
}

static DECLFW(Mapper473_WriteReg) {
	EXPREGS[A &3] =V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void Mapper473_Reset(void) {
	EXPREGS[0] =EXPREGS[1] =EXPREGS[2] =EXPREGS[3] =0;
	MMC3RegReset();
}

static void Mapper473_Power(void) {	
	EXPREGS[0] =EXPREGS[1] =EXPREGS[2] =EXPREGS[3] =0;
	GenMMC3Power();
	SetWriteHandler(0x4800, 0x4FFF, Mapper473_WriteReg);
}

void Mapper473_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, (info->PRGRamSize +info->PRGRamSaveSize) >>10, info->battery);
	cwrap = Mapper473_CHRWrap;
	pwrap = Mapper473_PRGWrap;
	info->Power = Mapper473_Power;
	info->Reset = Mapper473_Reset;
	AddExState(EXPREGS, 4, 0, "EXPR");
}
