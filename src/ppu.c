/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
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

#include        <string.h>
#include        <stdlib.h>

#include        "fceu-types.h"
#include        "x6502.h"
#include        "fceu.h"
#include        "ppu.h"
#include        "nsf.h"
#include        "sound.h"
#include        "general.h"
#include        "fceu-endian.h"
#include        "fceu-memory.h"

#include        "cart.h"
#include        "palette.h"
#include        "state.h"
#include        "video.h"
#include        "input.h"

#ifdef HAVE_HDPACK
#include        "hdpack/hdpack.h"
#endif

#define VBlankON        (PPU[0] & 0x80)		/* Generate VBlank NMI */
#define Sprite16        (PPU[0] & 0x20)		/* Sprites 8x16/8x8 */
#define BGAdrHI         (PPU[0] & 0x10)		/* BG pattern adr $0000/$1000 */
#define SpAdrHI         (PPU[0] & 0x08)		/* Sprite pattern adr $0000/$1000 */
#define INC32           (PPU[0] & 0x04)		/* auto increment 1/32 */

#define SpriteON        (PPU[1] & 0x10)		/* Show Sprite */
#define ScreenON        (PPU[1] & 0x08)		/* Show screen */
#define GRAYSCALE       (PPU[1] & 0x01)		/* Grayscale (AND palette entries with 0x30) */

#define PPU_status      (PPU[2])

#define Pal             (PALRAM)

static void FetchSpriteData(void);
static void FASTAPASS(1) RefreshLine(int lastpixel);
static void RefreshSprites(void);
static void CopySprites(uint8_t *target);

static void Fixit1(void);
static uint32_t ppulut1[256];
static uint32_t ppulut2[256];
static uint32_t ppulut3[128];

static void makeppulut(void) {
	int x;
	int y;
	int cc, xo, pixel;


	for (x = 0; x < 256; x++) {
		ppulut1[x] = 0;
		for (y = 0; y < 8; y++)
			ppulut1[x] |= ((x >> (7 - y)) & 1) << (y * 4);
		ppulut2[x] = ppulut1[x] << 1;
	}

	for (cc = 0; cc < 16; cc++) {
		for (xo = 0; xo < 8; xo++) {
			ppulut3[xo | (cc << 3)] = 0;
			for (pixel = 0; pixel < 8; pixel++) {
				int shiftr;
				shiftr = (pixel + xo) / 8;
				shiftr *= 2;
				ppulut3[xo | (cc << 3)] |= (uint32_t)((cc >> shiftr) & 3) << (2 + pixel * 4);
			}
		}
	}
}

static uint8_t ppudead = 1;
static uint8_t kook = 0;

int MMC5Hack = 0, PEC586Hack = 0;
uint32_t MMC5HackVROMMask = 0;
uint8_t *MMC5HackExNTARAMPtr = 0;
uint8_t *MMC5HackVROMPTR = 0;
uint8_t MMC5HackCHRMode = 0;
uint8_t MMC5HackSPMode = 0;
uint8_t MMC50x5130 = 0;
uint8_t MMC5HackSPScroll = 0;
uint8_t MMC5HackSPPage = 0;

uint8_t VRAMBuffer = 0, PPUGenLatch = 0;
uint8_t *vnapage[4];
uint8_t PPUNTARAM = 0;
uint8_t PPUCHRRAM = 0;

/* Color deemphasis emulation.  Joy... */
static uint8_t deemp = 0;
static int deempcnt[8];

void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
void FP_FASTAPASS(1) (*PPU_hook)(uint32_t A);

uint8_t vtoggle = 0;
uint8_t XOffset = 0;

uint32_t TempAddr = 0, RefreshAddr = 0;

static int maxsprites = 8;

/* scanline is equal to the current visible scanline we're on. */
int scanline;
static uint32_t scanlines_per_frame;

uint8_t PPU[4];
uint8_t PPUSPL;
uint8_t NTARAM[0x800], PALRAM[0x20], SPRAM[0x100], SPRBUF[0x100];
uint8_t UPALRAM[0x03];/* for 0x4/0x8/0xC addresses in palette, the ones in
					 * 0x20 are 0 to not break fceu rendering.
					 */

#define MMC5SPRVRAMADR(V)   &MMC5SPRVPage[(V) >> 10][(V)]
#define VRAMADR(V)          &VPage[(V) >> 10][(V)]

static uint8_t * MMC5BGVRAMADR(uint32_t V) {
	if (!Sprite16) {
		extern uint8_t mmc5ABMode;				/* A=0, B=1 */
		if (mmc5ABMode == 0)
			return MMC5SPRVRAMADR(V);
		else
			return &MMC5BGVPage[(V) >> 10][(V)];
	} else return &MMC5BGVPage[(V) >> 10][(V)];
}

#define PAL(c)  ((c) + cc)

#define GETLASTPIXEL    (PAL ? ((timestamp * 48 - linestartts) / 15) : ((timestamp * 48 - linestartts) >> 4))

static uint8_t *Pline, *Plinef;
static int firsttile;
static int linestartts;
static int tofix = 0;

typedef struct {
	uint16_t dot;
	uint8_t ppu1;
	uint8_t pad;
} PPU1TintEvent;

/* At most ~29 4-cycle CPU writes fit in a 114-cycle scanline. */
#define MAX_TINT_EVENTS 32
static PPU1TintEvent tint_queue[MAX_TINT_EVENTS];
static int tint_count = 0;
static uint8_t tint_line_start_ppu1 = 0;
uint8_t ppu_frame_tint_lines[240];
uint8_t ppu_frame_has_tint = 0;

/* Sprite 0 on the current line, until its hit is found (0x100: none). */
static int32_t sphitx;
static uint8_t sphitdata;

/* A status or open-bus read changes nothing rendering depends on, so the
 * line is brought up to date only when the read can see the sprite-0 hit
 * land, or when a mapper or input hook watches the rendering as it goes.
 * Otherwise the next register access or the end of the line renders it. */
#define PPU_READ_NEEDS_LINE \
	(sphitx != 0x100 || PPU_hook || MMC5Hack || InputScanlineHook)

static DECLFR(A2002) {
	uint8_t ret;

	if (PPU_READ_NEEDS_LINE)
		FCEUPPU_LineUpdate();
	ret = PPU_status;
	ret |= PPUGenLatch & 0x1F;

	{
		vtoggle = 0;
		PPU_status &= 0x7F;
		PPUGenLatch = ret;
	}

	return ret;
}

static DECLFR(A200x) {	/* Not correct for $2004 reads. */
	if (PPU_READ_NEEDS_LINE)
		FCEUPPU_LineUpdate();
	return PPUGenLatch;
}

static DECLFR(A2007) {
	uint8_t ret;
	uint32_t tmp = RefreshAddr & 0x3FFF;

	FCEUPPU_LineUpdate();

	if (tmp >= 0x3F00) {	/* Palette RAM tied directly to the output data, without VRAM buffer */
		if (!(tmp & 3)) {
			if (!(tmp & 0xC))
				ret = PALRAM[0x00];
			else
				ret = UPALRAM[((tmp & 0xC) >> 2) - 1];
		} else
			ret = PALRAM[tmp & 0x1F];
		if (GRAYSCALE)
			ret &= 0x30;
		{
			if ((tmp - 0x1000) < 0x2000)
				VRAMBuffer = VPage[(tmp - 0x1000) >> 10][tmp - 0x1000];
			else
				VRAMBuffer = vnapage[((tmp - 0x1000) >> 10) & 0x3][(tmp - 0x1000) & 0x3FF];
			if (PPU_hook) PPU_hook(tmp);
		}
	} else {
		ret = VRAMBuffer;
		{
			if (PPU_hook) PPU_hook(tmp);
			PPUGenLatch = VRAMBuffer;
			if (tmp < 0x2000) {
				VRAMBuffer = VPage[tmp >> 10][tmp];
			} else if (tmp < 0x3F00)
				VRAMBuffer = vnapage[(tmp >> 10) & 0x3][tmp & 0x3FF];
		}
	}

	{
		if ((ScreenON || SpriteON) && (scanline < 240)) {
			uint32_t rad = RefreshAddr;
			if ((rad & 0x7000) == 0x7000) {
				rad ^= 0x7000;
				if ((rad & 0x3E0) == 0x3A0)
					rad ^= 0xBA0;
				else if ((rad & 0x3E0) == 0x3e0)
					rad ^= 0x3e0;
				else
					rad += 0x20;
			} else
				rad += 0x1000;
			RefreshAddr = rad;
		} else {
			if (INC32)
				RefreshAddr += 32;
			else
				RefreshAddr++;
		}
		if (PPU_hook) PPU_hook(RefreshAddr & 0x3fff);
	}
	return ret;
}

static DECLFW(B2000) {
	FCEUPPU_LineUpdate();
	PPUGenLatch = V;

	if (!(PPU[0] & 0x80) && (V & 0x80) && (PPU_status & 0x80))
		TriggerNMI2();

	PPU[0] = V;
	TempAddr &= 0xF3FF;
	TempAddr |= (V & 3) << 10;
}

static DECLFW(B2001) {
	uint8_t changed = PPU[1] ^ V;

	if (changed & 0x1E)
		FCEUPPU_LineUpdate();

	if (changed & 0xE1) {
		int dot = GETLASTPIXEL;
		if (dot < 0)
			dot = 0;
		else if (dot > 256)
			dot = 256;

		if (tint_count > 0 && tint_queue[tint_count - 1].dot == (uint16_t)dot)
			tint_queue[tint_count - 1].ppu1 = V;
        else if (tint_count < MAX_TINT_EVENTS)
        {
			tint_queue[tint_count].dot = (uint16_t)dot;
			tint_queue[tint_count].ppu1 = V;
			tint_count++;
		}
	}

	PPUGenLatch = V;
	PPU[1] = V;
	if (V & 0xE0)
		deemp = V >> 5;
}

static DECLFW(B2002) {
	PPUGenLatch = V;
}

static DECLFW(B2003) {
	PPUGenLatch = V;
	PPU[3] = V;
	PPUSPL = V & 0x7;
}

/* For scanlines 0-255, the sprites whose rows cover each line, as bits
 * by OAM index (lo: 0-31, hi: 32-63), rebuilt when OAM or the sprite
 * height has changed since. */
static uint32_t spr_line_lo[256], spr_line_hi[256];
static uint8_t spr_lines_h;
static uint8_t spr_lines_dirty = 1;

static DECLFW(B2004) {
	PPUGenLatch = V;
	spr_lines_dirty = 1;
	if (PPUSPL >= 8) {
		if (PPU[3] >= 8)
			SPRAM[PPU[3]] = V;
	} else {
		SPRAM[PPUSPL] = V;
	}
	PPU[3]++;
	PPUSPL++;
}

static DECLFW(B2005) {
	uint32_t tmp = TempAddr;
	FCEUPPU_LineUpdate();
	PPUGenLatch = V;
	if (!vtoggle) {
		tmp &= 0xFFE0;
		tmp |= V >> 3;
		XOffset = V & 7;
	} else {
		tmp &= 0x8C1F;
		tmp |= ((V & ~0x7) << 2);
		tmp |= (V & 7) << 12;
	}
	TempAddr = tmp;
	vtoggle ^= 1;
}


static DECLFW(B2006) {
	FCEUPPU_LineUpdate();

	PPUGenLatch = V;
	if (!vtoggle) {
		TempAddr &= 0x00FF;
		TempAddr |= (V & 0x3f) << 8;
	} else {
		TempAddr &= 0xFF00;
		TempAddr |= V;

		RefreshAddr = TempAddr;
		if (PPU_hook)
			PPU_hook(RefreshAddr);
	}
	vtoggle ^= 1;
}

static DECLFW(B2007) {
	uint32_t tmp = RefreshAddr & 0x3FFF;
	PPUGenLatch = V;
	if (tmp < 0x2000) {
		if (PPUCHRRAM & (1 << (tmp >> 10)))
			VPage[tmp >> 10][tmp] = V;
	} else if (tmp < 0x3F00) {
		if (PPUNTARAM & (1 << ((tmp & 0xF00) >> 10)))
			vnapage[((tmp & 0xF00) >> 10)][tmp & 0x3FF] = V;
	} else {
		if (!(tmp & 3)) {
			if (!(tmp & 0xC))
				PALRAM[0x00] = PALRAM[0x04] = PALRAM[0x08] = PALRAM[0x0C] = V & 0x3F;
			else
				UPALRAM[((tmp & 0xC) >> 2) - 1] = V & 0x3F;
		} else
			PALRAM[tmp & 0x1F] = V & 0x3F;
	}
	if (INC32)
		RefreshAddr += 32;
	else
		RefreshAddr++;
	if (PPU_hook)
		PPU_hook(RefreshAddr & 0x3fff);
}

static DECLFW(B4014) {
	uint32_t t = V << 8;
	int x;

	for (x = 0; x < 256; x++)
		X6502_DMW(0x2004, X6502_DMR(t + x));
}

static void ResetRL(uint8_t *target) {
	memset(target, 0xFF, 256);
	if (InputScanlineHook)
		InputScanlineHook(0, 0, 0, 0);
	Plinef = target;
	Pline = target;
	firsttile = 0;
	linestartts = x6502_count_base;
	tint_line_start_ppu1 = PPU[1];
	tint_count = 0;
	tofix = 0;
	FCEUPPU_LineUpdate();
	tofix = 1;
}

static uint8_t sprlinebuf[256 + 8];
/* Columns [spr_xmin, spr_xmax) of sprlinebuf that sprites drew into. */
static int spr_xmin, spr_xmax;

void FCEUPPU_LineUpdate(void) {
	if (Pline) {
		int l = GETLASTPIXEL;
		RefreshLine(l);
	}
}

static int rendis = 0;

void FCEUI_SetRenderDisable(int sprites, int bg) {
	if (sprites >= 0) {
		if (sprites == 2) rendis ^= 1;
		else rendis = (rendis & ~1) | sprites ? 1 : 0;
	}
	if (bg >= 0) {
		if (bg == 2) rendis ^= 2;
		else rendis = (rendis & ~2) | bg ? 2 : 0;
	}
}

static void CheckSpriteHit(int p);

static void EndRL(void) {
	RefreshLine(272);
	if (tofix)
		Fixit1();
	CheckSpriteHit(272);
	Pline = 0;
}

static void CheckSpriteHit(int p) {
	int l = p - 16;
	int x;

	if (sphitx == 0x100) return;

	for (x = sphitx; x < (sphitx + 8) && x < l; x++) {
		if ((sphitdata & (0x80 >> (x - sphitx))) && !(Plinef[x] & 64) && x < 255) {
			PPU_status |= 0x40;
			sphitx = 0x100;
			break;
		}
	}
}

/* spork the world.  Any sprites on this line? Then this will be set to 1.
 * Needed for zapper emulation and *gasp* sprite emulation.
 */
static int spork = 0;

/* lasttile is really "second to last tile." */
static void FASTAPASS(1) RefreshLine(int lastpixel) {
	static uint32_t pshift[2];
	static uint32_t atlatch;
	uint32_t smorkus = RefreshAddr;

	#define RefreshAddr smorkus
	uint32_t vofs;
	int X1;

	register uint8_t *P = Pline;
	int lasttile = lastpixel >> 3;
	int numtiles;
	static int norecurse = 0;	/* Yeah, recursion would be bad.
								 * PPU_hook() functions can call
								 * mirroring/chr bank switching functions,
								 * which call FCEUPPU_LineUpdate, which call this
								 * function.
								 */
	if (norecurse) return;

	if (sphitx != 0x100 && !(PPU_status & 0x40)) {
		if ((sphitx < (lastpixel - 16)) && !(sphitx < ((lasttile - 2) * 8)))
			lasttile++;
	}

	if (lasttile > 34) lasttile = 34;
	numtiles = lasttile - firsttile;

	if (numtiles <= 0) return;

	P = Pline;

	vofs = 0;

	if(PEC586Hack)
		vofs = ((RefreshAddr & 0x200) << 3) | ((RefreshAddr >> 12) & 7);
	else
		vofs = ((PPU[0] & 0x10) << 8) | ((RefreshAddr >> 12) & 7);

	if (!ScreenON && !SpriteON) {
		uint32_t tem;
		tem = Pal[0] | (Pal[0] << 8) | (Pal[0] << 16) | (Pal[0] << 24);
		tem |= 0x40404040;
		FCEU_dwmemset(Pline, tem, numtiles * 8);
		P += numtiles * 8;
		Pline = P;

		firsttile = lasttile;

		#define TOFIXNUM (272 - 0x4)
		if (lastpixel >= TOFIXNUM && tofix) {
			Fixit1();
			tofix = 0;
		}

		if (InputScanlineHook && (lastpixel - 16) >= 0) {
			InputScanlineHook(Plinef, spork ? sprlinebuf : 0, linestartts, lasttile * 8 - 16);
		}
		return;
	}

	/* Priority bits, needed for sprite emulation. */
	Pal[0] |= 64;
	Pal[4] |= 64;
	Pal[8] |= 64;
	Pal[0xC] |= 64;

	/* This high-level graphics MMC5 emulation code was written for MMC5 carts in "CL" mode.
	 * It's probably not totally correct for carts in "SL" mode.
	 */

#define PPUT_MMC5
	if (MMC5Hack && geniestage != 1) {
		if (MMC5HackCHRMode == 0 && (MMC5HackSPMode & 0x80)) {
			int tochange = MMC5HackSPMode & 0x1F;
			tochange -= firsttile;
			for (X1 = firsttile; X1 < lasttile; X1++) {
				if ((tochange <= 0 && MMC5HackSPMode & 0x40) || (tochange > 0 && !(MMC5HackSPMode & 0x40))) {
					#define PPUT_MMC5SP
					#include "pputile.h"
					#undef PPUT_MMC5SP
				} else {
					#include "pputile.h"
				}
				tochange--;
			}
		} else if (MMC5HackCHRMode == 1 && (MMC5HackSPMode & 0x80)) {
			int tochange = MMC5HackSPMode & 0x1F;
			tochange -= firsttile;

			#define PPUT_MMC5SP
			#define PPUT_MMC5CHR1
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
			#undef PPUT_MMC5CHR1
			#undef PPUT_MMC5SP
		} else if (MMC5HackCHRMode == 1) {
			#define PPUT_MMC5CHR1
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
			#undef PPUT_MMC5CHR1
		} else {
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
		}
	}
	#undef PPUT_MMC5
	else if (PPU_hook) {
		norecurse = 1;
		#define PPUT_HOOK
		if (PEC586Hack) {
			#define PPU_BGFETCH
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
			#undef PPU_BGFETCH
		} else {
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
		}
		#undef PPUT_HOOK
		norecurse = 0;
	} else {
		if (PEC586Hack) {
			#define PPU_BGFETCH
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
			#undef PPU_BGFETCH
		} else {
			for (X1 = firsttile; X1 < lasttile; X1++) {
				#include "pputile.h"
			}
		}
	}

#undef vofs
#undef RefreshAddr

	/* Reverse changes made before. */
	Pal[0] &= 63;
	Pal[4] &= 63;
	Pal[8] &= 63;
	Pal[0xC] &= 63;

	RefreshAddr = smorkus;
	if (firsttile <= 2 && 2 < lasttile && !(PPU[1] & 2)) {
		uint32_t tem;
		tem = Pal[0] | (Pal[0] << 8) | (Pal[0] << 16) | (Pal[0] << 24);
		tem |= 0x40404040;
		*(uint32_t*)Plinef = *(uint32_t*)(Plinef + 4) = tem;
	}

	if (!ScreenON) {
		uint32_t tem;
		int tstart, tcount;
		tem = Pal[0] | (Pal[0] << 8) | (Pal[0] << 16) | (Pal[0] << 24);
		tem |= 0x40404040;

		tcount = lasttile - firsttile;
		tstart = firsttile - 2;
		if (tstart < 0) {
			tcount += tstart;
			tstart = 0;
		}
		if (tcount > 0)
			FCEU_dwmemset(Plinef + tstart * 8, tem, tcount * 8);
	}

	if (lastpixel >= TOFIXNUM && tofix) {
		Fixit1();
		tofix = 0;
	}

	/* This only works right because of a hack earlier in this function. */
	CheckSpriteHit(lastpixel);

	if (InputScanlineHook && (lastpixel - 16) >= 0) {
		InputScanlineHook(Plinef, spork ? sprlinebuf : 0, linestartts, lasttile * 8 - 16);
	}
	Pline = P;
	firsttile = lasttile;
}

static INLINE void Fixit2(void) {
	if (ScreenON || SpriteON) {
		uint32_t rad = RefreshAddr;
		rad &= 0xFBE0;
		rad |= TempAddr & 0x041f;
		RefreshAddr = rad;
	}
}

static void Fixit1(void) {
	if (ScreenON || SpriteON) {
		uint32_t rad = RefreshAddr;

		if ((rad & 0x7000) == 0x7000) {
			rad ^= 0x7000;
			if ((rad & 0x3E0) == 0x3A0)
				rad ^= 0xBA0;
			else if ((rad & 0x3E0) == 0x3e0)
				rad ^= 0x3e0;
			else
				rad += 0x20;
		} else
			rad += 0x1000;
		RefreshAddr = rad;
	}
}

static void ApplyTintSpan(uint8_t *target, uint8_t *dtarget, int x0, int x1, uint8_t ppu1)
{
	uint32_t and_mask = 0xFFFFFFFFu;
	uint32_t or_mask;
	uint8_t b_and;
	uint8_t b_or;
	uint8_t emph = ppu1 >> 5;
	int x = x0;
	int aligned_start;
	int aligned_end;

	if ((ScreenON || SpriteON) && (ppu1 & 0x01))
		and_mask = 0x30303030u;

	if (emph == 0x7) {
		and_mask &= 0x3f3f3f3fu;
		or_mask   = 0xc0c0c0c0u;
	} else if (ppu1 & 0xE0) {
		or_mask   = 0x40404040u;
	} else {
		and_mask &= 0x3f3f3f3fu;
		or_mask   = 0x80808080u;
	}

	b_and = (uint8_t)and_mask;
	b_or  = (uint8_t)or_mask;

	aligned_start = (x0 + 3) & ~3;
	aligned_end   = x1 & ~3;

	if (aligned_start >= aligned_end) {
		for (; x < x1; x++)
			target[x] = (target[x] & b_and) | b_or;
	} else {
		for (; x < aligned_start; x++)
			target[x] = (target[x] & b_and) | b_or;
		for (; x < aligned_end; x += 4) {
			uint32_t w;
			memcpy(&w, target + x, 4);
			w = (w & and_mask) | or_mask;
			memcpy(target + x, &w, 4);
		}
		for (; x < x1; x++)
			target[x] = (target[x] & b_and) | b_or;
	}

	memset(dtarget + x0, emph, x1 - x0);
}

void MMC5_hb(int);		/* Ugh ugh ugh. */
static void DoLine(void)
{
	int x;
	uint8_t *target = NULL;
	uint8_t *dtarget = NULL;

	if (scanline >= 240 && (unsigned)scanline != totalscanlines)
	{
		X6502_Run(256 + 69);
		scanline++;
		X6502_Run(16);
		return;
	}

	target = XBuf + ((scanline < 240 ? scanline : 240) << 8);
	dtarget = XDBuf + ((scanline < 240 ? scanline : 240) << 8);

#ifdef HAVE_HDPACK
	/* HD packs: snapshot the loopy-t / fine-x scroll registers at line
	 * start and invalidate the per-line tile fetch slots. */
	if (hdnes_active && scanline < 240)
		HDNes_LineStart(scanline);
#endif

	if (MMC5Hack && (ScreenON || SpriteON)) MMC5_hb(scanline);

	X6502_Run(256);
	EndRL();

	if (rendis & 2) {/* User asked to not display background data. */
		uint32_t tem;
		tem = Pal[0] | (Pal[0] << 8) | (Pal[0] << 16) | (Pal[0] << 24);
		tem |= 0x40404040;
		FCEU_dwmemset(target, tem, 256);
	}

#ifdef HAVE_HDPACK
	/* HD packs: the line's XBuf still holds pure background pixels
	 * here (bit 6 = colour-0 transparency), which is exactly what the
	 * per-pixel HD screen info needs; sprites are merged from the
	 * records made by FetchSpriteData on the previous line. */
	if (hdnes_active && scanline < 240)
		HDNes_RecordLine(scanline, target, rendis);
#endif

	if (SpriteON)
		CopySprites(target);

	if (scanline < 240)
	{
		ppu_frame_tint_lines[scanline] = (uint8_t)(tint_count > 0);
		if (tint_count > 0)
			ppu_frame_has_tint = 1;
	}
	
	/* One pass over the line: greyscale (while rendering), then the
	 * palette bank bits the line's emphasis selects. */
	if (!tint_count)
	{
		uint32_t and_mask = 0xFFFFFFFFu;
		uint32_t or_mask;

		if ((ScreenON || SpriteON) && (PPU[1] & 0x01))
			and_mask = 0x30303030u;
		if ((PPU[1] >> 5) == 0x7) {
			and_mask &= 0x3f3f3f3fu;
			or_mask   = 0xc0c0c0c0u;
		} else if (PPU[1] & 0xE0)
			or_mask   = 0x40404040u;
		else {
			and_mask &= 0x3f3f3f3fu;
			or_mask   = 0x80808080u;
		}
		for (x = 0; x < 256; x += 4) {
			uint32_t w;
			memcpy(&w, target + x, 4);
			w = (w & and_mask) | or_mask;
			memcpy(target + x, &w, 4);
		}
		
		/* The line's colour emphasis, one byte per pixel. */
		memset(dtarget, PPU[1] >> 5, 256);
	}
    else
    {
		int cur_x = 0;
		uint8_t cur_ppu1 = tint_line_start_ppu1;
		int i;

		for (i = 0; i <= tint_count; i++)
        {
			int next_x = (i < tint_count) ? tint_queue[i].dot : 256;
			if (next_x > cur_x)
            {
				ApplyTintSpan(target, dtarget, cur_x, next_x, cur_ppu1);
				cur_x = next_x;
			}
			if (i < tint_count)
				cur_ppu1 = tint_queue[i].ppu1;
		}
	}

    sphitx = 0x100;

	if (ScreenON || SpriteON)
		FetchSpriteData();

	if (GameHBIRQHook && (ScreenON || SpriteON) && ((PPU[0] & 0x38) != 0x18)) {
		X6502_Run(6);
		Fixit2();
		X6502_Run(4);
		GameHBIRQHook();
		X6502_Run(85 - 16 - 10);
	} else {
		X6502_Run(6);	/* Tried 65, caused problems with Slalom(maybe others) */
		Fixit2();
		X6502_Run(85 - 6 - 16);

		/* A semi-hack for Star Trek: 25th Anniversary */
		if (GameHBIRQHook && (ScreenON || SpriteON) && ((PPU[0] & 0x38) != 0x18))
			GameHBIRQHook();
	}

	if (SpriteON)
		RefreshSprites();
	if (GameHBIRQHook2 && (ScreenON || SpriteON))
		GameHBIRQHook2();
	scanline++;
	if (scanline < 240) {
		ResetRL(XBuf + (scanline << 8));
	}
	X6502_Run(16);
}

#define V_FLIP  0x80
#define H_FLIP  0x40
#define SP_BACK 0x20

typedef struct {
	uint8_t y, no, atr, x;
} SPR;

typedef struct {
	uint8_t ca[2], atr, x;
} SPRB;

void FCEUI_DisableSpriteLimitation(int a) {
	maxsprites = a ? 64 : 8;
}

static uint8_t numsprites, SpriteBlurp;
static void BuildSpriteLines(uint8_t H) {
	int i, t;

	memset(spr_line_lo, 0, sizeof(spr_line_lo));
	memset(spr_line_hi, 0, sizeof(spr_line_hi));
	for (i = 0; i < 64; i++) {
		int y = SPRAM[i << 2];
		uint32_t bit = (uint32_t)1 << (i & 31);
		uint32_t *line = (i < 32) ? spr_line_lo : spr_line_hi;
		for (t = 0; t < H && y + t < 256; t++)
			line[y + t] |= bit;
	}
	spr_lines_h = H;
	spr_lines_dirty = 0;
}

/* Index of the lowest set bit of a nonzero word. */
static const uint8_t spr_debruijn[32] = {
	0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
	31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};
#define SPR_LOWEST_BIT(m) \
	spr_debruijn[(((m) & (0u - (m))) * 0x077CB531u) >> 27]

/* Next OAM index from the candidate masks, lowest first; -1 when none
 * are left. */
static INLINE int NextSprite(uint32_t *lo, uint32_t *hi) {
	int i;
	if (*lo) {
		i = SPR_LOWEST_BIT(*lo);
		*lo &= *lo - 1;
		return i;
	}
	if (*hi) {
		i = 32 + SPR_LOWEST_BIT(*hi);
		*hi &= *hi - 1;
		return i;
	}
	return -1;
}

static void FetchSpriteData(void) {
	uint8_t ns, sb;
	SPR *spr;
	uint8_t H;
	int n;
	int vofs;
	uint8_t P0 = PPU[0];
	uint32_t cand_lo, cand_hi;
	int i;

	H = 8;

	ns = sb = 0;

	vofs = (uint32_t)(P0 & 0x8 & (((P0 & 0x20) ^ 0x20) >> 2)) << 9;
	H += (P0 & 0x20) >> 2;

	/* Past line 255 (the pre-render line) every sprite is a candidate
	 * and the range test below decides. */
	if ((uint32_t)scanline < 256) {
		if (spr_lines_dirty || spr_lines_h != H)
			BuildSpriteLines(H);
		cand_lo = spr_line_lo[scanline];
		cand_hi = spr_line_hi[scanline];
	} else
		cand_lo = cand_hi = 0xFFFFFFFFu;

#ifdef HAVE_HDPACK
	if (hdnes_active)
		HDNes_SpriteFetchStart();
#endif

	if (!PPU_hook)
		while ((i = NextSprite(&cand_lo, &cand_hi)) >= 0) {
			spr = (SPR*)SPRAM + i;
			n = 63 - i;
			if ((uint32_t)(scanline - spr->y) >= H) continue;
			if (ns < maxsprites) {
				if (n == 63) sb = 1;

				{
					SPRB dst;
					uint8_t *C;
					int t;
					uint32_t vadr;

					t = (int)scanline - (spr->y);

					if (Sprite16)
						vadr = ((spr->no & 1) << 12) + ((spr->no & 0xFE) << 4);
					else
						vadr = (spr->no << 4) + vofs;

					if (spr->atr & V_FLIP) {
						vadr += 7;
						vadr -= t;
						vadr += (P0 & 0x20) >> 1;
						vadr -= t & 8;
					} else {
						vadr += t;
						vadr += t & 8;
					}

					/* Fix this geniestage hack */
					if (MMC5Hack && geniestage != 1)
						C = MMC5SPRVRAMADR(vadr);
					else
						C = VRAMADR(vadr);

					dst.ca[0] = C[0];
					dst.ca[1] = C[8];
					dst.x = spr->x;
					dst.atr = spr->atr;

					/* memcpy avoids the strict-aliasing UB of casting
					 * a uint8_t buffer and a struct SPRB through a
					 * uint32_t* and also dodges the alignment trap on
					 * strict-alignment hosts. The compiler emits the
					 * same single 32-bit move on x86/ARM. */
					memcpy(&SPRBUF[ns << 2], &dst, 4);

#ifdef HAVE_HDPACK
					/* HD packs: record the sprite for next
					 * line's per-pixel assembly.  vadr has
					 * v-flip and the 8x16 half applied, so
					 * (vadr & 7) is the source row and
					 * C - (vadr & 7) the 16-byte tile base. */
					if (hdnes_active)
						HDNes_RecordSprite(ns, spr->x, spr->atr,
							C - (vadr & 7), (uint8_t)(vadr & 7),
							dst.ca[0], dst.ca[1]);
#endif
				}

				ns++;
			} else {
				PPU_status |= 0x20;
				break;
			}
		}
	else
		while ((i = NextSprite(&cand_lo, &cand_hi)) >= 0) {
			spr = (SPR*)SPRAM + i;
			n = 63 - i;
			if ((uint32_t)(scanline - spr->y) >= H) continue;

			if (ns < maxsprites) {
				if (n == 63) sb = 1;

				{
					SPRB dst;
					uint8_t *C;
					int t;
					uint32_t vadr;

					t = (int)scanline - (spr->y);

					if (Sprite16)
						vadr = ((spr->no & 1) << 12) + ((spr->no & 0xFE) << 4);
					else
						vadr = (spr->no << 4) + vofs;

					if (spr->atr & V_FLIP) {
						vadr += 7;
						vadr -= t;
						vadr += (P0 & 0x20) >> 1;
						vadr -= t & 8;
					} else {
						vadr += t;
						vadr += t & 8;
					}

					if (MMC5Hack)
						C = MMC5SPRVRAMADR(vadr);
					else
						C = VRAMADR(vadr);
					dst.ca[0] = C[0];
					if (ns < 8) {
						PPU_hook(0x2000);
						PPU_hook(vadr);
					}
					dst.ca[1] = C[8];
					dst.x = spr->x;
					dst.atr = spr->atr;


					memcpy(&SPRBUF[ns << 2], &dst, 4);

#ifdef HAVE_HDPACK
					/* HD packs: record the sprite for next
					 * line's per-pixel assembly.  vadr has
					 * v-flip and the 8x16 half applied, so
					 * (vadr & 7) is the source row and
					 * C - (vadr & 7) the 16-byte tile base. */
					if (hdnes_active)
						HDNes_RecordSprite(ns, spr->x, spr->atr,
							C - (vadr & 7), (uint8_t)(vadr & 7),
							dst.ca[0], dst.ca[1]);
#endif
				}

				ns++;
			} else {
				PPU_status |= 0x20;
				break;
			}
		}

	/* Handle case when >8 sprites per scanline option is enabled. */
	if (ns > 8) PPU_status |= 0x20;
	else if (PPU_hook) {
		for (n = 0; n < (8 - ns); n++) {
			PPU_hook(0x2000);
			PPU_hook(vofs);
		}
	}
	numsprites = ns;
	SpriteBlurp = sb;
}

static void RefreshSprites(void) {
	int n;
	SPRB *spr;

	spork = 0;
	if (!numsprites) return;

	FCEU_dwmemset(sprlinebuf, 0x80808080, 256);
	numsprites--;
	spr = (SPRB*)SPRBUF + numsprites;
	spr_xmin = 256;
	spr_xmax = 0;

	for (n = numsprites; n >= 0; n--, spr--) {
		register uint32_t pixdata;
		register uint8_t J, atr;

		int x = spr->x;
		uint8_t *C;
		uint8_t *VB;

		pixdata = ppulut1[spr->ca[0]] | ppulut2[spr->ca[1]];
		J = spr->ca[0] | spr->ca[1];
		atr = spr->atr;

		if (J) {
			if (x < spr_xmin)
				spr_xmin = x;
			if (x + 8 > spr_xmax)
				spr_xmax = x + 8;
			if (n == 0 && SpriteBlurp && !(PPU_status & 0x40)) {
				sphitx = x;
				sphitdata = J;
				if (atr & H_FLIP)
					sphitdata = ((J << 7) & 0x80) |
								((J << 5) & 0x40) |
								((J << 3) & 0x20) |
								((J << 1) & 0x10) |
								((J >> 1) & 0x08) |
								((J >> 3) & 0x04) |
								((J >> 5) & 0x02) |
								((J >> 7) & 0x01);
			}

			C = sprlinebuf + x;
			VB = (PALRAM + 0x10) + ((atr & 3) << 2);

			if (atr & SP_BACK) {
				if (atr & H_FLIP) {
					if (J & 0x80) C[7] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x40) C[6] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x20) C[5] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x10) C[4] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x08) C[3] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x04) C[2] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x02) C[1] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x01) C[0] = VB[pixdata] | 0x40;
				} else {
					if (J & 0x80) C[0] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x40) C[1] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x20) C[2] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x10) C[3] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x08) C[4] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x04) C[5] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x02) C[6] = VB[pixdata & 3] | 0x40;
					pixdata >>= 4;
					if (J & 0x01) C[7] = VB[pixdata] | 0x40;
				}
			} else {
				if (atr & H_FLIP) {
					if (J & 0x80) C[7] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x40) C[6] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x20) C[5] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x10) C[4] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x08) C[3] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x04) C[2] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x02) C[1] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x01) C[0] = VB[pixdata];
				} else {
					if (J & 0x80) C[0] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x40) C[1] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x20) C[2] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x10) C[3] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x08) C[4] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x04) C[5] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x02) C[6] = VB[pixdata & 3];
					pixdata >>= 4;
					if (J & 0x01) C[7] = VB[pixdata];
				}
			}
		}
	}
	SpriteBlurp = 0;
	spork = 1;
}

static void CopySprites(uint8_t *target) {
	int n = ((PPU[1] & 4) ^ 4) << 1;
	int end;
	uint8_t *P = target;

	if (!spork) return;
	spork = 0;

	if (rendis & 1) return;	/* User asked to not display sprites. */

	/* Only the 4-byte groups sprites drew into. */
	if (n < (spr_xmin & ~3))
		n = spr_xmin & ~3;
	end = (spr_xmax + 3) & ~3;
	if (end > 256)
		end = 256;

	for (; n < end; n += 4)
	{
		uint32_t t;
		memcpy(&t, sprlinebuf + n, 4);

		if (t != 0x80808080) {
			#ifdef MSB_FIRST
			if (!(t & 0x80000000)) {
				if (!(t & 0x40000000) || (P[n] & 64))	/* Normal sprite || behind bg sprite */
					P[n] = sprlinebuf[n];
			}

			if (!(t & 0x800000)) {
				if (!(t & 0x400000) || (P[n + 1] & 64))	/* Normal sprite || behind bg sprite */
					P[n + 1] = (sprlinebuf + 1)[n];
			}

			if (!(t & 0x8000)) {
				if (!(t & 0x4000) || (P[n + 2] & 64))		/* Normal sprite || behind bg sprite */
					P[n + 2] = (sprlinebuf + 2)[n];
			}

			if (!(t & 0x80)) {
				if (!(t & 0x40) || (P[n + 3] & 64))		/* Normal sprite || behind bg sprite */
					P[n + 3] = (sprlinebuf + 3)[n];
			}
			#else

			if (!(t & 0x80)) {
				if (!(t & 0x40) || (P[n] & 0x40))		/* Normal sprite || behind bg sprite */
					P[n] = sprlinebuf[n];
			}

			if (!(t & 0x8000)) {
				if (!(t & 0x4000) || (P[n + 1] & 0x40))		/* Normal sprite || behind bg sprite */
					P[n + 1] = (sprlinebuf + 1)[n];
			}

			if (!(t & 0x800000)) {
				if (!(t & 0x400000) || (P[n + 2] & 0x40))	/* Normal sprite || behind bg sprite */
					P[n + 2] = (sprlinebuf + 2)[n];
			}

			if (!(t & 0x80000000)) {
				if (!(t & 0x40000000) || (P[n + 3] & 0x40))	/* Normal sprite || behind bg sprite */
					P[n + 3] = (sprlinebuf + 3)[n];
			}
			#endif
		}
	}
}

void FCEUPPU_SetVideoSystem(int w) {
	if (w) {
		scanlines_per_frame = dendy ? 262 : 312;
		FSettings.FirstSLine = FSettings.UsrFirstSLine[1];
		FSettings.LastSLine = FSettings.UsrLastSLine[1];
	} else {
		scanlines_per_frame = 262;
		FSettings.FirstSLine = FSettings.UsrFirstSLine[0];
		FSettings.LastSLine = FSettings.UsrLastSLine[0];
	}
}

void FCEUPPU_Init(void) {
	makeppulut();
}

void FCEUPPU_Reset(void) {
	VRAMBuffer = PPU[0] = PPU[1] = PPU_status = PPU[3] = 0;
	PPUSPL = 0;
	PPUGenLatch = 0;
	RefreshAddr = TempAddr = 0;
	vtoggle = 0;
	ppudead = 2;
	kook = 0;
	ppu_frame_has_tint = 0;
	memset(ppu_frame_tint_lines, 0, sizeof(ppu_frame_tint_lines));
}

void FCEUPPU_Power(void) {
	int x;

	memset(NTARAM, 0x00, 0x800);
	memset(PALRAM, 0x00, 0x20);
	memset(UPALRAM, 0x00, 0x03);
	memset(SPRAM, 0x00, 0x100);
	spr_lines_dirty = 1;
	FCEUPPU_Reset();
	for (x = 0x2000; x < 0x4000; x += 8) {
		ARead[x] = A200x;
		BWrite[x] = B2000;
		ARead[x + 1] = A200x;
		BWrite[x + 1] = B2001;
		ARead[x + 2] = A2002;
		BWrite[x + 2] = B2002;
		ARead[x + 3] = A200x;
		BWrite[x + 3] = B2003;
		ARead[x + 4] = A200x;	/* A2004; */
		BWrite[x + 4] = B2004;
		ARead[x + 5] = A200x;
		BWrite[x + 5] = B2005;
		ARead[x + 6] = A200x;
		BWrite[x + 6] = B2006;
		ARead[x + 7] = A2007;
		BWrite[x + 7] = B2007;
	}

	BWrite[0x4014] = B4014;
}


int FCEUPPU_Loop(int skip) {
	/* Needed for Knight Rider, possibly others. */
	if (ppudead) {
		memset(XBuf, 0x80, 256 * 240);
		X6502_Run(scanlines_per_frame * (256 + 85));
		ppudead--;
	} else {
		X6502_Run(256 + 85);
		PPU_status |= 0x80;

		/* Not sure if this is correct.  According to Matt Conte and my own tests, it is.
		 * Timing is probably off, though.
		 * NOTE:  Not having this here breaks a Super Donkey Kong game.
		 */
		PPU[3] = PPUSPL = 0;

		/* I need to figure out the true nature and length of this delay. */
		X6502_Run(12);
		if (GameInfo->type == GIT_NSF)
			DoNSFFrame();
		else {
			if (VBlankON)
				TriggerNMI();
		}
		X6502_Run((scanlines_per_frame - 242) * (256 + 85) - 12);
		if (overclock_enabled && vblankscanlines) {
			if (!DMC_7bit || !skip_7bit_overclocking) {
				overclocked = 1;
				X6502_Run(vblankscanlines * (256 + 85) - 12);
				overclocked = 0;
			}
		}
		PPU_status &= 0x1f;
		X6502_Run(256);

		{
			int x;

			if (ScreenON || SpriteON) {
				if (GameHBIRQHook && ((PPU[0] & 0x38) != 0x18))
					GameHBIRQHook();
				if (PPU_hook)
					for (x = 0; x < 42; x++) {
						PPU_hook(0x2000); PPU_hook(0);
					}
				if (GameHBIRQHook2)
					GameHBIRQHook2();
			}
			X6502_Run(85 - 16);
			if (ScreenON || SpriteON) {
				RefreshAddr = TempAddr;
				if (PPU_hook) PPU_hook(RefreshAddr & 0x3fff);
			}

			/* Clean this stuff up later. */
			spork = numsprites = 0;
			ResetRL(XBuf);

			X6502_Run(16 - kook);
			kook ^= 1;
		}
		if (GameInfo->type == GIT_NSF)
			X6502_Run((256 + 85) * normal_scanlines);
		else {
			int x, max, maxref;

			deemp = PPU[1] >> 5;
			ppu_frame_has_tint = 0;

         /* manual samples can't play correctly with overclocking */
			if (DMC_7bit && skip_7bit_overclocking)
				totalscanlines = normal_scanlines;
			else
				totalscanlines = normal_scanlines + (overclock_enabled ? extrascanlines : 0);

			for (scanline = 0; (unsigned)scanline < totalscanlines; ) {	/* scanline is incremented in  DoLine.  Evil. :/ */
				deempcnt[deemp]++;
				DoLine();
				if ((unsigned)scanline < normal_scanlines || (unsigned)scanline == totalscanlines)
					overclocked = 0;
				else {
					if (DMC_7bit && skip_7bit_overclocking) /* 7bit sample started after 240th line */
						break;
					overclocked = 1;
				}
			}

			DMC_7bit = 0;
			if (MMC5Hack && (ScreenON || SpriteON)) MMC5_hb(scanline);
			for (x = 1, max = 0, maxref = 0; x < 7; x++) {
				if (deempcnt[x] > max) {
					max = deempcnt[x];
					maxref = x;
				}
				deempcnt[x] = 0;
			}
			SetNESDeemph(maxref, 0);
		}
	}

	{
		FCEU_PutImage();
		return(1);
	}
}

static uint16_t TempAddrT, RefreshAddrT;

void FCEUPPU_LoadState(int version) {
	spr_lines_dirty = 1;
	TempAddr = TempAddrT;
	RefreshAddr = RefreshAddrT;
}

SFORMAT FCEUPPU_STATEINFO[] = {
	{ NTARAM, 0x800, "NTAR" },
	{ PALRAM, 0x20, "PRAM" },
	{ SPRAM, 0x100, "SPRA" },
	{ PPU, 0x4, "PPUR" },
	{ &kook, 1, "KOOK" },
	{ &ppudead, 1, "DEAD" },
	{ &PPUSPL, 1, "PSPL" },
	{ &XOffset, 1, "XOFF" },
	{ &vtoggle, 1, "VTGL" },
	{ &RefreshAddrT, 2 | FCEUSTATE_RLSB, "RADD" },
	{ &TempAddrT, 2 | FCEUSTATE_RLSB, "TADD" },
	{ &VRAMBuffer, 1, "VBUF" },
	{ &PPUGenLatch, 1, "PGEN" },
	{ 0 }
};

void FCEUPPU_SaveState(void) {
	TempAddrT = TempAddr;
	RefreshAddrT = RefreshAddr;
}
