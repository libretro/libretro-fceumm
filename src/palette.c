/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002,2003 Xodnizel
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

#include <stdlib.h>
#include <math.h>

#include <string/stdstring.h>
#include <file/file_path.h>
#include <streams/file_stream.h>

#include "fceu-types.h"
#include "fceu.h"
#include "general.h"
#include "driver.h"

#include "palette.h"
#include "palettes/palettes.h"

/* These are dynamically filled/generated palettes: */
pal palette_game[PALETTE_ARRAY_SIZE] = { 0 }; /* Custom palette for an individual game. */
pal palette_user[PALETTE_ARRAY_SIZE] = { 0 }; /* Custom "global" palette. */

int palette_user_available = FALSE;
int palette_game_available = FALSE;
int palette_nes_selected = PAL_NES_DEFAULT;

static void ChoosePalette(void);
static void WritePalette(void);

pal *palo = NULL;
static pal *palette_nes_default[8] =
{
	palette,
	rp2c04_0001,
	rp2c04_0002,
	rp2c04_0003,
	rp2c04_0004,
	rp2c03,
};

static float bisqwit_gammafix(float f, float gamma) {
	return f < 0.f ? 0.f : pow(f, 2.2f / gamma);
}
static int bisqwit_clamp(int v) {
	return v < 0 ? 0 : v > 255 ? 255 : v;
}

/* Calculate the luma and chroma by emulating the relevant circuits: */
static int bisqwit_wave(int p, int color) {
	return (color + p + 8) % 12 < 6;
}

#define BCLAMP(x) ((x) < 0 ? 0 : ((x) > 255 ? 255 : (x)))

static void ApplyDeemphasisBisqwit(int entry, uint8 *r, uint8 *g, uint8 *b) {
	int myr = 0, myg = 0, myb = 0;

	/* The input value is a NES color index (with de-emphasis bits).
	 * We need RGB values. Convert the index into RGB.
	 * For most part, this process is described at:
	 *    http://wiki.nesdev.com/w/index.php/NTSC_video
	 */

	/* Decode the color index */
	int color = (entry & 0x0F), level = color < 0xE ? (entry >> 4) & 3 : 1;

	/* Voltage levels, relative to synch voltage */
	static const float black = .518f, white = 1.962f, attenuation = .746f,
	                   levels[8] = { .350f, .518f, .962f, 1.550f, /* Signal low */
		                   1.094f, 1.506f, 1.962f, 1.962f };      /* Signal high */

	float lo_and_hi[2]; /* = { levels[level + 4 * (color == 0x0)], levels[level + 4 * (color < 0xD)] }; */

	int p, pass;

	if (entry < 64)
		return;

	lo_and_hi[0] = levels[level + 4 * (color == 0x0)];
	lo_and_hi[1] = levels[level + 4 * (color < 0xD)];

	/* fceux alteration: two passes
	 * 1st pass calculates bisqwit's base color
	 * 2nd pass calculates it with deemph
	 * finally, we'll do something dumb: find a 'scale factor' between them and apply it to the input palette. (later,
	 * we could pregenerate the scale factors) whatever, it gets the job done.
	 */
	for (pass = 0; pass < 2; pass++) {
		float y = 0.f, i = 0.f, q = 0.f, gamma = 1.8f;
		int rt, gt, bt;

		for (p = 0; p < 12; ++p) /* 12 clock cycles per pixel. */
		{
			/* NES NTSC modulator (square wave between two voltage levels): */
			float spot = lo_and_hi[bisqwit_wave(p, color)];
			float v;

			/* De-emphasis bits attenuate a part of the signal: */
			if (pass == 1) {
				if (((entry & 0x40) && bisqwit_wave(p, 12)) || ((entry & 0x80) && bisqwit_wave(p, 4)) ||
				    ((entry & 0x100) && bisqwit_wave(p, 8)))
					spot *= attenuation;
			}

			/* Normalize: */
			v = (spot - black) / (white - black) / 12.f;

			/* Ideal TV NTSC demodulator: */
			y += v;
			i += v * cos(3.141592653 * p / 6);
			q += v * sin(3.141592653 * p / 6); /* Or cos(... p-3 ... )
			                                    * Note: Integrating cos() and sin() for p-0.5 .. p+0.5 range gives
			                                    *       the exactly same result, scaled by a factor of 2*cos(pi/12).
												*/
		}

		/* Convert YIQ into RGB according to FCC-sanctioned conversion matrix. */

		rt = bisqwit_clamp(255 * bisqwit_gammafix(y + 0.946882f * i + 0.623557f * q, gamma));
		gt = bisqwit_clamp(255 * bisqwit_gammafix(y + -0.274788f * i + -0.635691f * q, gamma));
		bt = bisqwit_clamp(255 * bisqwit_gammafix(y + -1.108545f * i + 1.709007f * q, gamma));

		if (pass == 0)
			myr = rt, myg = gt, myb = bt;
		else {
			float rscale = (float)rt / myr;
			float gscale = (float)gt / myg;
			float bscale = (float)bt / myb;

			if (myr != 0) *r = (uint8)(BCLAMP(*r * rscale));
			if (myg != 0) *g = (uint8)(BCLAMP(*g * gscale));
			if (myb != 0) *b = (uint8)(BCLAMP(*b * bscale));
		}
	}
}

#if 0
static void ApplyDeemphasisClassic(int entry, uint8 *r, uint8 *g, uint8 *b) {
	static const float rtmul[] = { 1.239f, 0.794f, 1.019f, 0.905f, 1.023f, 0.741f, 0.75f };
	static const float gtmul[] = { 0.915f, 1.086f, 0.98f,  1.026f, 0.908f, 0.987f, 0.75f };
	static const float btmul[] = { 0.743f, 0.882f, 0.653f, 1.277f, 0.979f, 0.101f, 0.75f };

	int nr, ng, nb, d;
	int deemph_bits = entry >> 6;

	if (deemph_bits == 0) return;

	d = deemph_bits - 1;
	nr = *r;
	ng = *g;
	nb = *b;
	nr = (int)(nr * rtmul[d]);
	ng = (int)(ng * gtmul[d]);
	nb = (int)(nb * btmul[d]);
	if (nr > 0xFF) nr = 0xFF;
	if (ng > 0xFF) ng = 0xFF;
	if (nb > 0xFF) nb = 0xFF;
	*r = (uint8)nr;
	*g = (uint8)ng;
	*b = (uint8)nb;
}
#endif

static void ApplyDeemphasisComplete(pal *pal512) {
	int i, p, idx;

	/* for each deemph level beyond 0 */
	for (i = 0, idx = 0; i < 8; i++) {
		/* for each palette entry */
		for (p = 0; p < 64; p++, idx++) {
			pal512[idx] = pal512[p];
			ApplyDeemphasisBisqwit(idx, &pal512[idx].r, &pal512[idx].g, &pal512[idx].b);
		}
	}
}

void FCEUI_SetPaletteUser(pal *data, int nEntries) {
	if (!data || !nEntries)
		palette_user_available = FALSE;
	else {
		memcpy(palette_user, data, nEntries * 3);
		if (nEntries != PALETTE_ARRAY_SIZE) ApplyDeemphasisComplete(palette_user);
		palette_user_available = TRUE;
	}
	FCEU_ResetPalette();
}

static uint8 lastd = 0;
void SetNESDeemph(uint8 d, int force) {
	static uint16 rtmul[7] = { 32768 * 1.239, 32768 * .794, 32768 * 1.019, 32768 * .905, 32768 * 1.023, 32768 * .741, 32768 * .75 };
	static uint16 gtmul[7] = { 32768 * .915, 32768 * 1.086, 32768 * .98, 32768 * 1.026, 32768 * .908, 32768 * .987, 32768 * .75 };
	static uint16 btmul[7] = { 32768 * .743, 32768 * .882, 32768 * .653, 32768 * 1.277, 32768 * .979, 32768 * .101, 32768 * .75 };
	uint32 r, g, b;
	int x;

	/* If it's not forced(only forced when the palette changes),
	don't waste cpu time if the same deemphasis bits are set as the last call. */
	if (!force) {
		if (d == lastd)
			return;
	} else {	/* Only set this when palette has changed. */
		r = rtmul[6];
		g = rtmul[6];
		b = rtmul[6];

		for (x = 0; x < 0x40; x++) {
			uint32 m, n, o;
			m = palo[x].r;
			n = palo[x].g;
			o = palo[x].b;
			m = (m * r) >> 15;
			n = (n * g) >> 15;
			o = (o * b) >> 15;
			if (m > 0xff) m = 0xff;
			if (n > 0xff) n = 0xff;
			if (o > 0xff) o = 0xff;
			FCEUD_SetPalette(x | 0xC0, m, n, o);
		}
	}
	if (!d) return;	/* No deemphasis, so return. */

	r = rtmul[d - 1];
	g = gtmul[d - 1];
	b = btmul[d - 1];

	for (x = 0; x < 0x40; x++) {
		uint32 m, n, o;

		m = palo[x].r;
		n = palo[x].g;
		o = palo[x].b;
		m = (m * r) >> 15;
		n = (n * g) >> 15;
		o = (o * b) >> 15;
		if (m > 0xff) m = 0xff;
		if (n > 0xff) n = 0xff;
		if (o > 0xff) o = 0xff;

		FCEUD_SetPalette(x | 0x40, m, n, o);
	}

	lastd = d;
}

void FCEU_LoadGamePalette(void) {
	uint8 ptmp[64 * 8 * 3] = {0};
	RFILE *fp = NULL;
	char *fn = NULL;

	palette_game_available = 0;

	fn = FCEU_MakeFName(FCEUMKF_PALETTE, 0, 0);

	if (!string_is_empty(fn) && path_is_valid(fn))
		fp = filestream_open(fn,
				RETRO_VFS_FILE_ACCESS_READ,
				RETRO_VFS_FILE_ACCESS_HINT_NONE);

	if (fp) {
		int64 readed = filestream_read(fp, ptmp, 64 * 8 * 3);
		int nEntries = readed / 3;
		filestream_close(fp);
		
		memcpy(palette_game, ptmp, readed);
		if (nEntries != PALETTE_ARRAY_SIZE) ApplyDeemphasisComplete(palette_game);

		palette_game_available = 1;
	}
	free(fn);
}

void FCEU_ResetPalette(void) {
	if (GameInfo) {
		ChoosePalette();
		WritePalette();
	}
}

static void ChoosePalette(void) {
	if (GameInfo->type == GIT_NSF) {
		palo = palette_nes_default[0];
	} else if (palette_user_available) {
		palo = palette_user;
	} else if (palette_game_available) {
		palo = palette_game;
	} else {
		palo = palette_nes_default[palette_nes_selected];
		ApplyDeemphasisComplete(palo);
	}
}

static void WritePalette(void) {
	int x;
	int unvaried = sizeof(unvpalette) / sizeof(unvpalette[0]);

	for (x = 0; x < unvaried; x++)
		FCEUD_SetPalette(x, unvpalette[x].r, unvpalette[x].g, unvpalette[x].b);
	for (x = unvaried; x < 256; x++)
		FCEUD_SetPalette(x, 205, 205, 205);
	for (x = 0; x < 64; x++)
		FCEUD_SetPalette(128 + x, palo[x].r, palo[x].g, palo[x].b);
	SetNESDeemph(lastd, 1);
	for (x = 0; x < PALETTE_ARRAY_SIZE; x++) {
		FCEUD_SetPalette(256 + x, palo[x].r, palo[x].g, palo[x].b);
	}
}
