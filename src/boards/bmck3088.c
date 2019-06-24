/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2019 Libretro Team
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

/* NES 2.0 Mapper 287
 * similar to BMC-411120-C but without jumper or dipswitch */

#include "mapinc.h"
#include "mmc3.h"

static void BMCK3088CW(uint32 A, uint8 V) {
	if (CHRptr[EXPREGS[0] & 7])
		setchr1r(EXPREGS[0] & 7, A, V);
	else
		setchr1(A, V | ((EXPREGS[0] & 3) << 7));
}

static void BMCK3088PW(uint32 A, uint8 V) {
	if (PRGptr[EXPREGS[0] & 7]) {
		uint8 chip = EXPREGS[0] & 7;
		if (EXPREGS[0] & 8)
			setprg32r(chip, 0x8000, ((EXPREGS[0] >> 4) & 3));
		else
			setprg8r(chip, A, (V & 0x0F));
	} else {
		if (EXPREGS[0] & 8)
			setprg32(0x8000, ((EXPREGS[0] >> 4) & 3) | (0x0C));
		else
			setprg8(A, (V & 0x0F) | ((EXPREGS[0] & 3) << 4));
	}
}

static DECLFW(BMCK3088LoWrite) {
	EXPREGS[0] = A;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void BMCK3088Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void BMCK3088Power(void) {
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, BMCK3088LoWrite);
}

void BMCK3088_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 128, 8, 0);
	pwrap = BMCK3088PW;
	cwrap = BMCK3088CW;
	info->Power = BMCK3088Power;
	info->Reset = BMCK3088Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
