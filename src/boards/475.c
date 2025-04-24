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
	if (EXPREGS[1] &1) {
		if ((EXPREGS[0] &3) ==3)
			setprg32(0x8000, EXPREGS[0] <<2 &0xC | EXPREGS[0] >>2 &0x3);
		else
			setprg8(A, V &0x0F | EXPREGS[0] <<4 &0xF0);
	} else
		setprg32(0x8000, EXPREGS[0] >>4 &0x3 | 0x10);
}

static void CHRWrap(uint32 A, uint8 V) {
	if (EXPREGS[1] &1)
		setchr1(A, V &0x7F | EXPREGS[0] <<7 &0x180);
	else
		setchr1(A, V &0x7F | 0x200);
}

static DECLFW(WriteExtra) {
	if (EXPREGS[1] &1)
		EXPREGS[0] =A &0xFF;
	else
		EXPREGS[0] =V;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void Reset(void) {
	EXPREGS[0] =0;
	EXPREGS[1] ^=1;
	MMC3RegReset();
}

static void Power(void) {
	EXPREGS[0] =0;
	EXPREGS[1] =0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, WriteExtra);
}

void Mapper475_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 256, 0, 0);
	cwrap = CHRWrap;
	pwrap = PRGWrap;
	info->Power = Power;
	info->Reset = Reset;
	AddExState(EXPREGS, 2, 0, "EXPR");
}
