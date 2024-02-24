/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

#include <string.h>
#include <stdlib.h>

#include <string/stdstring.h>
#include <file/file_path.h>
#include <streams/file_stream.h>

#include "fceu-types.h"
#include "fceu.h"
#include "ppu.h"

#include "cart.h"
#include "fceu-memory.h"
#include "x6502.h"

#include "general.h"

romData_t ROM;

/* This file contains all code for coordinating the mapping in of the
   address space external to the NES.
   It's also (ab)used by the NSF code.
 */

uint8 *Page[32], *VPage[8];
uint8 **VPageR = VPage;
uint8 *MMC5SPRVPage[8];
uint8 *MMC5BGVPage[8];

static uint8 PRGIsRAM[32];	/* This page is/is not PRG RAM. */

/* 16 are (sort of) reserved for UNIF/iNES and 16 to map other stuff. */
static uint8 CHRram[32];
static uint8 PRGram[32];

uint8 *PRGptr[32];
uint8 *CHRptr[32];

uint32 PRGsize[32];
uint32 CHRsize[32];

uint32 PRGmask2[32];
uint32 PRGmask4[32];
uint32 PRGmask8[32];
uint32 PRGmask16[32];
uint32 PRGmask32[32];

uint32 CHRmask1[32];
uint32 CHRmask2[32];
uint32 CHRmask4[32];
uint32 CHRmask8[32];

static INLINE void setpageptr(int s, uint16 A, uint8 *p, uint8 ram) {
	uint32 AB = A >> 11;
	int x;

	if (p) {
		for (x = (s >> 1) - 1; x >= 0; x--) {
			PRGIsRAM[AB + x] = ram;
			Page[AB + x] = p - A;
		}
	} else {
		for (x = (s >> 1) - 1; x >= 0; x--) {
			PRGIsRAM[AB + x] = 0;
			Page[AB + x] = 0;
		}
	}
}

static uint8 nothing[8192];
void ResetCartMapping(void) {
	int x;

	for (x = 0; x < 32; x++) {
		Page[x] = nothing - x * 2048;
		PRGptr[x] = CHRptr[x] = 0;
		PRGsize[x] = CHRsize[x] = 0;
	}
	for (x = 0; x < 8; x++) {
		MMC5SPRVPage[x] = MMC5BGVPage[x] = VPageR[x] = nothing - 0x400 * x;
	}
}

void SetupCartPRGMapping(int chip, uint8 *p, uint32 size, uint8 ram) {
	PRGptr[chip] = p;
	PRGsize[chip] = size;

	PRGmask2[chip] = (size >> 11) - 1;
	PRGmask4[chip] = (size >> 12) - 1;
	PRGmask8[chip] = (size >> 13) - 1;
	PRGmask16[chip] = (size >> 14) - 1;
	PRGmask32[chip] = (size >> 15) - 1;

	PRGram[chip] = ram ? TRUE : FALSE;
}

void SetupCartCHRMapping(int chip, uint8 *p, uint32 size, uint8 ram) {
	CHRptr[chip] = p;
	CHRsize[chip] = size;

	CHRmask1[chip] = (size >> 10) - 1;
	CHRmask2[chip] = (size >> 11) - 1;
	CHRmask4[chip] = (size >> 12) - 1;
	CHRmask8[chip] = (size >> 13) - 1;

	CHRram[chip] = ram ? TRUE : FALSE;
}

DECLFR(CartBR) {
	return Page[A >> 11][A];
}

DECLFW(CartBW) {
	if (PRGIsRAM[A >> 11] && Page[A >> 11])
		Page[A >> 11][A] = V;
}

DECLFR(CartBROB) {
	if (!Page[A >> 11]) {
		return(cpu.openbus);
	} else {
		return Page[A >> 11][A];
	}
}

void setprg2r(int r, uint16 A, uint16 V) {
	V &= PRGmask2[r];
	setpageptr(2, A, PRGptr[r] ? (&PRGptr[r][V << 11]) : 0, PRGram[r]);
}

void setprg2(uint16 A, uint16 V) {
	setprg2r(0, A, V);
}

void setprg4r(int r, uint16 A, uint16 V) {
	V &= PRGmask4[r];
	setpageptr(4, A, PRGptr[r] ? (&PRGptr[r][V << 12]) : 0, PRGram[r]);
}

void setprg4(uint16 A, uint16 V) {
	setprg4r(0, A, V);
}

void setprg8r(int r, uint16 A, uint16 V) {
	if (PRGsize[r] >= 8192) {
		V &= PRGmask8[r];
		setpageptr(8, A, PRGptr[r] ? (&PRGptr[r][V << 13]) : 0, PRGram[r]);
	} else {
		uint32 VA = V << 2;
		int x;
		for (x = 0; x < 4; x++) {
			setpageptr(2, A + (x << 11), PRGptr[r] ? (&PRGptr[r][((VA + x) & PRGmask2[r]) << 11]) : 0, PRGram[r]);
		}
	}
}

void setprg8(uint16 A, uint16 V) {
	setprg8r(0, A, V);
}

void setprg16r(int r, uint16 A, uint16 V) {
	if (PRGsize[r] >= 16384) {
		V &= PRGmask16[r];
		setpageptr(16, A, PRGptr[r] ? (&PRGptr[r][V << 14]) : 0, PRGram[r]);
	} else {
		uint32 VA = V << 3;
		int x;

		for (x = 0; x < 8; x++) {
			setpageptr(2, A + (x << 11), PRGptr[r] ? (&PRGptr[r][((VA + x) & PRGmask2[r]) << 11]) : 0, PRGram[r]);
		}
	}
}

void setprg16(uint16 A, uint16 V) {
	setprg16r(0, A, V);
}

void setprg32r(int r, uint16 A, uint16 V) {
	if (PRGsize[r] >= 32768) {
		V &= PRGmask32[r];
		setpageptr(32, A, PRGptr[r] ? (&PRGptr[r][V << 15]) : 0, PRGram[r]);
	} else {
		uint32 VA = V << 4;
		int x;

		for (x = 0; x < 16; x++) {
			setpageptr(2, A + (x << 11), PRGptr[r] ? (&PRGptr[r][((VA + x) & PRGmask2[r]) << 11]) : 0, PRGram[r]);
		}
	}
}

void setprg32(uint16 A, uint16 V) {
	setprg32r(0, A, V);
}

void setchr1r(int r, uint16 A, uint16 V) {
	if (!CHRptr[r]) {
		return;
	}
	FCEUPPU_LineUpdate();
	V &= CHRmask1[r];
	if (CHRram[r]) {
		PPUCHRRAM |= (1 << (A >> 10));
	} else {
		PPUCHRRAM &= ~(1 << (A >> 10));
	}
	VPageR[(A) >> 10] = &CHRptr[r][(V) << 10] - (A);
}

void setchr2r(int r, uint16 A, uint16 V) {
	if (!CHRptr[r]) {
		return;
	}
	FCEUPPU_LineUpdate();
	V &= CHRmask2[r];
	VPageR[(A) >> 10] = VPageR[((A) >> 10) + 1] = &CHRptr[r][(V) << 11] - (A);
	if (CHRram[r]) {
		PPUCHRRAM |= (3 << (A >> 10));
	} else {
		PPUCHRRAM &= ~(3 << (A >> 10));
	}
}

void setchr4r(int r, uint16 A, uint16 V) {
	if (!CHRptr[r]) {
		return;
	}
	FCEUPPU_LineUpdate();
	V &= CHRmask4[r];
	VPageR[(A) >> 10] = VPageR[((A) >> 10) + 1] =
	VPageR[((A) >> 10) + 2] = VPageR[((A) >> 10) + 3] = &CHRptr[r][(V) << 12] - (A);
	if (CHRram[r]) {
		PPUCHRRAM |= (15 << (A >> 10));
	} else {
		PPUCHRRAM &= ~(15 << (A >> 10));
	}
}

void setchr8r(int r, uint16 V) {
	int x;

	if (!CHRptr[r]) {
		return;
	}
	FCEUPPU_LineUpdate();
	V &= CHRmask8[r];
	for (x = 7; x >= 0; x--) {
		VPageR[x] = &CHRptr[r][V << 13];
	}
	if (CHRram[r]) {
		PPUCHRRAM |= 0xFF;
	} else {
		PPUCHRRAM = 0;
	}
}

void setchr1(uint16 A, uint16 V) {
	setchr1r(0, A, V);
}

void setchr2(uint16 A, uint16 V) {
	setchr2r(0, A, V);
}

void setchr4(uint16 A, uint16 V) {
	setchr4r(0, A, V);
}

void setchr8(uint16 V) {
	setchr8r(0, V);
}

/* This function can be called without calling SetupCartMirroring(). */

void setntamem(uint8 *p, int ram, int b) {
	FCEUPPU_LineUpdate();
	vnapage[b] = p;
	PPUNTARAM &= ~(1 << b);
	if (ram) {
		PPUNTARAM |= 1 << b;
	}
}

static int mirrorhard = 0;
void setmirrorw(int a, int b, int c, int d) {
	FCEUPPU_LineUpdate();
	vnapage[0] = NTARAM + a * 0x400;
	vnapage[1] = NTARAM + b * 0x400;
	vnapage[2] = NTARAM + c * 0x400;
	vnapage[3] = NTARAM + d * 0x400;
}

void setmirror(int t) {
	FCEUPPU_LineUpdate();

	if (mirrorhard) {
		return;
	}

	switch (t) {
	case MI_H:
		vnapage[0] = vnapage[1] = NTARAM;
		vnapage[2] = vnapage[3] = NTARAM + 0x400;
		break;
	case MI_V:
		vnapage[0] = vnapage[2] = NTARAM;
		vnapage[1] = vnapage[3] = NTARAM + 0x400;
		break;
	case MI_0:
		vnapage[0] = vnapage[1] = vnapage[2] = vnapage[3] = NTARAM;
		break;
	case MI_1:
		vnapage[0] = vnapage[1] = vnapage[2] = vnapage[3] = NTARAM + 0x400;
		break;
	case MI_4:
		vnapage[0] = NTARAM;
		vnapage[1] = NTARAM + 0x400;
		vnapage[2] = NTARAM + 0x800;
		vnapage[3] = NTARAM + 0xC00;
		break;
	}
	PPUNTARAM = 0xF;
}

void SetupCartMirroring(int m, int hard, uint8 *extra) {
	if (m < 4) {
		mirrorhard = 0;
		setmirror(m);
	} else {
		vnapage[0] = NTARAM;
		vnapage[1] = NTARAM + 0x400;
		vnapage[2] = extra;
		vnapage[3] = extra + 0x400;
		PPUNTARAM = 0xF;
	}
	mirrorhard = hard;
}
