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

static void M044PW(uint16 A, uint16 V) {
    uint8 base = (mmc3.wram << 4) & 0x70;
    uint8 mask = ((mmc3.wram & 0x06) == 0x06) ? 0x1F : 0x0F;

    setprg8(A, (base & ~mask) | (V & mask));
}

static void M044CW(uint16 A, uint16 V) {
    uint16 base = (mmc3.wram << 7) & 0x380;
    uint16 mask = ((mmc3.wram & 0x06) == 0x06) ? 0xFF : 0x7F;

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW (M044WriteA000) {
    MMC3_CMDWrite(A, V);
    switch (A & 0xE001) {
    case 0xA001:
        MMC3_FixPRG();
        MMC3_FixCHR();
        break;
    }
}

static void M044Reset(void) {
    MMC3_Reset();
}

static void M044Power(void) {
	MMC3_Power();
    SetWriteHandler(0xA000, 0xBFFF, M044WriteA000);
}

void Mapper044_Init(CartInfo *info) {
	MMC3_Init(info, 8, info->battery);
	MMC3_cwrap = M044CW;
	MMC3_pwrap = M044PW;
	info->Power = M044Power;
    info->Reset = M044Reset;
}
