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
#include "n163sound.h"

static uint8 IRAM[128];

static uint8 ram_addr;
static uint8 sound_disabled;

static void DoN163Sound(int32 *Wave, int Count);
static void DoN163SoundHQ(void);

static uint32 FreqCache[8];
static uint32 EnvCache[8];
static uint32 LengthCache[8];

static void FixCache(int a, int V) {
	int w = (a >> 3) & 0x7;
	switch (a & 0x07) {
	case 0x00:
		FreqCache[w] &= ~0x000000FF;
		FreqCache[w] |= V;
		break;
	case 0x02:
		FreqCache[w] &= ~0x0000FF00;
		FreqCache[w] |= V << 8;
		break;
	case 0x04:
		FreqCache[w] &= ~0x00030000;
		FreqCache[w] |= (V & 3) << 16;
		LengthCache[w] = 256 - (V & 0xFC);
		break;
	case 0x07:
		EnvCache[w] = (double)(V & 0xF) * 576716;
		break;
	}
}

static int dwave = 0;

static void N163SoundHack(void) {
	int32 z, a;
	if (FSettings.soundq >= 1) {
		DoN163SoundHQ();
		return;
	}
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	if (a) DoN163Sound(&Wave[dwave], a);
	dwave += a;
}

static void N163Sound(int Count) {
	int32 z, a;
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	if (a) DoN163Sound(&Wave[dwave], a);
	dwave = 0;
}

static uint32 PlayIndex[8];
static int32 vcount[8];
static int32 CVBC;

#define TOINDEX        (16 + 1)

static SFORMAT N163Snd_StateRegs[] =
{
	{ PlayIndex, 32, "IDX0" },
	{ vcount, 32, "VCT0" },
	{ &CVBC, 4, "BC00" },
	{ &ram_addr, 1, "RADR" },
    { &sound_disabled, 1, "SOFF" },

	{ 0 }
};

/* 16:15 */
static void SyncHQ(int32 ts) {
	CVBC = ts;
}

/* Things to do:
	1        Read freq low
	2        Read freq mid
	3        Read freq high
	4        Read envelope
	...?
*/

static INLINE uint32 FetchDuffHQ(uint32 P, uint32 envelope) {
	uint32 duff;
	uint8 sample = (IRAM[0x46 + (P << 3)] + (PlayIndex[P] >> TOINDEX)) & 0xFF;
	if (sample & 0x01) {
		duff = IRAM[sample >> 1] >> 4;
	} else {
		duff = IRAM[sample >> 1] & 0x0F;
	}
	duff = (duff * envelope) >> 16;
	return (uint32)GetVolume(APU_N163, duff);
}

static void DoN163SoundHQ(void) {
	int32 P, V;
	int32 cyclesuck = (((IRAM[0x7F] >> 4) & 7) + 1) * 15;

    if (sound_disabled) {
        return;
    }

	for (P = 7; P >= (7 - ((IRAM[0x7F] >> 4) & 7)); P--) {
		if ((IRAM[0x44 + (P << 3)] & 0xE0) && (IRAM[0x47 + (P << 3)] & 0xF)) {
			uint32 freq;
			int32 vco;
			uint32 duff2, lengo, envelope;

			vco = vcount[P];
			freq = FreqCache[P];
			envelope = EnvCache[P];
			lengo = LengthCache[P];

			duff2 = FetchDuffHQ(P, envelope);
			for (V = CVBC << 1; V < (int)SOUNDTS << 1; V++) {
				WaveHi[V >> 1] += duff2;
				if (!vco) {
					PlayIndex[P] += freq;
					while ((PlayIndex[P] >> TOINDEX) >= lengo) PlayIndex[P] -= lengo << TOINDEX;
					duff2 = FetchDuffHQ(P, envelope);
					vco = cyclesuck;
				}
				vco--;
			}
			vcount[P] = vco;
		}
	}
	CVBC = SOUNDTS;
}

static INLINE uint32 FetchDuff(uint32 P, uint32 envelope) {
	uint32 duff;
	uint8 sample = (IRAM[0x46 + (P << 3)] + PlayIndex[P]) & 0xFF;
	if (sample & 0x01) {
		duff = IRAM[sample >> 1] >> 4;
	} else {
		duff = IRAM[sample >> 1] & 0x0F;
	}
	duff = (duff * envelope) >> 19;
	return (uint32)GetVolume(APU_N163, duff);
}

static void DoN163Sound(int32 *Wave, int Count) {
	int P, V;

    if (sound_disabled) {
        return;
    }

	for (P = 7; P >= 7 - ((IRAM[0x7F] >> 4) & 7); P--) {
		if ((IRAM[0x44 + (P << 3)] & 0xE0) && (IRAM[0x47 + (P << 3)] & 0xF)) {
			int32 inc;
			uint32 freq;
			int32 vco;
			uint32 duff2, lengo, envelope;

			vco = vcount[P];
			freq = FreqCache[P];
			envelope = EnvCache[P];
			lengo = LengthCache[P];

			if (!freq)
				continue;

			{
				int c = ((IRAM[0x7F] >> 4) & 7) + 1;
				inc = (long double)(FSettings.SndRate << 15) / ((long double)freq * 21477272 / ((long double)0x400000 * c * 45));
			}

			duff2 = FetchDuff(P, envelope);
			for (V = 0; V < Count * 16; V++) {
				if (vco >= inc) {
					PlayIndex[P]++;
					if (PlayIndex[P] >= lengo)
						PlayIndex[P] = 0;
					vco -= inc;
					duff2 = FetchDuff(P, envelope);
				}
				Wave[V >> 4] += duff2;
				vco += 0x8000;
			}
			vcount[P] = vco;
		}
	}
}

uint8 *GetIRAM_ptr(void) {
    return IRAM;
}

uint32 GetIRAM_size(void) {
    return sizeof(IRAM);
}

void N163Sound_FixCache(void) {
	int x;
	for (x = 0x40; x < 0x80; x++) {
		FixCache(x, IRAM[x]);
    }
}

void N163Sound_Reset(void) {
    memset(vcount, 0, sizeof(vcount));
	memset(PlayIndex, 0, sizeof(PlayIndex));
	CVBC = 0;
}

DECLFR(N163Sound_Read) {
	uint8 ret = IRAM[ram_addr & 0x7f];
	/* Maybe I should call N163SoundHack() here? */
	#ifdef FCEUDEF_DEBUGGER
	if (!fceuindbg)
	#endif
	if (ram_addr & 0x80) {
		ram_addr = (ram_addr & 0x80) | ((ram_addr + 1) & 0x7f);
	}
	return ret;
}

DECLFW(N163Sound_Write) {
	switch (A & 0xF800) {
	case 0x4800:
		if (ram_addr & 0x40) {
			if (FSettings.SndRate) {
				N163SoundHack();
				GameExpSound.Fill = N163Sound;
				GameExpSound.HiFill = DoN163SoundHQ;
				GameExpSound.HiSync = SyncHQ;
			}
			FixCache(ram_addr, V);
		}
		IRAM[ram_addr & 0x7f] = V;
		if (ram_addr & 0x80) {
			ram_addr = (ram_addr & 0x80) | ((ram_addr + 1) & 0x7f);
		}
		break;
	case 0xE000:
        sound_disabled = (V & 0x40) == 0x40;
		break;
	case 0xF800:
		ram_addr = V;
		break;
	}
}

static void M19SC(void) {
	if (FSettings.SndRate) {
		N163Sound_Reset();
	}
}

void N163Sound_ESI(void) {
	GameExpSound.RChange = M19SC;

	ram_addr = 0x7F;
    sound_disabled = 0;

    N163Sound_Reset();

    FCEU_MemoryRand(IRAM, sizeof(IRAM));
	AddExState(IRAM, sizeof(IRAM), 0, "IRAM");
	AddExState(N163Snd_StateRegs, ~0, 0, 0);

	N163Sound_FixCache();
}
