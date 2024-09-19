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

#define TIMER_SHIFT ((FSettings.soundq >= 1) ? 0 : 17)

typedef struct DRIPSOUND {
	uint8 buffer[256], readPos, writePos;
	uint8 bufferFull, bufferEmpty;
	uint8 volume;
	uint16 freq;
	int32 timer;
	int16 out;
} DRIPSOUND;

static DRIPSOUND channel[2];
static int32 cvbc = 0;

static void SetOutput(DRIPSOUND *ds, int32 output) {
	ds->out = output;
}

static void ChannelReset(DRIPSOUND *ds) {
	memset(ds->buffer, 0, 256);
	ds->readPos = 0;
	ds->writePos = 0;
	ds->bufferFull = FALSE;
	ds->bufferEmpty = TRUE;
}

static uint8 ChannelRead(DRIPSOUND *ds, uint16 A) {
	uint8 result = 0;

	if (ds->bufferFull) {
		result |= 0x80;
	}

	if (ds->bufferEmpty) {
		result |= 0x40;
	}

	return result;
}

static void ChannelWrite(DRIPSOUND *ds, uint16 A, uint8 V) {
	switch (A & 0x03) {
	case 0:
		ChannelReset(ds);
		SetOutput(ds, 0);
		ds->timer = ds->freq << TIMER_SHIFT;
		break;

	case 1:
		if (ds->readPos == ds->writePos) {
			ds->bufferEmpty = FALSE;
			SetOutput(ds, V * ds->volume);
			ds->timer = ds->freq << TIMER_SHIFT;
		}

		ds->buffer[ds->writePos++] = V;
		if (ds->readPos == ds->writePos) {
			ds->bufferFull = TRUE;
		}
		break;

	case 2:
		ds->freq = (ds->freq & 0xF00) | V;
		break;

	case 3:
		ds->freq = (ds->freq & 0xFF) | ((V & 0xF) << 8);
		ds->volume = (V & 0xF0) >> 4;
		if (!ds->bufferEmpty) {
			SetOutput(ds, ds->buffer[ds->readPos] * ds->volume);
		}
		break;
	}
}

static int32 GetSample(void) {
	int32 out = 0;
	int P;

	for (P = 0; P < 2; P++) {
		DRIPSOUND *ds = &channel[P];

		if (!ds->bufferEmpty) {
			ds->timer -= nesincsize;
			while (ds->timer <= 0) {
				ds->timer += ds->freq << TIMER_SHIFT;
				if (ds->readPos == ds->writePos) {
					ds->bufferFull = FALSE;
				}

				ds->readPos++;
				SetOutput(ds, ds->buffer[ds->readPos] * ds->volume);

				if (ds->readPos == ds->writePos) {
					ds->bufferEmpty = TRUE;
				}
			}
		}
		out += ds->out;
	}
	return out;
}

static void DRIPSound(void) {
	int P, V;
	int start = cvbc;
	int end = ((int)SOUNDTS << 16) / soundtsinc;

	if (end <= start) {
		return;
	}
	cvbc = end;

	for (V = start; V < end; V++) {
		Wave[V >> 4] += GetSample() >> 4;
	}
}

static int32 GetSampleHQ(void) {
	int32 out = 0;
	int P;

	for (P = 0; P < 2; P++) {
		DRIPSOUND *ds = &channel[P];

		if (!ds->bufferEmpty) {
			ds->timer--;
			if (ds->timer <= 0) {
				ds->timer = ds->freq << TIMER_SHIFT;
				if (ds->readPos == ds->writePos) {
					ds->bufferFull = FALSE;
				}

				ds->readPos++;
				SetOutput(ds, ds->buffer[ds->readPos] * ds->volume);

				if (ds->readPos == ds->writePos) {
					ds->bufferEmpty = TRUE;
				}
			}
		}
		out += ds->out;
	}
	return out;
}

static void DoDRIPSoundHQ(void) {
	int P, V;

	for (V = cvbc; V < (int)SOUNDTS; V++) {
		WaveHi[V] += GetSampleHQ();
	}
	cvbc = SOUNDTS;
}

static void DoDripSound(int Count) {
	DRIPSound();
	cvbc = Count;
}

static void SyncHQ(int32 ts) {
	cvbc = ts;
}

DECLFR(DRIPSound_Read) {
	if (A & 0x800) {
		return (ChannelRead(&channel[1], A));
	}
	return (ChannelRead(&channel[0], A));
}

DECLFW(DRIPSound_Write) {
	if (FSettings.SndRate > 0) {
		if (FSettings.soundq >= 1) {
			DoDRIPSoundHQ();
		} else {
			DRIPSound();
		}
	}
	if (A & 0x04) {
		ChannelWrite(&channel[1], A, V);
	} else {
		ChannelWrite(&channel[0], A, V);
	}
}

static void DRIPSound_SC(void) {
	GameExpSound[0].Fill = DoDripSound;
	GameExpSound[0].HiSync = SyncHQ;
	GameExpSound[0].HiFill = DoDRIPSoundHQ;
	GameExpSound[0].RChange = DRIPSound_SC;
	channel[0].timer = 0;
	channel[1].timer = 0;
	cvbc = 0;
}

void DRIPSound_ESI(void) {
	GameExpSound[0].RChange = DRIPSound_SC;

	ChannelReset(&channel[0]);
	ChannelReset(&channel[1]);

	DRIPSound_SC();
}

void DRIPSound_AddStateInfo(void) {
	AddExState(channel[0].buffer,     256, 0, "FF00");
	AddExState(&channel[0].readPos,     1, 0, "RDP0");
	AddExState(&channel[0].writePos,    1, 0, "WRP0");
	AddExState(&channel[0].bufferFull,  1, 0, "FUL0");
	AddExState(&channel[0].bufferEmpty, 1, 0, "EMT0");
	AddExState(&channel[0].freq,        2, 0, "FRQ0");
	AddExState(&channel[0].volume,      1, 0, "VOL0");
	AddExState(&channel[0].timer,       4, 0, "TIM0");
	AddExState(&channel[0].out,         2, 0, "POS0");

	AddExState(channel[1].buffer,     256, 0, "FF01");
	AddExState(&channel[1].readPos,     1, 0, "RDP1");
	AddExState(&channel[1].writePos,    1, 0, "WRP1");
	AddExState(&channel[1].bufferFull,  1, 0, "FUL1");
	AddExState(&channel[1].bufferEmpty, 1, 0, "EMT1");
	AddExState(&channel[1].freq,        2, 0, "FRQ1");
	AddExState(&channel[1].volume,      1, 0, "VOL1");
	AddExState(&channel[1].timer,       4, 0, "TIM1");
	AddExState(&channel[1].out,         2, 0, "POS1");

	AddExState(&cvbc,                   4, 0, "CVBC");
}
