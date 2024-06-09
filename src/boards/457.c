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

static void M457CW(uint32 A, uint8 V) {
	uint32 mask = (EXPREGS[0] & 8) ? 0xFF : 0x7F;
	setchr1(A, ((EXPREGS[0] << 7) & ~mask) | (V & mask));
}

static void M457PW(uint32 A, uint8 V) {
	uint32 mask = (EXPREGS[0] & 8) ? 0x1F : 0x0F;
	setprg8(A, (((EXPREGS[0] & 7) << 4) & ~mask) | (V & mask));
}

static DECLFW(M457Write) {
	if (MMC3CanWriteToWRAM()) {
		EXPREGS[0] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void M457Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void M457Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M457Write);
}

void Mapper457_Init(CartInfo *info) {
	GenMMC3_Init(info, 128, 256, 0, 0);
	cwrap = M457CW;
	pwrap = M457PW;
	info->Reset = M457Reset;
	info->Power = M457Power;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
