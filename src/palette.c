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
static pal palette_game[512];		/* Custom palette for an individual game. */
static pal palette_user[512];		/* Custom "global" palette. */

uint8 palette_game_available = false;
uint8 palette_user_available = false;

static void ChoosePalette(void);
static void WritePalette(void);
uint8 default_palette_selected = 0;

pal *palo;
static pal *default_palette[8] =
{
	palette,
	rp2c04_0001,
	rp2c04_0002,
	rp2c04_0003,
	rp2c04_0004,
	rp2c03,
};

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

static void ApplyDeemphasisComplete(pal *pal512) {
	int i, p, idx;

	/* for each deemph level beyond 0 */
	for (i = 0, idx = 0; i < 8; i++) {
		/* for each palette entry */
		for (p = 0; p < 64; p++, idx++) {
			pal512[idx] = pal512[p];
			ApplyDeemphasisClassic(idx, &pal512[idx].r, &pal512[idx].g, &pal512[idx].b);
		}
	}
}

void FCEUI_SetPaletteArray(uint8 *pal, int nEntries) {
	if (!pal || !nEntries)
		palette_user_available = false;
	else {
		int x;
		palette_user_available = true;
		for (x = 0; x < nEntries; x++) {
			palette_user[x].r = *((uint8*)pal + x + x + x);
			palette_user[x].g = *((uint8*)pal + x + x + x + 1);
			palette_user[x].b = *((uint8*)pal + x + x + x + 2);
		}
		if (nEntries != 512) {
			ApplyDeemphasisComplete(palette_user);
		}
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
	uint8 ptmp[64 * 8 * 3];
	RFILE *fp = NULL;
	char *fn = NULL;

	palette_game_available = false;

	fn = FCEU_MakeFName(FCEUMKF_PALETTE, 0, 0);

	if (!string_is_empty(fn) && path_is_valid(fn))
		fp = filestream_open(fn,
				RETRO_VFS_FILE_ACCESS_READ,
				RETRO_VFS_FILE_ACCESS_HINT_NONE);

	if (fp) {
		int x;
		int ssize = filestream_get_size(fp);
		int nEntries = ssize / 3;
		filestream_read(fp, ptmp, ssize);
		filestream_close(fp);
		for (x = 0; x < nEntries; x++) {
			palette_game[x].r = ptmp[x + x + x];
			palette_game[x].g = ptmp[x + x + x + 1];
			palette_game[x].b = ptmp[x + x + x + 2];
		}
		if (nEntries != 512)
			ApplyDeemphasisComplete(palette_game);
		palette_game_available = true;
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
		palo = default_palette[0];
	} else if (palette_user_available) {
		palo = palette_user;
	} else if (palette_game_available) {
		palo = palette_game;
	} else {
		palo = default_palette[default_palette_selected];
		ApplyDeemphasisComplete(palo);
	}
}

void WritePalette(void) {
	int x;
	int unvaried = sizeof(unvpalette) / sizeof(unvpalette[0]);

	for (x = 0; x < unvaried; x++)
		FCEUD_SetPalette(x, unvpalette[x].r, unvpalette[x].g, unvpalette[x].b);
	for (x = unvaried; x < 256; x++)
		FCEUD_SetPalette(x, 205, 205, 205);
	for (x = 0; x < 64; x++)
		FCEUD_SetPalette(128 + x, palo[x].r, palo[x].g, palo[x].b);
	SetNESDeemph(lastd, 1);
	for (x = 0; x < 512; x++) {
		FCEUD_SetPalette(256 + x, palo[x].r, palo[x].g, palo[x].b);
	}
}
