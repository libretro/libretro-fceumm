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

/* NES 2.0 Mapper 344
 * BMC-GN-26
 * Kuai Da Jin Ka Zhong Ji Tiao Zhan 3-in-1 (3-in-1,6-in-1,Unl)
 */

#include "mapinc.h"
#include "mmc3.h"

static void BMCGN26CW(uint32 A, uint8 V) {
	uint32 base = (EXPREGS[0] & 0x03) << 7;
	setchr1(A, base | (V & 0xFF));
}

static void BMCGN26PW(uint32 A, uint8 V) {
	/* Re-ordered -> 0:SF4 1:Contra Force 2:Revolution Hero */
	uint32 table[] = { 0, 0, 1, 2 };
	uint32 base = table[(EXPREGS[0] & 0x03)];

	if (EXPREGS[0] & 4) {
		if (A == 0x8000)
			setprg32(0x8000, (base << 2) | (V >> 2));
	} else
		setprg8(A, (base << 4) | (V & 0x0F));
}

static DECLFW(BMCGN26Write) {
	EXPREGS[0] = A & 0x0F;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void BMCGN26Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void BMCGN26Power(void) {
	GenMMC3Power();
	SetWriteHandler(0x6800, 0x68FF, BMCGN26Write);
}

void BMCGN26_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 256, 1, 0);
	pwrap = BMCGN26PW;
	cwrap = BMCGN26CW;
	info->Power = BMCGN26Power;
	info->Reset = BMCGN26Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
