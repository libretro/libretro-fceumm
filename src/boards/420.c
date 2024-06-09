/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

static void M420CW(uint32 A, uint8 V) {
	uint8 mask = (EXPREGS[1] & 0x80) ? 0x7F : 0xFF;
	setchr1(A, ((EXPREGS[1] << 1) & 0x100) | ((EXPREGS[1] << 5) & 0x80) | (V & mask));
}

static void M420PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x80) {
		setprg32(0x8000, ((EXPREGS[2] >> 2) & 8) | ((EXPREGS[0] >> 1) & 7));
	} else {
		uint8 mask = (EXPREGS[0] & 0x20) ? 0x0F : ((EXPREGS[3] & 0x20) ? 0x1F : 0x3F);
		setprg8(A, ((EXPREGS[3] << 3) & 0x20) | (V & mask));
	}
}

static DECLFW(M420Write) {
	/* writes possible regardless of MMC3 wram state */
	EXPREGS[A & 3] = V;
	FixMMC3PRG(MMC3_cmd);        
	FixMMC3CHR(MMC3_cmd);
}

static void M420Reset(void) {
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	MMC3RegReset();
}

static void M420Power(void) {
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M420Write);
}

void Mapper420_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 0, 0);
	cwrap = M420CW;
	pwrap = M420PW;
	info->Reset = M420Reset;
	info->Power = M420Power;
	AddExState(EXPREGS, 4, 0, "EXPR");
}
