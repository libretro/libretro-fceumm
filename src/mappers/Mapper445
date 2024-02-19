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

/* NES 2.0 Mapper 445
 * DG574B MMC3-compatible multicart circuit board.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];

static void M445PW(uint16 A, uint16 V) {
    uint8 mask = (reg[2] & 0x01) ? 0x0F : 0x1F;
	uint8 base = reg[0];

    setprg8(A, (base & ~mask) | (V & mask));
}

static void M445CW(uint16 A, uint16 V) {
    uint16 mask = (reg[2] & 0x08) ? 0x7F : 0xFF;
	uint16 base = reg[1] << 3;

	setchr1(A, (base & ~mask) | (V & mask));
}

static DECLFW(M445Write) {
    if (!(reg[3] & 0x20)) {
        reg[A & 0x03] = V;
        MMC3_FixPRG();
        MMC3_FixCHR();
    }
}

static void M445Reset(void) {
	reg[0] = 0x00;
	reg[1] = 0x00;
	reg[2] = 0x00;
    reg[3] = 0x00;
	MMC3_Reset();
}

static void M445Power(void) {
    reg[0] = 0x00;
	reg[1] = 0x00;
	reg[2] = 0x00;
    reg[3] = 0x00;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M445Write);
}

void Mapper445_Init(CartInfo *info) {
	MMC3_Init(info, info->PRGRamSize + info->PRGRamSaveSize, info->battery);
	MMC3_pwrap = M445PW;
	MMC3_cwrap = M445CW;
	info->Power = M445Power;
	info->Reset = M445Reset;
	AddExState(reg, 4, 0, "EXPR");
}
