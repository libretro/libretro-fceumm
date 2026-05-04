/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2003 Xodnizel
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
#include "share.h"

static int seq, ptr, bit, cnt, have;
static uint8_t bdata[20];


static uint8_t FP_FASTAPASS(2) Read(int w, uint8_t ret) {
	if (w && have) {
		switch (seq) {
		case 0: seq++; ptr = 0; ret |= 0x4; break;
		case 1: seq++; bit = bdata[ptr]; cnt = 0; ret |= 0x4; break;
		case 2: ret |= ((bit & 0x01) ^ 0x01) << 2; bit >>= 1; if (++cnt > 7) seq++;
			break;
		case 3: if (++ptr > 19) {
				seq = -1;
				have = 0;
		} else
				seq = 1;
		default: break;
		}
	}
	return(ret);
}

static void FP_FASTAPASS(1) Write(uint8_t V) {
}

static void FP_FASTAPASS(2) Update(void *data, int arg) {
	if (*(uint8_t*)data) {
		size_t i;
		const uint8_t *src = (uint8_t*)data + 1;
		*(uint8_t*)data = 0;
		seq = ptr = 0;
		have = 1;
		/* bdata is 20 bytes total; bytes 13..19 are filled by the
		 * memcpy below. Copy at most 13 bytes from `src`, ensuring
		 * NUL termination at index 13 or earlier. */
		for (i = 0; i < 13; i++) {
			if (!src[i]) break;
			bdata[i] = src[i];
		}
		bdata[i] = 0;
		memcpy((char*)&bdata[13], "SUNSOFT", 7);
	}
}

static INPUTCFC BarcodeWorld = { Read, Write, 0, Update, 0, 0 };

INPUTCFC *FCEU_InitBarcodeWorld(void) {
	return(&BarcodeWorld);
}

