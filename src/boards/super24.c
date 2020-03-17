/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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

/* BMC-Super24in1SC03, basically is just a duplicate of BMC-FK23C (mapper 176)
 * This should be merge with the said mapper soon... */

#include "mapinc.h"
#include "mmc3.h"

static uint8 *CHRRAM = NULL;
static const int masko8[8] = { 63, 31, 15, 1, 3, 0, 0, 0 };

static void Super24PW(uint32 A, uint8 V) {
	setprg8(A, (EXPREGS[1] << 1) | (V & masko8[EXPREGS[0] & 0x7]));
}

static void Super24CW(uint32 A, uint8 V) {
	setchr1r((EXPREGS[0] & 0x20) >> 1, A, (EXPREGS[2] << 3) | V);
}

static DECLFW(Super24Write) {
	switch (A & 0xF003) {
	case 0x5000:
		if (EXPREGS[0] != V) {
			EXPREGS[0] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
		}
		break;
	case 0x5001:
		if (EXPREGS[1] != V) {
			EXPREGS[1] = V;
			FixMMC3PRG(MMC3_cmd);
		}
		break;
	case 0x5002:
		if (EXPREGS[2] != V) {
			EXPREGS[2] = V;
			FixMMC3CHR(MMC3_cmd);
		}
		break;
	}
}

static void Super24Power(void) {
	EXPREGS[0] = 0x24;
	EXPREGS[1] = 0x9F;
	EXPREGS[2] = 0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x7FFF, Super24Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void Super24Reset(void) {
	EXPREGS[0] = 0x24;
	EXPREGS[1] = 0x9F;
	EXPREGS[2] = 0;
	MMC3RegReset();
}

static void Super24Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	CHRRAM = NULL;
}

void Super24_Init(CartInfo *info) {
	GenMMC3_Init(info, 512, 512, 0, 0);
	info->Power = Super24Power;
	info->Reset = Super24Reset;
	info->Close = Super24Close;
	cwrap = Super24CW;
	pwrap = Super24PW;
	CHRRAM = (uint8*)FCEU_gmalloc(8192);
	SetupCartCHRMapping(0x10, CHRRAM, 8192, 1);
	AddExState(CHRRAM, 8192, 0, "CHRR");
	AddExState(EXPREGS, 3, 0, "BIG2");
}
