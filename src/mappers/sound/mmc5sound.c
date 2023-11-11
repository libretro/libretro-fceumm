/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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
#include "mmc5sound.h"

MMC5SOUND MMC5Sound;

static void (*sfun)(int P);
static void (*psfun)(void);

static void Do5PCM(void) {
	int32 V;
	int32 start, end;

	start = MMC5Sound.BC[2];
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) return;
	MMC5Sound.BC[2] = end;

	if (!(MMC5Sound.rawcontrol & 0x40) && MMC5Sound.raw) {
		int32 amp = GetVolume(APU_MMC5, MMC5Sound.raw) << 1;
		for (V = start; V < end; V++)
			Wave[V >> 4] += amp;
	}
}

static void Do5PCMHQ(void) {
	uint32 V;
	if (!(MMC5Sound.rawcontrol & 0x40) && MMC5Sound.raw) {
		int32 amp = GetVolume(APU_MMC5, MMC5Sound.raw) << 5;
		for (V = MMC5Sound.BC[2]; V < SOUNDTS; V++)
			WaveHi[V] += amp;
	}
	MMC5Sound.BC[2] = SOUNDTS;
}

static void Do5SQ(int P) {
	static int tal[4] = { 1, 2, 4, 6 };
	int32 V, amp, rthresh, wl;
	int32 start, end;

	start = MMC5Sound.BC[P];
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) return;
	MMC5Sound.BC[P] = end;

	wl = MMC5Sound.wl[P] + 1;
	amp = (MMC5Sound.env[P] & 0xF) << 4;
	amp = GetVolume(APU_MMC5, amp);
	rthresh = tal[(MMC5Sound.env[P] & 0xC0) >> 6];

	if (wl >= 8 && (MMC5Sound.running & (P + 1))) {
		int dc, vc;

		wl <<= 18;
		dc = MMC5Sound.dcount[P];
		vc = MMC5Sound.vcount[P];

		for (V = start; V < end; V++) {
			if (dc < rthresh)
				Wave[V >> 4] += amp;
			vc -= nesincsize;
			while (vc <= 0) {
				vc += wl;
				dc = (dc + 1) & 7;
			}
		}
		MMC5Sound.dcount[P] = dc;
		MMC5Sound.vcount[P] = vc;
	}
}

static void Do5SQHQ(int P) {
	static int tal[4] = { 1, 2, 4, 6 };
	uint32 V;
	int32 amp, rthresh, wl;

	wl = MMC5Sound.wl[P] + 1;
	amp = ((MMC5Sound.env[P] & 0xF) << 8);
	amp = GetVolume(APU_MMC5, amp);
	rthresh = tal[(MMC5Sound.env[P] & 0xC0) >> 6];

	if (wl >= 8 && (MMC5Sound.running & (P + 1))) {
		int dc, vc;

		wl <<= 1;

		dc = MMC5Sound.dcount[P];
		vc = MMC5Sound.vcount[P];
		for (V = MMC5Sound.BC[P]; V < SOUNDTS; V++) {
			if (dc < rthresh)
				WaveHi[V] += amp;
			vc--;
			if (vc <= 0) { /* Less than zero when first started. */
				vc = wl;
				dc = (dc + 1) & 7;
			}
		}
		MMC5Sound.dcount[P] = dc;
		MMC5Sound.vcount[P] = vc;
	}
	MMC5Sound.BC[P] = SOUNDTS;
}

static void MMC5RunSoundHQ(void) {
	Do5SQHQ(0);
	Do5SQHQ(1);
	Do5PCMHQ();
}

static void MMC5HiSync(int32 ts) {
	int x;
	for (x = 0; x < 3; x++)
		MMC5Sound.BC[x] = ts;
}

static void MMC5RunSound(int Count) {
	int x;
	Do5SQ(0);
	Do5SQ(1);
	Do5PCM();
	for (x = 0; x < 3; x++)
		MMC5Sound.BC[x] = Count;
}

DECLFW(MMC5Sound_Write) {
	A &= 0x1F;

	GameExpSound.Fill   = MMC5RunSound;
	GameExpSound.HiFill = MMC5RunSoundHQ;

	switch (A) {
	case 0x10:
		if (psfun)
			psfun();
		MMC5Sound.rawcontrol = V;
		break;
	case 0x11:
		if (psfun)
			psfun();
		MMC5Sound.raw = V;
		break;

	case 0x0:
	case 0x4:
		if (sfun)
			sfun(A >> 2);
		MMC5Sound.env[A >> 2] = V;
		break;
	case 0x2:
	case 0x6:
		if (sfun)
			sfun(A >> 2);
		MMC5Sound.wl[A >> 2] &= ~0x00FF;
		MMC5Sound.wl[A >> 2] |= V & 0xFF;
		break;
	case 0x3:
	case 0x7:
		MMC5Sound.wl[A >> 2] &= ~0x0700;
		MMC5Sound.wl[A >> 2] |= (V & 0x07) << 8;
		MMC5Sound.running |= 1 << (A >> 2);
		break;
	case 0x15:
		if (sfun) {
			sfun(0);
			sfun(1);
		}
		MMC5Sound.running &= V;
		MMC5Sound.enable = V;
		break;
	}
}

void MMC5Sound_ESI(void) {
	GameExpSound.RChange = MMC5Sound_ESI;
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			sfun  = Do5SQHQ;
			psfun = Do5PCMHQ;
		} else {
			sfun  = Do5SQ;
			psfun = Do5PCM;
		}
	} else {
		sfun  = 0;
		psfun = 0;
	}
	memset(MMC5Sound.BC, 0, sizeof(MMC5Sound.BC));
	memset(MMC5Sound.vcount, 0, sizeof(MMC5Sound.vcount));
	GameExpSound.HiSync = MMC5HiSync;
}
