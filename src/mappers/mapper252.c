/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
#include "vrc2and4.h"

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSIZE;

static uint8 maskCHRCompare;
static uint8 maskCHRBank;

static writefunc writePPU;
extern uint32 RefreshAddr;

static void M252PW(uint16 A, uint16 V) {
	setprg8(A, V & 0x1F);
}

static void M252CW(uint16 A, uint16 V) {
	if ((V & maskCHRBank) == maskCHRCompare) {
		setchr1r(0x10, A, V & 0x01);
	} else {
		setchr1(A, V);
	}
}

static DECLFW(M252PPU_B2007) {
	if (RefreshAddr < 0x2000) {
		switch (vrc24.chr[RefreshAddr >> 10]) {
		case 0x88:
			maskCHRBank = 0xFC;
			maskCHRCompare = 0x4C;
			break;
		case 0xC2:
			maskCHRBank = 0xFE;
			maskCHRCompare = 0x7C;
			break;
		case 0xC8:
			maskCHRBank = 0xFE;
			maskCHRCompare = 0x04;
			break;
		}
	}
	writePPU(A, V);
}

static void M252Close(void) {
	VRC24_Close();
	if (CHRRAM) {
		FCEU_free(CHRRAM);
		CHRRAM = NULL;
	}
}

static void M252Power(void) {
	if (iNESCart.mapper == 252) {
		maskCHRBank = 0xFE;
		maskCHRCompare = 0x06;
	} else {
		maskCHRBank = 0xFE;
		maskCHRCompare = 0x04;
	}
	VRC24_Power();

	writePPU = GetWriteHandler(0x2007);
	SetWriteHandler(0x2007, 0x2007, M252PPU_B2007);
}

void Mapper252_Init(CartInfo *info) {
	VRC24_Init(info, VRC4, 0x04, 0x08, TRUE, TRUE);
	VRC24_pwrap = M252PW;
	VRC24_cwrap = M252CW;

	info->Power = M252Power;
	info->Close = M252Close;

	CHRRAMSIZE = 2048;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
	AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
}
