/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 *
 */

#include "mapinc.h"
#include "mmc3.h"

static void M412PW(uint32 A, uint8 V) {
	int prgAND  =0x0F | (EXPREGS[1] &0x02? 0x00: 0x20) | (EXPREGS[1] &0x10? 0x00: 0x10);
	int prgOR   =(EXPREGS[1] &0x40? 0x10: 0x00) | (EXPREGS[1] &0x04? 0x20: 0x00);
	if (EXPREGS[2] &0x02) { /* NROM mode */
		if (EXPREGS[2] &0x04) /* NROM-256 */
			setprg32(0x8000, EXPREGS[2] >>4);
		else {
			setprg16(0x8000, EXPREGS[2] >>3);
			setprg16(0xC000, EXPREGS[2] >>3);
		}
	} else
		setprg8(A, V &prgAND | prgOR &~prgAND);
}

static void M412CW(uint32 A, uint8 V) {
	int chrAND  =EXPREGS[1] &0x20? 0x7F: 0xFF;
	int chrOR   =(EXPREGS[1] &0x80? 0x80: 0x00) | (EXPREGS[1] &0x08? 0x100: 0x000);
	if (EXPREGS[2] &0x02) /* (C)NROM mode */
		setchr8(EXPREGS[0] >>2);
	else
		setchr1(A, (V & chrAND) | (chrOR & ~chrAND));
}

static DECLFR(M412Read) {
	return EXPREGS[4];
}

static DECLFW(M412Write) {
	if (~EXPREGS[1] &0x01) {
		EXPREGS[A & 3] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
	CartBW(A, V);
}

static void M412Reset(void) {
	EXPREGS[0] =EXPREGS[1] =EXPREGS[2] =EXPREGS[3] =0;
	EXPREGS[4]++;
	MMC3RegReset();
}

static void M412Power(void) {
	EXPREGS[0] =EXPREGS[1] =EXPREGS[2] =EXPREGS[3] =EXPREGS[4] =0;
	GenMMC3Power();
	SetReadHandler(0x5000, 0x5FFF, M412Read);
	SetWriteHandler(0x6000, 0x7FFF, M412Write);
}

void Mapper412_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 0, 0);
	cwrap = M412CW;
	pwrap = M412PW;
	info->Power = M412Power;
	info->Reset = M412Reset;
	AddExState(EXPREGS, 5, 0, "EXPR");
}
