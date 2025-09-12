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

/* 2025-09-12 - negativeExponent
   Ported modulation/sweep logic from Mednafen NES, adapted for FCEUmm.
 */

/* Begin FDS sound */
#include <string.h>
#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "sound.h"
#include "state.h"

#define FDSClock (1789772.7272727272727272 / 2)

typedef struct {
	int64 cycles;		/* Cycles per PCM sample */
	int64 count;		/* Cycle counter */
	int64 envcount;		/* Envelope cycle counter */
	uint8 amplitude[2];	/* Current amplitudes. */
	uint8 speedo[2];
	uint8 mwave[0x20];	/* Modulation waveform */
	uint8 cwave[0x40];	/* Game-defined waveform(carrier) */
	uint8 SPSG[0xB];

	uint32 cwave_freq;	/* $4082 and lower 4 bits of $4083 */
	uint32 cwave_pos;

	uint32 mod_freq;	/* $4086 and lower 4 bits of $4087 */
	uint32 mod_pos;		/* Should be named "mwave_pos", but "mod_pos" distinguishes it more. */
	uint8 mod_disabled;	/* Upper bit of $4087 */

	uint32 sweep_bias;

	int32 mod_out;		/* output from modulator */
	uint32 sample_cache_out; /* Sample out cache, with volume, modulation, and others applied */
} FDSSOUND;

static FDSSOUND fdso;

static const int bias_tab[8] = { 0, 1, 2, 4, 0, -4, -2, -1 };

#define  SPSG  fdso.SPSG
#define b19shiftreg60  fdso.b19shiftreg60
#define b24adder66  fdso.b24adder66
#define b24latch68  fdso.b24latch68
#define b17latch76  fdso.b17latch76
#define b8shiftreg88  fdso.b8shiftreg88
#define clockcount  fdso.clockcount
#define amplitude  fdso.amplitude
#define speedo    fdso.speedo

#define cwave_freq fdso.cwave_freq
#define cwave_pos fdso.cwave_pos

#define mod_freq fdso.mod_freq
#define mod_pos fdso.mod_pos
#define mod_disabled fdso.mod_disabled

#define sweep_bias fdso.sweep_bias

void FDSSoundStateAdd(void) {
	AddExState(fdso.cwave, 64, 0, "WAVE");
	AddExState(fdso.mwave, 32, 0, "MWAV");
	AddExState(amplitude, 2, 0, "AMPL");
	AddExState(SPSG, 0xB, 0, "SPSG");

	AddExState(&cwave_freq, 4, 1, "CFRQ");
	AddExState(&cwave_pos, 4, 1, "CPOS");

	AddExState(&mod_freq, 4, 1, "MFRQ");
	AddExState(&mod_pos, 4, 1, "MPOS");
	AddExState(&mod_disabled, 1, 1, "MDIS");

	AddExState(&sweep_bias, 4, 1, "BIAS");

	AddExState(&fdso.mod_out, 4, 1, "MOUT");
	AddExState(&fdso.sample_cache_out, 4, 1, "WOUT");
}

static DECLFR(FDSSRead) {
	switch (A & 0xF) {
	case 0x0: return(amplitude[0] | (X.DB & 0xC0));
	case 0x2: return(amplitude[1] | (X.DB & 0xC0));
	}
	return(X.DB);
}

static void RenderSound(void);
static void RenderSoundHQ(void);

static DECLFW(FDSSWrite) {
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1)
			RenderSoundHQ();
		else
			RenderSound();
	}
	A -= 0x4080;
	switch (A) {
	case 0x0:
	case 0x4:
		if (V & 0x80)
			amplitude[(A & 0xF) >> 2] = V & 0x3F;
		break;
	case 0x2:
		cwave_freq &= 0xFF00;
		cwave_freq |= V << 0;
		break;

	case 0x3:
		if (!(V & 0x80) && (SPSG[0x3] & 0x80))
			cwave_pos = 0;

		cwave_freq &= 0x00FF;
		cwave_freq |= (V & 0xF) << 8;
		break;

	case 0x5:
		sweep_bias = (V & 0x7F) << 4;
		mod_pos = 0;
		break;

	case 0x6:
		mod_freq &= 0xFF00;
		mod_freq |= V << 0;
		break;

	case 0x7:
		mod_freq &= 0x00FF;
		mod_freq |= (V & 0xF) << 8;
		mod_disabled = (bool)(V & 0x80);
		break;

	case 0x8:
		if(mod_disabled) {
			int i;

			for(i = 0; i < 31; i++)
				fdso.mwave[i] = fdso.mwave[i + 1];

			fdso.mwave[0x1F] = bias_tab[V & 0x7];

			if((V & 0x7) == 0x4)
				fdso.mwave[0x1F] = 0x10;
		}
		break;
	}
	SPSG[A] = V;
}

/* $4080 - Fundamental wave amplitude data register 92
 * $4082 - Fundamental wave frequency data register 58
 * $4083 - Same as $4082($4083 is the upper 4 bits).
 *
 * $4084 - Modulation amplitude data register 78
 * $4086 - Modulation frequency data register 72
 * $4087 - Same as $4086($4087 is the upper 4 bits)
 */


static void DoEnv() {
	int x;

	for (x = 0; x < 2; x++)
		if (!(SPSG[x << 2] & 0x80) && !(SPSG[0x3] & 0x40)) {
			static int counto[2] = { 0, 0 };

			if (counto[x] <= 0) {
				if (!(SPSG[x << 2] & 0x80)) {
					if (SPSG[x << 2] & 0x40) {
						if (amplitude[x] < 0x3F)
							amplitude[x]++;
					} else {
						if (amplitude[x] > 0)
							amplitude[x]--;
					}
				}
				counto[x] = (SPSG[x << 2] & 0x3F);
			} else
				counto[x]--;
		}
}

static DECLFR(FDSWaveRead) {
	return(fdso.cwave[A & 0x3f] | (X.DB & 0xC0));
}

static DECLFW(FDSWaveWrite) {
	if (SPSG[0x9] & 0x80)
		fdso.cwave[A & 0x3f] = V & 0x3F;
}

static INLINE int32 sign_x_to_s32(int n, int32 v) {
	return ((int32)((uint32)v << (32 - n)) >> (32 - n));
}

static INLINE void ClockMod(void) {
	if (!mod_disabled) {
		uint32 prev_mod_pos = mod_pos;
		int32 temp;

		mod_pos += mod_freq;

		if ((mod_pos & (0x3F << 11)) != (prev_mod_pos & (0x3F << 11))) {
			const int32 mw = fdso.mwave[((mod_pos >> 16) & 0x1F)];

			sweep_bias = (sweep_bias + mw) & 0x7FF;
			if (mw == 0x10)
				sweep_bias = 0;
		}

		temp = sign_x_to_s32(11, sweep_bias) * ((amplitude[1] > 0x20) ? 0x20 : amplitude[1]);

		// >> 4 or / 16?  / 16 sounds better in Zelda...
		if (temp & 0x0F0) {
			temp /= 256;
			if (sweep_bias & 0x400)
				temp--;
			else
				temp += 2;
		} else
			temp /= 256;

		if (temp >= 194) {
			temp -= 258;
		}
		if (temp < -64) {
			temp += 256;
		}

		fdso.mod_out = temp;
	}
}

static INLINE void ClockCarrier(void) {
	int32 cur_cwave_freq;

	if (!mod_disabled) {
		cur_cwave_freq = (int32)(cwave_freq << 6) + (int32)cwave_freq * fdso.mod_out;

		if (cur_cwave_freq < 0)
			cur_cwave_freq = 0;
	} else
		cur_cwave_freq = cwave_freq << 6;

	cwave_pos += cur_cwave_freq;
}

static INLINE int32 FDSDoSound(void) {
	uint32 prev_cwave_pos = cwave_pos;

	fdso.count += fdso.cycles;
	if (fdso.count >= ((int64)1 << 40)) {
 dogk:
		fdso.count -= (int64)1 << 40;

		ClockMod();

		if (!(SPSG[0x9] & 0x80) && !(SPSG[0x3] & 0x80))
			ClockCarrier();
		
		fdso.envcount--;
		if (fdso.envcount <= 0) {
			fdso.envcount += SPSG[0xA] * 3;
			DoEnv();
		}
	}
	if (fdso.count >= 32768) goto dogk;

	/* Might need to emulate applying the amplitude to the waveform a bit better... */
	if ((cwave_pos ^ prev_cwave_pos) & (1 << 21)) {
		int k = amplitude[0];
		if (k > 0x20) k = 0x20;
		fdso.sample_cache_out = (fdso.cwave[(cwave_pos >> 21) & 0x3F] * k) * 4 / ((SPSG[0x9] & 0x3) + 2);
	}

	return fdso.sample_cache_out;
}

static int32 FBC = 0;

static void RenderSound(void) {
	int32 end, start;
	int32 x;

	start = FBC;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start)
		return;
	FBC = end;

	if (!(SPSG[0x9] & 0x80))
		for (x = start; x < end; x++) {
			uint32 t = FDSDoSound();
			t += t >> 1;
			t >>= 4;
			Wave[x >> 4] += t;	/* (t>>2)-(t>>3); */ /* >>3; */
		}
}

static void RenderSoundHQ(void) {
	uint32 x;

	if (!(SPSG[0x9] & 0x80))
		for (x = FBC; x < SOUNDTS; x++) {
			uint32 t = FDSDoSound();
			t += t >> 1;
			WaveHi[x] += t;	/* (t<<2)-(t<<1); */
		}
	FBC = SOUNDTS;
}

static void HQSync(int32 ts) {
	FBC = ts;
}

void FDSSound(int c) {
	RenderSound();
	FBC = c;
}

static void FDS_ESI(void) {
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			fdso.cycles = (int64)1 << 39;
		} else {
			fdso.cycles = ((int64)1 << 40) * FDSClock;
			fdso.cycles /= FSettings.SndRate * 16;
		}
	}
	SetReadHandler(0x4040, 0x407f, FDSWaveRead);
	SetWriteHandler(0x4040, 0x407f, FDSWaveWrite);
	SetWriteHandler(0x4080, 0x408A, FDSSWrite);
	SetReadHandler(0x4090, 0x4092, FDSSRead);
}

void FDSSoundReset(void) {
	memset(&fdso, 0, sizeof(fdso));
	FDS_ESI();
	GameExpSound.HiSync = HQSync;
	GameExpSound.HiFill = RenderSoundHQ;
	GameExpSound.Fill = FDSSound;
	GameExpSound.RChange = FDS_ESI;
}

uint8 FDSSoundRead(uint32 A) {
	if (A >= 0x4040 && A < 0x4080) return FDSWaveRead(A);
	if (A >= 0x4090 && A < 0x4093) return FDSSRead(A);
	return X.DB;
}

void FDSSoundWrite(uint32 A, uint8 V) {
	if (A >= 0x4040 && A < 0x4080) FDSWaveWrite(A, V);
	else if (A >= 0x4080 && A < 0x408B) FDSSWrite(A, V);
}

void FDSSoundPower(void) {
	FDSSoundReset();
	FDSSoundStateAdd();
}
