
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

/* SUNSOFT-5/FME-7 Sound */

#include "mapinc.h"
#include "fme7sound.h"

static void(*sfun[3]) (void);

static void AYSound(int Count);
static void AYSoundHQ(void);
static void DoAYSQ(int x);
static void DoAYSQHQ(int x);

static uint8 sndcmd, sreg[14];
static int32 vcount[3];
static int32 dcount[3];
static int32 CAYBC[3];

static SFORMAT SStateRegs[] =
{
	{ &sndcmd, 1, "SCMD" },
	{ sreg, 14, "SREG" },
	{ dcount, 12, "DCNT" },
	{ vcount, 12, "VCNT" },
	{ CAYBC, 12, "BC00" },

	{ 0 }
};

DECLFW(FME7Sound_WriteIndex) {
	sndcmd = V % 14;
}

DECLFW(FME7Sound_WriteReg) {
	GameExpSound.Fill = AYSound;
	GameExpSound.HiFill = AYSoundHQ;
	switch (sndcmd) {
		case 0:
		case 1:
		case 8:
			if (sfun[0])
				sfun[0]();
			break;
		case 2:
		case 3:
		case 9:
			if (sfun[1])
				sfun[1]();
			break;
		case 4:
		case 5:
		case 10:
			if (sfun[2])
				sfun[2]();
			break;
		case 7:
			if (sfun[0])
				sfun[0]();
			if (sfun[1])
				sfun[1]();
			break;
	}
	sreg[sndcmd] = V;
}

static void DoAYSQ(int x) {
	int32 freq = ((sreg[x << 1] | ((sreg[(x << 1) + 1] & 15) << 8)) + 1) << (4 + 17);
	int32 amp = (sreg[0x8 + x] & 15) << 2;
	int32 start, end;
	int V;

	amp += amp >> 1;
	amp = GetVolume(APU_S5B, amp);

	start = CAYBC[x];
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start)
		return;
	CAYBC[x] = end;

	if (amp && !(sreg[0x7] & (1 << x))) {
		for (V = start; V < end; V++) {
			if (dcount[x])
				Wave[V >> 4] += amp;
			vcount[x] -= nesincsize;
			while (vcount[x] <= 0) {
				dcount[x] ^= 1;
				vcount[x] += freq;
			}
		}
	}
}

static void DoAYSQHQ(int x) {
	uint32 V;
	int32 freq = ((sreg[x << 1] | ((sreg[(x << 1) + 1] & 15) << 8)) + 1) << 4;
	int32 amp = (sreg[0x8 + x] & 15) << 6;

	amp += amp >> 1;
	amp = GetVolume(APU_S5B, amp);

	if (!(sreg[0x7] & (1 << x))) {
		for (V = CAYBC[x]; V < SOUNDTS; V++) {
			if (dcount[x])
				WaveHi[V] += amp;
			vcount[x]--;
			if (vcount[x] <= 0) {
				dcount[x] ^= 1;
				vcount[x] = freq;
			}
		}
	}
	CAYBC[x] = SOUNDTS;
}

static void DoAYSQ1(void) {
	DoAYSQ(0);
}

static void DoAYSQ2(void) {
	DoAYSQ(1);
}

static void DoAYSQ3(void) {
	DoAYSQ(2);
}

static void DoAYSQ1HQ(void) {
	DoAYSQHQ(0);
}

static void DoAYSQ2HQ(void) {
	DoAYSQHQ(1);
}

static void DoAYSQ3HQ(void) {
	DoAYSQHQ(2);
}

static void AYSound(int Count) {
	int x;
	DoAYSQ1();
	DoAYSQ2();
	DoAYSQ3();
	for (x = 0; x < 3; x++)
		CAYBC[x] = Count;
}

static void AYSoundHQ(void) {
	DoAYSQ1HQ();
	DoAYSQ2HQ();
	DoAYSQ3HQ();
}

static void AYHiSync(int32 ts) {
	int x;

	for (x = 0; x < 3; x++)
		CAYBC[x] = ts;
}

static void FME7Sound_SC(void) {
    memset(dcount, 0, sizeof(dcount));
	memset(vcount, 0, sizeof(vcount));
	memset(CAYBC, 0, sizeof(CAYBC));
	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			sfun[0] = DoAYSQ1HQ;
			sfun[1] = DoAYSQ2HQ;
			sfun[2] = DoAYSQ3HQ;
		} else {
			sfun[0] = DoAYSQ1;
			sfun[1] = DoAYSQ2;
			sfun[2] = DoAYSQ3;
		}
	} else {
		memset(sfun, 0, sizeof(sfun));
    }
}

void FME7Sound_ESI(void) {
    sndcmd = 0;
	GameExpSound.RChange = FME7Sound_SC;
	GameExpSound.HiSync = AYHiSync;
	FME7Sound_SC();
    AddExState(SStateRegs, ~0, 0, NULL);
}
