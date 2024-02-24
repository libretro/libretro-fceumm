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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "mapinc.h"
#include "mmc5sound.h"

typedef struct __MMC5SQUARE {
	/* regs */
	uint8 volume;
	uint8 duty;
	uint8 enabled;
	int16 freq;

	/* timers */
	uint8 dcount;
	int32 vcount;
	int32 cvbc;
} MMC5SQUARE;

typedef struct __MMC5PCM {
	uint8 rawdata;
	uint8 control;
	int32 cvbc;
} MMC5PCM;

typedef struct __MMC5SOUND {
	MMC5SQUARE square[2];
	MMC5PCM pcm;
} MMC5SOUND;

static MMC5SOUND MMC5Sound;

static void (*sfun)(MMC5SQUARE *channel);
static void (*psfun)(void);

static void Do5PCM(void) {
	int32 V;
	int32 start, end;

	start = MMC5Sound.pcm.cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	MMC5Sound.pcm.cvbc = end;

	if (!(MMC5Sound.pcm.control & 0x40) && MMC5Sound.pcm.rawdata) {
		int32 amp = GetOutput(SND_MMC5, MMC5Sound.pcm.rawdata << 1);

		for (V = start; V < end; V++) {
			Wave[V >> 4] += amp;
		}
	}
}

static void Do5PCMHQ(void) {
	int32 V;

	if (!(MMC5Sound.pcm.control & 0x40) && MMC5Sound.pcm.rawdata) {
		int32 amp = GetOutput(SND_MMC5, MMC5Sound.pcm.rawdata << 5);

		for (V = MMC5Sound.pcm.cvbc; V < SOUNDTS; V++) {
			WaveHi[V] += amp;
		}
	}
	MMC5Sound.pcm.cvbc = SOUNDTS;
}

static void Do5SQ(MMC5SQUARE *channel) {
	static int tal[4] = { 1, 2, 4, 6 };
	int32 V, amp, duty, freq;
	int32 start, end;

	start = channel->cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	channel->cvbc = end;

	freq = channel->freq + 1;
	amp = GetOutput(SND_MMC5, channel->volume << 4);
	duty = tal[channel->duty];

	if ((freq >= 8) && channel->enabled) {
		int32 dc, vc;

		freq <<= 18;
		dc = channel->dcount;
		vc = channel->vcount;

		for (V = start; V < end; V++) {
			if (dc < duty) {
				Wave[V >> 4] += amp;
			}
			vc -= nesincsize;
			while (vc <= 0) {
				vc += freq;
				dc = (dc + 1) & 0x07;
			}
		}
		channel->dcount = dc;
		channel->vcount = vc;
	}
}

static void Do5SQHQ(MMC5SQUARE *channel) {
	static int tal[4] = { 1, 2, 4, 6 };
	int V, amp, duty, freq;

	freq = channel->freq + 1;
	amp = GetOutput(SND_MMC5, channel->volume << 8);
	duty = tal[channel->duty];

	if ((freq >= 8) && channel->enabled) {
		int dc, vc;

		freq <<= 1;

		dc = channel->dcount;
		vc = channel->vcount;

		for (V = channel->cvbc; V < SOUNDTS; V++) {
			if (dc < duty) {
				WaveHi[V] += amp;
			}
			vc--;
			if (vc <= 0) { /* Less than zero when first started. */
				vc = freq;
				dc = (dc + 1) & 0x07;
			}
		}
		channel->dcount = dc;
		channel->vcount = vc;
	}
	channel->cvbc = SOUNDTS;
}

static void MMC5RunSoundHQ(void) {
	Do5SQHQ(&MMC5Sound.square[0]);
	Do5SQHQ(&MMC5Sound.square[1]);
	Do5PCMHQ();
}

static void MMC5HiSync(int32 ts) {
	MMC5Sound.square[0].cvbc = ts;
	MMC5Sound.square[1].cvbc = ts;
	MMC5Sound.pcm.cvbc       = ts;
}

static void MMC5RunSound(int Count) {
	Do5SQ(&MMC5Sound.square[0]);
	Do5SQ(&MMC5Sound.square[1]);
	Do5PCM();
	MMC5Sound.square[0].cvbc = Count;
	MMC5Sound.square[1].cvbc = Count;
	MMC5Sound.pcm.cvbc       = Count;
}

static void MMC5Square_Write(MMC5SQUARE *channel, uint8 reg, uint8 V) {
	switch (reg) {
	case 0:
		channel->volume = V & 0x0F;
		channel->duty = (V & 0xC0) >> 6;
		break;
	
	case 1:
		break;

	case 2:
		channel->freq = (channel->freq & ~0x00FF) | V;
		break;

	case 3:
		channel->freq = (channel->freq & ~0x0700) | ((V & 0x07) << 8);
		break;

	case 4:
		channel->enabled = (V != 0);
		break;
	}
}

DECLFW(MMC5Sound_Write) {
	GameExpSound[SND_MMC5 - 6].Fill   = MMC5RunSound;
	GameExpSound[SND_MMC5 - 6].HiFill = MMC5RunSoundHQ;

	switch (A) {
	case 0x5010:
		if (psfun) {
			psfun();
		}
		MMC5Sound.pcm.control = V;
		break;

	case 0x5011:
		if (psfun) {
			psfun();
		}
		MMC5Sound.pcm.rawdata = V;
		break;
	
	case 0x5000:
	case 0x5001:
	case 0x5002:
	case 0x5003:
		if (sfun) {
			sfun(&MMC5Sound.square[0]);
		}
		MMC5Square_Write(&MMC5Sound.square[0], A & 0x03, V);
		break;
	
	case 0x5004:
	case 0x5005:
	case 0x5006:
	case 0x5007:
		if (sfun) {
			sfun(&MMC5Sound.square[1]);
		}
		MMC5Square_Write(&MMC5Sound.square[1], A & 0x03, V);
		break;

	case 0x5015:
		if (sfun) {
			sfun(&MMC5Sound.square[0]);
			sfun(&MMC5Sound.square[1]);
		}
		MMC5Square_Write(&MMC5Sound.square[0], 4, V & 0x01);
		MMC5Square_Write(&MMC5Sound.square[1], 4, V & 0x02);
		break;
	}
}

static void MMC5SC(void) {
	GameExpSound[SND_MMC5 - 6].HiSync = MMC5HiSync;

	MMC5Sound.square[0].vcount = 0;
	MMC5Sound.square[1].vcount = 0;

	MMC5Sound.square[0].cvbc = 0;
	MMC5Sound.square[1].cvbc = 0;
	MMC5Sound.pcm.cvbc = 0;

	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			sfun = Do5SQHQ;
			psfun = Do5PCMHQ;
		} else {
			sfun = Do5SQ;
			psfun = Do5PCM;
		}
	} else {
		sfun = 0;
		psfun = 0;
	}
}

void MMC5Sound_ESI(void) {
	memset(&MMC5Sound, 0, sizeof(MMC5Sound));
	GameExpSound[SND_MMC5 - 6].RChange = MMC5SC;
	MMC5SC();
}

void MMC5Sound_AddStateInfo(void) {
	AddExState(&MMC5Sound.square[0].enabled, 1, 0, "S0EN");
	AddExState(&MMC5Sound.square[0].volume,  1, 0, "S0VL");
	AddExState(&MMC5Sound.square[0].freq,    2, 0, "S0FQ");
	AddExState(&MMC5Sound.square[0].duty,    1, 0, "S0DT");
	AddExState(&MMC5Sound.square[0].dcount,  1, 0, "S0DC");
	AddExState(&MMC5Sound.square[0].vcount,  4, 0, "S0VC");
	AddExState(&MMC5Sound.square[0].cvbc,    4, 0, "S0BC");

	AddExState(&MMC5Sound.square[1].enabled, 1, 0, "S1EN");
	AddExState(&MMC5Sound.square[1].volume,  1, 0, "S1VL");
	AddExState(&MMC5Sound.square[1].freq,    2, 0, "S1FQ");
	AddExState(&MMC5Sound.square[1].duty,    1, 0, "S1DT");
	AddExState(&MMC5Sound.square[1].dcount,  1, 0, "S1DC");
	AddExState(&MMC5Sound.square[1].vcount,  4, 0, "S1VC");
	AddExState(&MMC5Sound.square[1].cvbc,    4, 0, "S1BC");

	AddExState(&MMC5Sound.pcm.control,       1, 0, "PCTL");
	AddExState(&MMC5Sound.pcm.rawdata,       1, 0, "PRAW");
	AddExState(&MMC5Sound.pcm.cvbc,          4, 0, "PCVB");
}