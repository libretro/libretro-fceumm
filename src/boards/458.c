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

static void M458CW(uint32 A, uint8 V) {
	setchr1(A, ((EXPREGS[0] << 4) & ~0x7F) | (V & 0x7F));
}

static void M458PW(uint32 A, uint8 V) {
	if (EXPREGS[0] & 0x10) {
		setprg32(0x8000, EXPREGS[0] >> 1);
	} else {
		setprg16(0x8000, EXPREGS[0]);
		setprg16(0xC000, EXPREGS[0]);
	}
}

static DECLFR(M458Read) {
	if ((EXPREGS[0] & 0x20) && (EXPREGS[1] & 3)) {
		return CartBR((A & ~3) | (EXPREGS[1] & 3));
	}
	return CartBR(A);
}

static DECLFW(M458Write) {
	if (MMC3CanWriteToWRAM()) {
		EXPREGS[0] = A & 0xFF;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void M458Reset(void) {
	EXPREGS[0] = 0;
	EXPREGS[1]++;
	MMC3RegReset();
}

static void M458Power(void) {
	EXPREGS[0] = 0;
	EXPREGS[1] = 0;
	GenMMC3Power();
	SetReadHandler(0x8000, 0xFFFF, M458Read);
	SetWriteHandler(0x6000, 0x7FFF, M458Write);
}

void Mapper458_Init(CartInfo *info) {
	GenMMC3_Init(info, 256, 256, 0, 0);
	cwrap = M458CW;
	pwrap = M458PW;
	info->Reset = M458Reset;
	info->Power = M458Power;
	AddExState(EXPREGS, 2, 0, "EXPR");
}
