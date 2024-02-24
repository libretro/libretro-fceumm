/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
 *  Copyright (C) 2023-2024 negativeExponent
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* NES 2.0 Mapper 516 - Brilliant Com Cocoma Pack */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static void M516CW(uint16 A, uint16 V) {
/*    FCEU_printf("CHR: A:%04x V:%02x R0:%02x\n", A, V, reg); */
	setchr1(A, ((reg << 5) & 0x180) | (V & 0x7F));
}

static void M516PW(uint16 A, uint16 V) {
/*    FCEU_printf("PRG: A:%04x V:%02x R0:%02x\n", A, V, reg); */
	setprg8(A, ((reg << 4) & 0x30) | (V & 0x0F));
}

static DECLFW(M516Write) {
/*    FCEU_printf("Wr: A:%04x V:%02x R0:%02x\n", A, V, reg); */
	if (A & 0x10) {
		reg = A & 0x0F;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
	MMC3_Write(A, V);
}

static void M516Power(void) {
	reg = 0;
	MMC3_Power();
	SetWriteHandler(0x8000, 0xFFFF, M516Write);
}

void Mapper516_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M516CW;
	MMC3_pwrap = M516PW;
	info->Power = M516Power;
	AddExState(&reg, 1, 0, "EXPR");
}
