/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022
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

/* NES 2.0 Mapper 412 is a variant of mapper 45 where the
 * ASIC's PRG A21/CHR A20 output (set by bit 6 of the third write to $6000)
 * selects between regularly-banked CHR-ROM (=0) and 8 KiB of unbanked CHR-RAM (=1).
 * It is used solely for the Super 8-in-1 - 98格鬥天王＋熱血 (JY-302) multicart.
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg[4];

static void M412CW(uint16 A, uint16 V) {
	uint16 mask = (reg[1] & 0x20) ? 0x7F : 0xFF;
	uint16 base = ((reg[1] << 5) & 0x100) | (reg[1] & 0x80);
	uint16 bank = reg[0] >> 2;

    if (reg[2] & 0x02) {
        setchr8(bank);
    } else {
        setchr1(A, (base & ~mask) | (V & mask));
    }
}

static void M412PW(uint16 A, uint16 V) {
	uint16 mask = 0x3F & ~(((reg[1] << 1) & 0x20) | ((reg[1] << 3) & 0x10));
	uint16 base = ((reg[1] << 3) & 0x20) | ((reg[1] >> 2) & 0x10);
	uint16 bank = reg[2] >> 3;

    if (reg[2] & 0x02) {
        if (reg[2] & 0x04) {
            setprg32(0x8000, bank >> 1);
        } else {
            setprg16(0x8000, bank);
            setprg16(0xC000, bank);
        }
    } else {
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static DECLFW(M412Write) {
	if (MMC3_WramIsWritable()) {
		CartBW(A, V);
		if (!(reg[1] & 0x01)) {
			reg[A & 0x03] = V;
			MMC3_FixPRG();
			MMC3_FixCHR();
		}
	}
}

static void M412Reset(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Reset();
	mmc3.wram = 0x80;
}

static void M412Power(void) {
    reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_Power();
	mmc3.wram = 0x80;
	SetWriteHandler(0x6000, 0x7FFF, M412Write);
}

void Mapper412_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M412CW;
	MMC3_pwrap = M412PW;
	info->Reset = M412Reset;
	info->Power = M412Power;
	AddExState(reg, 4, 0, "EXPR");
}
