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

/* NC3000M PCB */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;
static uint8 dipsw;

static void M443PW(uint16 A, uint16 V) {
	uint8 mask = 0x0F;
	uint8 base = ((reg << 4) & 0x20) | (reg & 0x10);

	if (reg & 0x04) { /* NROM */
		uint16 bank = (base & ~mask) | (mmc3.reg[6] & mask);
		if (reg & 0x08) { /* NROM-128 */
			setprg16(0x8000, bank >> 1);
			setprg16(0xC000, bank >> 1);
		} else { /* NROM-256 */
			setprg32(0x8000, bank >> 2);
		}
	} else { /*  MMC3 */
		setprg8(A, (base & ~mask) | (V & mask));
	}
}

static void M443CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 8) & ~0xFF) | (V & 0xFF));
}

static DECLFR(M443Read) {
	return (((reg & 0x0C) == 0x08) ? dipsw : CartBR(A));
}

static DECLFW(M443Write) {
	reg = A & 0xFF;
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static void M443Reset(void) {
	dipsw++;
	dipsw &= 15;
	reg = 0;
	MMC3_Reset();
}

static void M443Power(void) {
	dipsw = 0;
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M443Write);
	SetReadHandler(0x8000, 0xFFFF, M443Read);
}

void Mapper443_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M443CW;
	MMC3_pwrap = M443PW;
	info->Power = M443Power;
	info->Reset = M443Reset;
	AddExState(&reg, 1, 0, "EXPR");
	AddExState(&dipsw, 1, 0, "DIPS");
}
