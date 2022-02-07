/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020 negativeExponent
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

static void M432CW(uint32 A, uint8 V) {
    if (EXPREGS[0] & 4)
        setchr1(A, (V & 0x7F) | ((EXPREGS[0] << 7) & 0x80) | ((EXPREGS[0] << 5) & 0x100));
    else
        setchr1(A, V | ((EXPREGS[0] << 5) & 0x100));
}

static void M432PW(uint32 A, uint8 V) {
    if (EXPREGS[0] & 4)
        setprg8(A, (V & 0x0F) | ((EXPREGS[0] << 4) & 0x10) | ((EXPREGS[0] << 1) & 0x20));
    else
        setprg8(A, (V & 0x1F) | ((EXPREGS[0] << 1) & 0x20));
}

static DECLFW(M432Write) {
    EXPREGS[0] = V;
    FixMMC3PRG(MMC3_cmd);
	FixMMC3CHR(MMC3_cmd);
}

static void M432Reset(void) {
	EXPREGS[0] = 0;
	MMC3RegReset();
}

static void M432Power(void) {
	EXPREGS[0] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, M432Write);
}

void Mapper432_Init(CartInfo *info) {
	GenMMC3_Init(info, 256, 256, 0, 0);
	cwrap = M432CW;
	pwrap = M432PW;
	info->Power = M432Power;
	info->Reset = M432Reset;
	AddExState(EXPREGS, 1, 0, "EXPR");
}
