/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

/* FDSSound rewrite based on MDFN NES fds sound implementation, modifed for fceumm's use. */
/* WIP as some crackling sounds can be annoyingly heard and other issues. */

#include "mapinc.h"
#include "fdssound.h"

static void RenderSoundHQ(void);
static void RenderSound(void);

static const int32 mod_bias_tab[8] = { 0, 1, 2, 4, 0, -4, -2, -1 };

static FDSSOUND fdso = { 0 };
static int32 FBC = 0;

void FDSSound_AddStateInfo(void) {
	AddExState(&fdso.EnvUnits[EVOL].speed,    1, 0, "SPD0");
	AddExState(&fdso.EnvUnits[EVOL].control,  1, 0, "CTL0");
	AddExState(&fdso.EnvUnits[EVOL].volume,   1, 0, "VOL0");
	AddExState(&fdso.EnvUnits[EVOL].counter,  1, 0, "CNT0");

	AddExState(&fdso.EnvUnits[EMOD].speed,    1, 0, "SPD1");
	AddExState(&fdso.EnvUnits[EMOD].control,  1, 0, "CTL1");
	AddExState(&fdso.EnvUnits[EMOD].volume,   1, 0, "VOL1");
	AddExState(&fdso.EnvUnits[EMOD].counter,  1, 0, "CNT1");

	AddExState(fdso.cwave,                   64, 0, "WAVE");
	AddExState(fdso.mwave,                   32, 0, "MWAV");

	AddExState(&fdso.cwave_freq,              2, 0, "WFRQ");
	AddExState(&fdso.cwave_pos,               4, 0, "WPOS");
	AddExState(&fdso.cwave_control,           1, 0, "WCTL");

	AddExState(&fdso.mod_freq,                2, 0, "MFRQ");
	AddExState(&fdso.mod_pos,                 4, 0, "MPOS");
	AddExState(&fdso.mod_control,             1, 0, "MCTL");

	AddExState(&fdso.sweep_bias,              4, 0, "SWBS");

	AddExState(&fdso.master_control,          1, 0, "MCTL");
	AddExState(&fdso.master_env_speed,        1, 0, "MSPD");

	AddExState(&fdso.envcount,                2, 0, "EDIV");
}

static void FDSSoundUpdate(void) {
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			RenderSoundHQ();
		} else {
			RenderSound();
		}
	}
}

DECLFR(FDSWaveRead) {
	if (fdso.master_control & WAVE_WRITE_MODE) {
		return (OPENBUS | fdso.cwave[A & 0x3F]);
	}
	return (OPENBUS | fdso.cwave[(fdso.cwave_pos >> 21) & 0x3F]);
}

DECLFW(FDSWaveWrite) {
	if (fdso.master_control & WAVE_WRITE_MODE) {
		fdso.cwave[A & 0x3f] = V & 0x3F;
	}
}

DECLFW(FDSSReg0Write) {
	FDSSoundUpdate();

	fdso.EnvUnits[EVOL].speed = V & 0x3F;
	fdso.EnvUnits[EVOL].control = V & 0xC0;

	fdso.EnvUnits[EVOL].counter = fdso.EnvUnits[EVOL].speed + 1;

	if (fdso.EnvUnits[EVOL].control & ENV_CTRL_DISABLE) {
		fdso.EnvUnits[EVOL].volume = fdso.EnvUnits[EVOL].speed;
	}
}

DECLFW(FDSSReg1Write) {
	FDSSoundUpdate();

	fdso.cwave_freq &= 0x0F00;
	fdso.cwave_freq |= V;
}

DECLFW(FDSSReg2Write) {
	FDSSoundUpdate();

	fdso.cwave_freq &= 0x00FF;
	fdso.cwave_freq |= (V & 0x0F) << 8;

	fdso.cwave_control = V & 0xC0;

	if ((fdso.cwave_control & WAVE_DISABLE)) {
		fdso.cwave_pos = 0;
	}
}

DECLFW(FDSSReg3Write) {
	FDSSoundUpdate();

	fdso.EnvUnits[EMOD].speed = V & 0x3F;
	fdso.EnvUnits[EMOD].control = V & 0xC0;

	fdso.EnvUnits[EMOD].counter = fdso.EnvUnits[EMOD].speed + 1;

	if (fdso.EnvUnits[EMOD].control & ENV_CTRL_DISABLE) {
		fdso.EnvUnits[EMOD].volume = fdso.EnvUnits[EMOD].speed;
	}
}

DECLFW(FDSSReg4Write) {
	FDSSoundUpdate();

	fdso.sweep_bias = (V & 0x7F) << 4;
	fdso.mod_pos = 0;
}

DECLFW(FDSSReg5Write) {
	FDSSoundUpdate();

	fdso.mod_freq &= 0x0F00;
	fdso.mod_freq |= V;
}

DECLFW(FDSSReg6Write) {
	FDSSoundUpdate();

	fdso.mod_freq &= 0x00FF;
	fdso.mod_freq |= (V & 0x0F) << 8;

	fdso.mod_control = V & 0xC0;

	if (fdso.mod_control & MOD_WRITE_MODE) {
		fdso.mod_pos = 0;
	}
}

DECLFW(FDSSReg7Write) {
	FDSSoundUpdate();

	if (fdso.mod_control & MOD_WRITE_MODE) {
		int i;
		for (i = 0; i < 31; i++) {
			fdso.mwave[i] = fdso.mwave[i + 1];
		}
		fdso.mwave[0x1F] = mod_bias_tab[V & 0x07];
		if ((V & 0x07) == 0x04) {
			fdso.mwave[0x1F] = 0x10;
		}
	}
}

DECLFW(FDSSReg8Write) {
	FDSSoundUpdate();

	fdso.master_control = V;
}

DECLFW(FDSSReg9Write) {
	FDSSoundUpdate();

	fdso.master_env_speed = V;
}

DECLFR(FDSEnvVolumeRead) {
	FDSSoundUpdate();
	return (OPENBUS | fdso.EnvUnits[EVOL].volume);
}

DECLFR(FDSEnvModRead) {
	FDSSoundUpdate();
	return (OPENBUS | fdso.EnvUnits[EMOD].volume);
}

#define sign_x_to_s32(n, v) ((int32)((uint32)(v) << (32 - (n))) >> (32 - (n)))

static INLINE int32 FDSDoSound(void) {
	uint32 prev_cwave_pos = fdso.cwave_pos;
	static int32 temp;

	fdso.count += fdso.cycles;
	if (fdso.count >= ((int64)1 << 40)) {
dogk:
		fdso.count -= (int64)1 << 40;

		if (!(fdso.cwave_control & ENVELOPES_DISABLE) && fdso.master_env_speed) {
			if (fdso.envcount) {
				fdso.envcount--;
			} else {
				fdso.envcount = fdso.master_env_speed * 3;
				if (!(fdso.EnvUnits[EVOL].control & ENV_CTRL_DISABLE)) {
					if (fdso.EnvUnits[EVOL].counter) {
						fdso.EnvUnits[EVOL].counter--;
					} else {
						fdso.EnvUnits[EVOL].counter = fdso.EnvUnits[EVOL].speed + 1;
						if ((fdso.EnvUnits[EVOL].control & ENV_CTRL_INCREASE)) {
							if (fdso.EnvUnits[EVOL].volume < 0x20) {
								fdso.EnvUnits[EVOL].volume++;
							}
						} else {
							if (fdso.EnvUnits[EVOL].volume > 0) {
								fdso.EnvUnits[EVOL].volume--;
							}
						}
					}
				}

				if (!(fdso.EnvUnits[EMOD].control & ENV_CTRL_DISABLE)) {
					if (fdso.EnvUnits[EMOD].counter) {
						fdso.EnvUnits[EMOD].counter--;
					} else {
						fdso.EnvUnits[EMOD].counter = fdso.EnvUnits[EMOD].speed + 1;
						if ((fdso.EnvUnits[EMOD].control & ENV_CTRL_INCREASE)) {
							if (fdso.EnvUnits[EMOD].volume < 0x20) {
								fdso.EnvUnits[EMOD].volume++;
							}
						} else {
							if (fdso.EnvUnits[EMOD].volume > 0) {
								fdso.EnvUnits[EMOD].volume--;
							}
						}
					}
				}
			}
		}

		if (!(fdso.mod_control & MOD_WRITE_MODE)) {
			uint32 prev_mod_pos = fdso.mod_pos;

			fdso.mod_pos += fdso.mod_freq;
			if ((fdso.mod_pos & (0x3F << 11)) !=
			    (prev_mod_pos & (0x3F << 11))) {
				const int32 mw = fdso.mwave[((fdso.mod_pos >> 16) & 0x1F)];

				fdso.sweep_bias = (fdso.sweep_bias + mw) & 0x7FF;
				if (mw == 0x10) {
					fdso.sweep_bias = 0;
				}
			}

			temp = sign_x_to_s32(11, fdso.sweep_bias) *
			       ((fdso.EnvUnits[EMOD].volume > 0x20) ? 0x20 : fdso.EnvUnits[EMOD].volume);

			if (temp & 0x0F0) {
				temp /= 256;
				if (fdso.sweep_bias & 0x400) {
					temp--;
				} else {
					temp += 2;
				}
			} else {
				temp /= 256;
			}

			if (temp >= 192) {
				temp -= 256;
			}
			if (temp < -64) {
				temp += 256;
			}
		}

		if (!(fdso.cwave_control & WAVE_DISABLE)) {
			int32 cur_cwave_freq = (int32)(fdso.cwave_freq << 6);

			if (!(fdso.mod_control & MOD_WRITE_MODE)) {
				cur_cwave_freq += (int32)fdso.cwave_freq * temp;
				if (cur_cwave_freq < 0) {
					cur_cwave_freq = 0;
				}
			}
			fdso.cwave_pos = (fdso.cwave_pos + cur_cwave_freq) & 0x7FFFFFF;
		}
	}
	if (fdso.count >= 32768) {
		goto dogk;
	}

	/* Might need to emulate applying the amplitude to the waveform a bit better... */
	{
		int k = fdso.EnvUnits[EVOL].volume;
		if (k > 0x20) {
			k = 0x20;
		}
		return (fdso.cwave[fdso.cwave_pos >> 21] * FSettings.volume[SND_FDS] * k * 4 / ((fdso.master_control & MASTER_VOLUME) + 2)) >> 8;
	}
}

static void RenderSound(void) {
	int32 end, start;
	int32 x;

	start = FBC;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	FBC = end;

	if (!(fdso.master_control & WAVE_WRITE_MODE)) {
		for (x = start; x < end; x++) {
			uint32 t = FDSDoSound();
			t += t >> 1;
			t >>= 4;
			Wave[x >> 4] += t;	/* (t>>2)-(t>>3); */ /* >>3; */
		}
	}
}

static void RenderSoundHQ(void) {
	uint32 x;

	if (!(fdso.master_control & WAVE_WRITE_MODE)) {
		for (x = FBC; x < SOUNDTS; x++) {
			uint32 t = FDSDoSound();
			t += t >> 1;
			WaveHi[x] += t;	/* (t<<2)-(t<<1); */
		}
	}
	FBC = SOUNDTS;
}

static void HQSync(int32 ts) {
	FBC = ts;
}

static void FDSSound(int c) {
	RenderSound();
	FBC = c;
}

void FDSSound_SC(void) {
	GameExpSound[SND_FDS - 6].HiSync = HQSync;
	GameExpSound[SND_FDS - 6].HiFill = RenderSoundHQ;
	GameExpSound[SND_FDS - 6].Fill = FDSSound;
	GameExpSound[SND_FDS - 6].RChange = FDSSound_SC;

	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			fdso.cycles = (int64)1 << 39;
		} else {
			fdso.cycles = ((int64)1 << 40) * FDSClock;
			fdso.cycles /= FSettings.SndRate * 16;
		}
	}
}

void FDSSoundRegReset(void) {
	memset(&fdso, 0, sizeof(fdso));

	/* https://github.com/bbbradsmith/nsfplay/blob/f563b9fe32df9ef0a94944026642d8ce5144c000/xgm/devices/Sound/nes_fds.cpp#L109 */

	/* NOTE: the FDS BIOS reset only does the following related to audio: */
	/*   $4023 = $00
	 *   $4023 = $83 enables master_io
	 *   $4080 = $80 output volume = 0, envelope disabled
	 *   $408A = $E8 master envelope speed
	 *   $4023 = $00
	 *   $4023 = $83 */
	FDSSReg0Write(0x4080, 0x80);
	FDSSReg9Write(0x408A, 0xE8);

	/* reset other stuff */
	FDSSReg1Write(0x4082, 0x00); /* wav freq 0 */
	FDSSReg2Write(0x4083, 0x80); /* wav disable */
	FDSSReg3Write(0x4084, 0x80); /* mod strength 0 */
	FDSSReg4Write(0x4085, 0x00); /* mod position 0 */
	FDSSReg5Write(0x4086, 0x00); /* mod freq 0 */
	FDSSReg6Write(0x4087, 0x80); /* mod disable */
	FDSSReg8Write(0x4089, 0x00); /* wav write disable, max global volume} */
}

/* callback for bootleg mappers */

void FDSSound_Power(void) {
	FDSSoundRegReset();
	FDSSound_SC();	

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
