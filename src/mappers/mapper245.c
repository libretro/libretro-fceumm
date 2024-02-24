/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

static uint8 reg;

static SFORMAT StateRegs[] = {
    { &reg, 1, "REGS" },
    { 0 }
};

static void M245PW(uint16 A, uint16 V) {
	setprg8(A, ((mmc3.reg[0] & 0x02) << 5) | (V & 0x3F));
}

static void M245CW(uint16 A, uint16 V) {
    setchr8(0);
}

static DECLFW(M245Write) {
    if (A & 0x01) {
        mmc3.reg[mmc3.cmd & 0x07] = V;
    } else {
        mmc3.cmd = V;
    }
    MMC3_FixPRG();
    MMC3_FixCHR();
}

static void M245Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x8000, 0x9FFF, M245Write);
}

void Mapper245_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	info->Power = M245Power;
	MMC3_pwrap = M245PW;
	MMC3_cwrap = M245CW;
	AddExState(StateRegs, ~0, 0, NULL);
}
