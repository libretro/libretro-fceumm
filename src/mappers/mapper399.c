/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
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

#include "mapinc.h"
#include "mmc3.h"

static uint8 prg[2];
static uint8 chr[2];

static SFORMAT StateRegs[] = {
	{ prg, 2, "PREG" },
    { chr, 2, "CREG" },
	{ 0 }
};

static void M399PRG(void) {
	if (iNESCart.submapper == 1) {
		setprg8(0x6000, 0xFE);
		setprg8(0x8000, prg[0] << 1 | 0);
		setprg8(0xA000, prg[0] << 1 | 1);
		setprg8(0xC000, prg[1]);
		setprg8(0xE000, 0xFF);
	} else {
		setprg8(0x8000, 0x00);
		setprg8(0xA000, prg[0]);
		setprg8(0xC000, prg[1]);
		setprg8(0xE000, 0xFF);
	}
}

static void M399CHR(void) {
	setchr4(0x0000, chr[0]);
	setchr4(0x1000, chr[1]);
}

static DECLFW(M399WriteReg) {
    if (A & 0x01) {
        prg[V >> 7] = V;
		MMC3_FixPRG();
    } else {
        chr[V >> 7] = V;
		MMC3_FixCHR();
    }
}

static DECLFW(M399Write1) {
	MMC3_Write(0x2000 + A, V);
}

static void M399Power(void) {
	prg[0] = chr[0] = 0;
    prg[1] = chr[1] = 1;
	MMC3_Power();
    if (iNESCart.submapper == 1) {
		SetReadHandler(0x6000, 0x7FFF, CartBR);
	    SetWriteHandler(0x8000, 0xDFFF, M399Write1);
		SetWriteHandler(0xE000, 0xFFFF, M399WriteReg);
    } else {
        SetWriteHandler(0x8000, 0x9FFF, M399WriteReg);
    }
}

void Mapper399_Init(CartInfo *info) {
	MMC3_Init(info, 0, 0);
    MMC3_FixPRG = M399PRG;
    MMC3_FixCHR = M399CHR;
	info->Power = M399Power;
	AddExState(StateRegs, ~0, 0, NULL);
}
