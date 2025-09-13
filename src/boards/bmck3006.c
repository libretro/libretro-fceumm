/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright (C) 2019 Libretro Team
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
 */

/* NES 2.0 mapper 339 is used for a 21-in-1 multicart.
 * Its UNIF board name is BMC-K-3006. 
 * http://wiki.nesdev.com/w/index.php/NES_2.0_Mapper_339
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 submapper; /* 0: K-3006, 1: unmarked, 2: TL 8058, 3: K-3091/GN-16 */

static DECLFR (readPad) {
	return CartBR(A &~3 | EXPREGS[2] &3);
}

static void BMCK3006CW(uint32 A, uint8 V) {
	setchr1(A, (V & 0x7F) | (EXPREGS[0] & 0x18) << 4);
}

static void BMCK3006PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x20) {				/* MMC3 mode */
		setprg8(A, (V & 0x0F) | (EXPREGS[0] & 0x18) << 1);
	} else {
		if (submapper ==0 && (EXPREGS[0] & 0x06) == 0x06 ||
		    submapper ==1 && EXPREGS[0] &4 ||
		    submapper ==2 && (EXPREGS[0] &1 || EXPREGS[1] &1) ||
		    submapper ==3 && EXPREGS[0] &0x18) { /* NROM-256 */
			setprg32(0x8000, (EXPREGS[0] >> 1) & 0x0F);
		} else { /* NROM-128 */
			setprg16(0x8000, EXPREGS[0] & 0x1F);
			setprg16(0xC000, EXPREGS[0] & 0x1F);
		}
	}
}

static DECLFW(BMCK3006Write) {
	EXPREGS[0] = A &0xFF;
	EXPREGS[1] = A >>8 &0xFF;
	FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
	if (submapper == 2 || submapper == 3) SetReadHandler(0x8000, 0xFFFF, EXPREGS[0] &0x80? readPad: CartBR);
}

static void BMCK3006Reset(void) {
	EXPREGS[0] = EXPREGS[1] = 0;
	EXPREGS[2]++;
	MMC3RegReset();
}

static void BMCK3006Power(void) {
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, BMCK3006Write);
}

void BMCK3006_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 512, 512, 8, 0);
	pwrap = BMCK3006PW;
	cwrap = BMCK3006CW;
	info->Power = BMCK3006Power;
	info->Reset = BMCK3006Reset;
	AddExState(EXPREGS, 3, 0, "EXPR");
}
