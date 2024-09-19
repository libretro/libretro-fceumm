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

/* iNES Mapper 49 */
/* BMC-STREETFIGTER-GAME4IN1 - Sic. $6000 set to $41 rather than $00 on power-up. */

#include "mapinc.h"
#include "mmc3.h"

static uint8 reg;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M049PW(uint16 A, uint16 V) {
	if (reg & 0x01) {
		setprg8(A, ((reg >> 2) & ~0x0F) | (V & 0x0F));
	} else {
		setprg32(0x8000, (reg >> 4) & 0x03);
	}
}

static void M049CW(uint16 A, uint16 V) {
	setchr1(A, ((reg << 1) & 0x180) | (V & 0x7F));
}

static DECLFW(M049Write) {
	if (MMC3_WramIsWritable()) {
		reg = V;
		MMC3_FixPRG();
		MMC3_FixCHR();
	}
}

static void M049Reset(void) {
	reg = ((iNESCart.submapper == 1) || (iNESCart.PRGCRC32 == 0x408EA235)) ? 0x41 : 0x00; /* Street Fighter II Game 4-in-1 */
	MMC3_Reset();
}

static void M049Power(void) {
	reg = ((iNESCart.submapper == 1) || (iNESCart.PRGCRC32 == 0x408EA235)) ? 0x41 : 0x00; /* Street Fighter II Game 4-in-1 */
	MMC3_Power();
	SetWriteHandler(0x6000, 0x7FFF, M049Write);
}

void Mapper049_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_cwrap = M049CW;
	MMC3_pwrap = M049PW;
	info->Reset = M049Reset;
	info->Power = M049Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
