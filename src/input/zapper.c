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

#include		<string.h>
#include		<stdlib.h>

#include		"share.h"

#define ROUNDED_TARGET
#ifdef ROUNDED_TARGET
#define MAX_TOLERANCE 20
static uint32_t targetExpansion[MAX_TOLERANCE+1];
#endif
static uint32_t tolerance;
static uint32_t ZapperStrobe[2];

int switchZapper = 0;
int zapper_trigger_invert_option = 1;
int zapper_sensor_invert_option = 1;

typedef struct {
	uint32_t mzx, mzy, mzb, mzs; /* sequential targets lightgun sensor added */
	int zap_readbit;
	int bogo;
	int zappo;

	uint64_t zaphit;
} ZAPPER;

static ZAPPER ZD[2];

static void FP_FASTAPASS(3) ZapperFrapper(int w, uint8_t * bg, uint8_t * spr, uint32_t linets, int final) {
	if (!switchZapper) {
	   int xs, xe;
	   int zx, zy;

		if (!bg) {	/* New line, so reset stuff. */
			ZD[w].zappo = 0;
			return;
		}
		xs = ZD[w].zappo;
		xe = final;

		zx = ZD[w].mzx;
		zy = ZD[w].mzy;

		if (xe > 256) xe = 256;

		if (scanline >= (zy - tolerance) && scanline <= (zy + tolerance)) {
	#ifdef ROUNDED_TARGET
			int spread;
			int dy = scanline - zy;
			if (dy < 0)
				dy = -dy;
			spread = targetExpansion[dy];
	#else
			int spread = tolerance;
	#endif
			while (xs < xe) {
				uint8_t a1, a2;
				uint32_t sum;
				if (xs <= (zx + spread) && xs >= (zx - spread)) {
					a1 = bg[xs];
					if (spr) {
						a2 = spr[xs];

						if (!(a2 & 0x80))
							if (!(a2 & 0x40) || (a1 & 64))
								a1 = a2;
					}
					a1 &= 63;

					sum = palo[a1].r + palo[a1].g + palo[a1].b;
					if (sum >= 100 * 3) {
						ZD[w].zaphit = ((uint64_t)linets + (uint64_t)(xs + 16) * (PAL ? 15 : 16)) / 48 + timestampbase;
						goto endo;
					}
				}
				xs++;
			}
		}
 	endo:
		ZD[w].zappo = final;
	}
	else {
		ZD[w].zappo = 0;
	}
}

static INLINE int CheckColor(int w) {
	FCEUPPU_LineUpdate();

	if ((ZD[w].zaphit + 100) >= (timestampbase + timestamp)
		&& !(ZD[w].mzb & 2)) return(0);

	return(1);
}

static uint8_t FP_FASTAPASS(1) ReadZapperVS(int w) {
	uint8_t ret = 0;

	if (ZD[w].zap_readbit == 4) ret = 1;

	if (ZD[w].zap_readbit == 7) {
		if (!ZD[w].bogo)
			ret |= 0x1;	
	}

	if (ZD[w].zap_readbit == 6) {
		if (!switchZapper) {
			if (!CheckColor(w))
				ret |= 0x1;
		}
		else if (!ZD[w].mzs)
			ret |= 0x1;
	}

	ZD[w].zap_readbit++;
	return ret;
}

static void FP_FASTAPASS(1) StrobeZapperVS(int w) {
	ZD[w].zap_readbit = 0;
}

static uint8_t FP_FASTAPASS(1) ReadZapper(int w) {
	uint8_t ret = 0;
		
	if (ZD[w].bogo) 
		ret |= 0x10;

	if (!switchZapper) {
		if (CheckColor(w))
			ret |= 0x8;
	}
	else if (ZD[w].mzs)
			ret |= 0x8;	

	return ret;
}

static void FASTAPASS(3) DrawZapper(int w, uint8_t * buf, int arg) {
	if (arg && !switchZapper)
		FCEU_DrawGunSight(buf, ZD[w].mzx, ZD[w].mzy);
}

static void FP_FASTAPASS(3) UpdateZapper(int w, void *data, int arg) {
	uint32_t *ptr = (uint32_t*)data;

	if (ZD[w].bogo)
		ZD[w].bogo--;
	if (ptr[2] & 3 && (!(ZD[w].mzb & 3)))
		ZD[w].bogo = 5;

	ZD[w].mzx = ptr[0];
	ZD[w].mzy = ptr[1];

	if (zapper_trigger_invert_option)
		ZD[w].mzb = ptr[2];
	else
		ZD[w].mzb = !ptr[2];

	if (zapper_sensor_invert_option)
		ZD[w].mzs = !ptr[3];
	else
		ZD[w].mzs = ptr[3];
}

static INPUTC ZAPC = { ReadZapper, 0, 0, UpdateZapper, ZapperFrapper, DrawZapper };
static INPUTC ZAPVSC = { ReadZapperVS, 0, StrobeZapperVS, UpdateZapper, ZapperFrapper, DrawZapper };

#ifdef ROUNDED_TARGET
static uint32_t InefficientSqrt(uint32_t z) {
	uint32_t i;
	for (i = 0 ; i * i <= z ; i++) ;
	return i-1;
}
#endif

void FCEU_ZapperSetTolerance(int t)
{
#ifdef ROUNDED_TARGET
	uint32_t y;
	tolerance = t <= MAX_TOLERANCE ? t : MAX_TOLERANCE;
	for (y = 0; y <= tolerance; y++)
		targetExpansion[y] = InefficientSqrt(tolerance*tolerance-y*y);
#else
	tolerance = t;
#endif
}

INPUTC *FCEU_InitZapper(int w) {
	memset(&ZD[w], 0, sizeof(ZAPPER));
	if (GameInfo->type == GIT_VSUNI)
		return(&ZAPVSC);
	else
		return(&ZAPC);
}

