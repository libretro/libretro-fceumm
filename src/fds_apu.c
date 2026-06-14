/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *  Copyright (C) 2023-2026 negativeExponent (MDFN-derived FDS sound)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

/* MDFN-derived modulator/carrier model.  Replaces the previous Xodnizel
 * bit-serial b8shiftreg88 implementation; the latter lacked the
 * modulator's sweep-bias accumulator entirely, producing static pitch
 * shifts where games (e.g. VS. Excitebike FDS triangle voice, #560)
 * programmed slowly-developing pitch sweeps.  Backported from
 * libretro/fceumm_next/src/mappers/sound/fdssound.c (negativeExponent,
 * itself derived from Mednafen's NES FDS sound). */

#include <string.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "sound.h"
#include "filter.h"
#include "state.h"
#include "fds_apu.h"

#define FDSClock (1789772.7272727272727272 / 2)

enum { EVOL = 0, EMOD };
enum {
	ENV_CTRL_INCREASE = 0x40,
	ENV_CTRL_DISABLE  = 0x80,
	ENVELOPES_DISABLE = 0x40,
	WAVE_DISABLE      = 0x80,
	MOD_WRITE_MODE    = 0x80,
	WAVE_WRITE_MODE   = 0x80
};
enum {
	VOLUME_MIN = 0,
	VOLUME_MAX = 32
};

typedef struct {
	uint8_t  speed;
	uint8_t  volume;
	uint8_t  control;
	int32_t  counter;
} FDSENVUNIT;

typedef struct {
	int64_t cycles;
	int64_t count;

	FDSENVUNIT EnvUnits[2];

	int32_t env_divider;

	uint16_t cwave_freq;
	uint32_t cwave_pos;
	uint8_t  cwave_control;

	uint16_t mod_freq;
	uint32_t mod_pos;
	uint8_t  mod_disabled;

	uint8_t master_control;
	uint8_t master_env_speed;

	int32_t mwave[0x20];
	uint8_t cwave[0x40];

	uint32_t sweep_bias;
	int32_t  mod_output;
	int32_t  sample_out_cache;

	uint8_t cwave_pos_shift;
	uint8_t env_count_mul;
	uint8_t mod_pos_shift;
	uint8_t mod_overflow_shift;
} FDSSOUND;

static const int32_t mod_bias_tab[8] = { 0, 1, 2, 4, 0, -4, -2, -1 };

static FDSSOUND fdso;
static int32_t  FBC;

void FDSSoundStateAdd(void) {
	AddExState(&fdso.sample_out_cache, 4, 1, "FDSO");

	AddExState(&fdso.EnvUnits[EVOL].speed,   1, 0, "SPD0");
	AddExState(&fdso.EnvUnits[EVOL].control, 1, 0, "CTL0");
	AddExState(&fdso.EnvUnits[EVOL].volume,  1, 0, "VOL0");
	AddExState(&fdso.EnvUnits[EVOL].counter, 4, 1, "CNT0");

	AddExState(&fdso.EnvUnits[EMOD].speed,   1, 0, "SPD1");
	AddExState(&fdso.EnvUnits[EMOD].control, 1, 0, "CTL1");
	AddExState(&fdso.EnvUnits[EMOD].volume,  1, 0, "VOL1");
	AddExState(&fdso.EnvUnits[EMOD].counter, 4, 1, "CNT1");

	AddExState(fdso.cwave, 64, 0, "WAVE");
	AddExState(fdso.mwave, 32 * 4, 1, "MWAV");

	AddExState(&fdso.cwave_freq,    2, 1, "WFRQ");
	AddExState(&fdso.cwave_pos,     4, 1, "WPOS");
	AddExState(&fdso.cwave_control, 1, 0, "WCTL");

	AddExState(&fdso.mod_freq,     2, 1, "MFRQ");
	AddExState(&fdso.mod_pos,      4, 1, "MPOS");
	AddExState(&fdso.mod_disabled, 1, 0, "MDIS");
	AddExState(&fdso.mod_output,   4, 1, "MCRM");

	AddExState(&fdso.sweep_bias, 4, 1, "SWBS");

	AddExState(&fdso.master_control,   1, 0, "MCTL");
	AddExState(&fdso.master_env_speed, 1, 0, "MSPD");

	AddExState(&fdso.env_divider, 4, 1, "EDIV");

	AddExState(&fdso.cwave_pos_shift,    1, 0, "CWPS");
	AddExState(&fdso.env_count_mul,      1, 0, "ECNM");
	AddExState(&fdso.mod_pos_shift,      1, 0, "MPSH");
	AddExState(&fdso.mod_overflow_shift, 1, 0, "MOFS");
}

static void RenderSound(void);
static void RenderSoundHQ(void);

static void FDSSoundUpdate(void) {
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1)
			RenderSoundHQ();
		else
			RenderSound();
	}
}

static DECLFR(FDSWaveRead) {
	if (fdso.master_control & WAVE_WRITE_MODE)
		return (fdso.cwave[A & 0x3F] | (X.DB & 0xC0));
	return (fdso.cwave[(fdso.cwave_pos >> 21) & 0x3F] | (X.DB & 0xC0));
}

static DECLFW(FDSWaveWrite) {
	if (fdso.master_control & WAVE_WRITE_MODE)
		fdso.cwave[A & 0x3F] = V & 0x3F;
}

static DECLFW(FDSSReg0Write) {
	FDSSoundUpdate();

	fdso.EnvUnits[EVOL].speed   = V & 0x3F;
	fdso.EnvUnits[EVOL].control = V & 0xC0;
	fdso.EnvUnits[EVOL].counter = fdso.EnvUnits[EVOL].speed + 1;

	if (fdso.EnvUnits[EVOL].control & ENV_CTRL_DISABLE)
		fdso.EnvUnits[EVOL].volume = fdso.EnvUnits[EVOL].speed;
}

static DECLFW(FDSSReg1Write) {
	FDSSoundUpdate();

	fdso.cwave_freq &= 0x0F00;
	fdso.cwave_freq |= V;
}

static DECLFW(FDSSReg2Write) {
	FDSSoundUpdate();

	fdso.cwave_freq &= 0x00FF;
	fdso.cwave_freq |= (V & 0x0F) << 8;

	fdso.cwave_control = V & 0xC0;

	if (fdso.cwave_control & WAVE_DISABLE)
		fdso.cwave_pos = 0;
}

static DECLFW(FDSSReg3Write) {
	FDSSoundUpdate();

	fdso.EnvUnits[EMOD].speed   = V & 0x3F;
	fdso.EnvUnits[EMOD].control = V & 0xC0;
	fdso.EnvUnits[EMOD].counter = fdso.EnvUnits[EMOD].speed + 1;

	if (fdso.EnvUnits[EMOD].control & ENV_CTRL_DISABLE)
		fdso.EnvUnits[EMOD].volume = fdso.EnvUnits[EMOD].speed;
}

static DECLFW(FDSSReg4Write) {
	FDSSoundUpdate();

	fdso.sweep_bias = (V & 0x7F) << 4;
	fdso.mod_pos    = 0;
}

static DECLFW(FDSSReg5Write) {
	FDSSoundUpdate();

	fdso.mod_freq &= 0x0F00;
	fdso.mod_freq |= V;
}

static DECLFW(FDSSReg6Write) {
	FDSSoundUpdate();

	fdso.mod_freq    &= 0x00FF;
	fdso.mod_freq    |= (V & 0x0F) << 8;
	fdso.mod_disabled = (V & MOD_WRITE_MODE) != 0;
	if (fdso.mod_disabled)
		fdso.mod_pos = 0;
}

static DECLFW(FDSSReg7Write) {
	FDSSoundUpdate();

	if (fdso.mod_disabled) {
		int i;
		for (i = 0; i < 31; i++)
			fdso.mwave[i] = fdso.mwave[i + 1];
		fdso.mwave[0x1F] = mod_bias_tab[V & 0x07];
		if ((V & 0x07) == 0x04)
			fdso.mwave[0x1F] = 0x10;
	}
}

static DECLFW(FDSSReg8Write) {
	FDSSoundUpdate();
	fdso.master_control = V;
}

static DECLFW(FDSSReg9Write) {
	FDSSoundUpdate();
	fdso.master_env_speed = V;
}

static DECLFR(FDSEnvVolumeRead) {
	FDSSoundUpdate();
	return (fdso.EnvUnits[EVOL].volume | (X.DB & 0xC0));
}

static DECLFR(FDSEnvModRead) {
	FDSSoundUpdate();
	return (fdso.EnvUnits[EMOD].volume | (X.DB & 0xC0));
}

static INLINE void FDSEnvStep(FDSENVUNIT *e) {
	if (!(e->control & ENV_CTRL_DISABLE)) {
		e->counter--;
		if (e->counter <= 0) {
			e->counter = e->speed + 1;
			if (e->control & ENV_CTRL_INCREASE) {
				if (e->volume < VOLUME_MAX) e->volume++;
			} else {
				if (e->volume > VOLUME_MIN) e->volume--;
			}
		}
	}
}

static void FDSDoEnv(void) {
	FDSEnvStep(&fdso.EnvUnits[EVOL]);
	FDSEnvStep(&fdso.EnvUnits[EMOD]);
}

static INLINE int32_t sign_x_to_s32(int n, int32_t v) {
	return ((int32_t)((uint32_t)v << (32 - n)) >> (32 - n));
}

static void FDSModUpdateOutput(void) {
	int32_t mod_amp = (fdso.EnvUnits[EMOD].volume > 0x20)
		? 0x20 : fdso.EnvUnits[EMOD].volume;
	int32_t temp = sign_x_to_s32(11, fdso.sweep_bias) * mod_amp;

	if (temp & 0x0F0) {
		temp /= 256;
		if (fdso.sweep_bias & 0x400)
			temp--;
		else
			temp += 2;
	} else {
		temp /= 256;
	}

	if (temp >= 194) temp -= 258;
	if (temp <  -64) temp += 256;

	fdso.mod_output = temp;
}

static int FDSDoModulator(void) {
	uint32_t prev_mod_pos = fdso.mod_pos;

	if (!fdso.mod_disabled) {
		uint32_t overflow_mask = 0x3F << fdso.mod_overflow_shift;
		fdso.mod_pos += fdso.mod_freq;
		if ((fdso.mod_pos & overflow_mask) != (prev_mod_pos & overflow_mask)) {
			const int32_t mw = fdso.mwave[(fdso.mod_pos >> fdso.mod_pos_shift) & 0x1F];
			fdso.sweep_bias = (fdso.sweep_bias + mw) & 0x7FF;
			if (mw == 0x10)
				fdso.sweep_bias = 0;
		}
		return 1;
	}
	return 0;
}

static INLINE void FDSDoCarrier(void) {
	int32_t cur_cwave_freq = (int32_t)(fdso.cwave_freq << 6);

	if (!fdso.mod_disabled) {
		cur_cwave_freq += (int32_t)fdso.cwave_freq * fdso.mod_output;
		if (cur_cwave_freq < 0)
			cur_cwave_freq = 0;
	}
	fdso.cwave_pos += cur_cwave_freq;
}

static void FDSClockUnits(void) {
	if (FDSDoModulator())
		FDSModUpdateOutput();
	if (!(fdso.master_control & WAVE_WRITE_MODE) &&
	    !(fdso.cwave_control & WAVE_DISABLE))
		FDSDoCarrier();
	if (fdso.master_env_speed) {
		fdso.env_divider--;
		if (fdso.env_divider <= 0) {
			fdso.env_divider += fdso.master_env_speed * fdso.env_count_mul;
			if (!(fdso.cwave_control & ENVELOPES_DISABLE))
				FDSDoEnv();
		}
	}
}

static void FDSDoSound(void) {
	uint32_t prev_cwave_pos = fdso.cwave_pos;

	if (FSettings.soundq >= 1) {
		FDSClockUnits();
	} else {
		fdso.count += fdso.cycles;
		if (fdso.count >= ((int64_t)1 << 40)) {
			fdso.count -= (int64_t)1 << 40;
			FDSClockUnits();
		}
		while (fdso.count >= 32768) {
			fdso.count -= (int64_t)1 << 40;
			FDSClockUnits();
		}
	}

	if ((fdso.cwave_pos ^ prev_cwave_pos) & (1U << fdso.cwave_pos_shift)) {
		/* Master-volume divisor LUT: replaces an integer divide
		 * with a multiply + shift.  Indices match $4089 bits 0-1:
		 * 0 -> div 2, 1 -> div 3, 2 -> div 4, 3 -> div 5. */
		static const uint32_t fds_div_lut[4] = {
			(1U << 16) / 2,
			(1U << 16) / 3,
			(1U << 16) / 4,
			(1U << 16) / 5
		};
		int32_t k   = fdso.EnvUnits[EVOL].volume;
		int32_t idx = fdso.master_control & 0x03;
		int32_t sample;

		if (k > 0x20) k = 0x20;
		sample = fdso.cwave[(fdso.cwave_pos >> fdso.cwave_pos_shift) & 0x3F]
		         * k * 4;
		sample = (int32_t)((uint32_t)sample * fds_div_lut[idx]) >> 16;
		fdso.sample_out_cache = GetExpOutput(SND_FDS, sample);
	}
}

static void RenderSound(void) {
	int32_t end = (SOUNDTS << 16) / soundtsinc;
	int32_t start = FBC;
	int32_t x;

	if (end <= start) return;
	FBC = end;

	for (x = start; x < end; x++) {
		uint32_t t;
		FDSDoSound();
		t  = (uint32_t)fdso.sample_out_cache;
		t += t >> 1;
		t >>= 4;
		Wave[x >> 4] += (int32_t)t;
	}
}

static void RenderSoundHQ(void) {
	uint32_t x;
	for (x = (uint32_t)FBC; x < (uint32_t)SOUNDTS; x++) {
		uint32_t t;
		FDSDoSound();
		t  = (uint32_t)fdso.sample_out_cache;
		t += t >> 1;
		WaveHi[x] += (int32_t)t;
	}
	FBC = (int32_t)SOUNDTS;
}

static void HQSync(int32_t ts) {
	FBC = ts;
}

static void FDSSound(int c) {
	RenderSound();
	FBC = c;
}

static void FDS_ESI(void) {
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			fdso.cycles             = (int64_t)1 << 39;
			fdso.cwave_pos_shift    = 22;
			fdso.mod_pos_shift      = 17;
			fdso.mod_overflow_shift = 12;
			fdso.env_count_mul      = 8;
		} else {
			fdso.cycles             = ((int64_t)1 << 40) * FDSClock;
			fdso.cycles            /= FSettings.SndRate * 16;
			fdso.cwave_pos_shift    = 21;
			fdso.mod_pos_shift      = 16;
			fdso.mod_overflow_shift = 11;
			fdso.env_count_mul      = 4;
		}
	}
	SetReadHandler(0x4040, 0x407F, FDSWaveRead);
	SetReadHandler(0x4090, 0x4090, FDSEnvVolumeRead);
	SetReadHandler(0x4092, 0x4092, FDSEnvModRead);

	SetWriteHandler(0x4040, 0x407F, FDSWaveWrite);
	SetWriteHandler(0x4080, 0x4080, FDSSReg0Write);
	SetWriteHandler(0x4082, 0x4082, FDSSReg1Write);
	SetWriteHandler(0x4083, 0x4083, FDSSReg2Write);
	SetWriteHandler(0x4084, 0x4084, FDSSReg3Write);
	SetWriteHandler(0x4085, 0x4085, FDSSReg4Write);
	SetWriteHandler(0x4086, 0x4086, FDSSReg5Write);
	SetWriteHandler(0x4087, 0x4087, FDSSReg6Write);
	SetWriteHandler(0x4088, 0x4088, FDSSReg7Write);
	SetWriteHandler(0x4089, 0x4089, FDSSReg8Write);
	SetWriteHandler(0x408A, 0x408A, FDSSReg9Write);
}

void FDSSoundReset(void) {
	memset(&fdso, 0, sizeof(fdso));

	/* BIOS-style register reset.  Matches what the FDS BIOS does on
	 * power-up; see https://github.com/bbbradsmith/nsfplay/.../nes_fds.cpp
	 * Apply these BEFORE FDS_ESI so the writes don't trigger spurious
	 * RenderSound calls against an unsized output buffer. */
	FDSSReg0Write(0x4080, 0x80); /* output volume = 0, env disabled */
	FDSSReg9Write(0x408A, 0xE8); /* master envelope speed */
	FDSSReg1Write(0x4082, 0x00); /* carrier freq 0 */
	FDSSReg2Write(0x4083, 0x80); /* carrier disabled */
	FDSSReg3Write(0x4084, 0x80); /* mod strength 0 */
	FDSSReg4Write(0x4085, 0x00); /* mod position 0 */
	FDSSReg5Write(0x4086, 0x00); /* mod freq 0 */
	FDSSReg6Write(0x4087, 0x80); /* mod disable */
	FDSSReg8Write(0x4089, 0x00); /* wave write disable */

	FDS_ESI();

	GameExpSound.HiSync  = HQSync;
	GameExpSound.HiFill  = RenderSoundHQ;
	GameExpSound.Fill    = FDSSound;
	GameExpSound.RChange = FDS_ESI;
}

/* Used by FDS-conversion mappers (e.g. some Whirlwind Manu bootlegs) that
 * surface the FDS audio registers through their own write paths instead of
 * the standard FDS BIOS layout.  Same signature as before the backport. */
void FDSSoundWrite(uint32_t A, uint8_t V) {
	switch (A) {
	case 0x4080: FDSSReg0Write(A, V); break;
	case 0x4082: FDSSReg1Write(A, V); break;
	case 0x4083: FDSSReg2Write(A, V); break;
	case 0x4084: FDSSReg3Write(A, V); break;
	case 0x4085: FDSSReg4Write(A, V); break;
	case 0x4086: FDSSReg5Write(A, V); break;
	case 0x4087: FDSSReg6Write(A, V); break;
	case 0x4088: FDSSReg7Write(A, V); break;
	case 0x4089: FDSSReg8Write(A, V); break;
	case 0x408A: FDSSReg9Write(A, V); break;
	default:
		if (A >= 0x4040 && A <= 0x407F) FDSWaveWrite(A, V);
		break;
	}
}

void FDSSoundPower(void) {
	FDSSoundReset();
	FDSSoundStateAdd();
}
