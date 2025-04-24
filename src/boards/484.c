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

static void PRGWrap(uint32 A, uint8 V) {
	int prgAND =EXPREGS[0] &0x80? 0x1F: 0x3F;
	int prgOR  =EXPREGS[0] <<5 &0x20;
	setprg8(A, V &prgAND | prgOR &~prgAND);
}

static DECLFW(Write) {
	if (~EXPREGS[0] &0x80) {
		EXPREGS[0] =V;
		FixMMC3PRG(MMC3_cmd);
	}
}

static void Power(void) {
	EXPREGS[0] =0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x5FFF, Write);
}

void Mapper484_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, info->iNES2? (info->PRGRamSize + info->PRGRamSaveSize) /1024: 8, info->battery);
	pwrap = PRGWrap;
	info->Power = Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
