/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
static uint8 extMode;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ &extMode, 1, "MODE" },
	{ 0 }
};

static void M219PW(uint16 A, uint16 V) {
	setprg8(A, (reg << 4) | (V & 0x0F));
}

static void M219CW(uint16 A, uint16 V) {
	setchr1(A, (reg << 7) | (V & 0x7F));
}

static DECLFW(M219WriteOuter) {
	switch (A & 0x01) {
	case 0:
		reg = (reg & ~0x01) | ((V >> 3) & 0x01);
		break;
	case 1:
		reg = (reg & ~0x02) | ((V >> 4) & 0x02);
		break;
	}
	MMC3_FixPRG();
	MMC3_FixCHR();
}

static DECLFW(M219WriteASIC) {
	uint8 oldcmd = mmc3.cmd;

	if (!(A & 0x01)) { /* Register index */
		mmc3.cmd = V;
		if ((oldcmd & 0x40) != ((V & 0x40))) {
			MMC3_FixPRG();
		}
		if ((oldcmd & 0x80) != ((V & 0x80))) {
			MMC3_FixPRG();
		}
		if (A & 0x02) {
			extMode = (V & 0x20) != 0;
		}
	} else {
		if (!extMode) { /* Scrambled mode inactive */
			MMC3_CMDWrite(A, V);
		} else { /* Scrambled mode active */
			if ((mmc3.cmd >= 0x08) && (mmc3.cmd <= 0x1F)) { /* Scrambled CHR register */
				uint8 index = (mmc3.cmd - 8) >> 2;
				if (mmc3.cmd & 0x01) { /* LSB nibble */
					mmc3.reg[index] &= ~0x0F;
					mmc3.reg[index] |= ((V >> 1) & 0x0F);
				} else { /* MSB nibble */
					mmc3.reg[index] &= ~0xF0;
					mmc3.reg[index] |= ((V << 4) & 0xF0);
				}
				MMC3_FixCHR();
			} else if ((mmc3.cmd >= 0x25) && (mmc3.cmd <= 0x26)) { /* Scrambled PRG register */
				V = ((V << 1) & 0x08) | ((V >> 1) & 0x04) | ((V >> 3) & 0x02) | ((V >> 5) & 0x01);
				mmc3.reg[6 | (mmc3.cmd & 0x01)] = V;
				MMC3_FixPRG();
			}
		}
	}
}

static void M219Power(void) {
	extMode = FALSE;
	reg = 3;
	MMC3_Power();
	SetWriteHandler(0x5000, 0x5FFF, M219WriteOuter);
	SetWriteHandler(0x8000, 0x9FFF, M219WriteASIC);
}

static void M219Reset(void) {
	extMode = FALSE;
	reg = ~0;
	MMC3_Reset();
}

void Mapper219_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
	MMC3_pwrap = M219PW;
	MMC3_cwrap = M219CW;
	info->Power = M219Power;
	info->Reset = M219Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
