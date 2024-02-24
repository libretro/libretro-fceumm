
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

/* SUNSOFT-5B Sound */

#include "mapinc.h"
#include "s5bsound.h"

#if 0 /* remove comment to use old S5B emulation */
#define USE_OLD_SB5 
#endif

#ifndef USE_OLD_SB5
#define USE_EMU2149
#endif

#ifdef USE_EMU2149
#include "emu2149.h"

static int32 dwave = 0;
static PSG *psg_chip = NULL;

static void PSG_fillbuf(PSG *ssg, int32 *buf, int32 len, int shift) {
	while (len > 0) {
		*buf += (GetOutput(SND_S5B, PSG_calc(psg_chip) << shift));
		buf++;
		len--;
	}
}

static void UpdatePSGNEO(int32 *buf, int Count) {
	PSG_fillbuf(psg_chip, buf, Count, 4);
}

static void UpdatePSG(int Count) {
	int32 z, a;
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	if (a) {
		PSG_fillbuf(psg_chip, &Wave[dwave], a, 1);
    }
	dwave = 0;
}

DECLFW(S5BSound_Write) {
	if (!psg_chip) {
		return;
	}

	switch (A & 0xE000) {
	case 0xC000:
		PSG_writeIO(psg_chip, 0, V);
		break;
	case 0xE000:
		GameExpSound[SND_S5B - 6].Fill = UpdatePSG;
		GameExpSound[SND_S5B - 6].NeoFill = UpdatePSGNEO;
		PSG_writeIO(psg_chip, 1, V);
		break;
	}
}

static void S5BSound_SC(void) {
	if (psg_chip) {
		PSG_setRate(psg_chip, FSettings.SndRate ? FSettings.SndRate : 44100);
		PSG_setVolumeMode(psg_chip, 1);
		PSG_reset(psg_chip);
	}
}

static void S5BSound_KILL(void) {
	if (psg_chip) {
		PSG_delete(psg_chip);
		psg_chip = NULL;
	}
}

void S5BSound_ESI(void) {
	if (!(psg_chip = PSG_new(3579545 / 4, FSettings.SndRate ? FSettings.SndRate : 44100))) {
		return;
	}

	GameExpSound[SND_S5B - 6].RChange = S5BSound_SC;
	GameExpSound[SND_S5B - 6].Kill = S5BSound_KILL;

	S5BSound_SC();
}

void S5BSound_AddStateInfo(void) {
	if (!psg_chip) {
		return;
	}

	/* Sound states */
	AddExState(psg_chip->reg, sizeof(psg_chip->reg), 0, "REG");

	AddExState(psg_chip->count, sizeof(psg_chip->count), 0, "PCNT");
	AddExState(psg_chip->volume, sizeof(psg_chip->volume), 0, "PVOL");
	AddExState(psg_chip->edge, sizeof(psg_chip->edge), 0, "EDGE");
	AddExState(psg_chip->freq, sizeof(psg_chip->freq), 0, "FREQ");
	AddExState(psg_chip->tmask, sizeof(psg_chip->tmask), 0, "TMSK");
	AddExState(psg_chip->nmask, sizeof(psg_chip->nmask), 0, "NMSK");

	AddExState(&psg_chip->env_ptr, sizeof(psg_chip->env_ptr), 0, "PTR");
	AddExState(&psg_chip->env_face, sizeof(psg_chip->env_face), 0, "FACE");

	AddExState(&psg_chip->env_continue, sizeof(psg_chip->env_continue), 0, "CONT");
	AddExState(&psg_chip->env_attack, sizeof(psg_chip->env_attack), 0, "ATTK");
	AddExState(&psg_chip->env_alternate, sizeof(psg_chip->env_alternate), 0, "ALT");
	AddExState(&psg_chip->env_hold, sizeof(psg_chip->env_hold), 0, "HOLD");
	AddExState(&psg_chip->env_pause, sizeof(psg_chip->env_pause), 0, "PAUS");

	AddExState(&psg_chip->env_freq, sizeof(psg_chip->env_freq), 0, "EFRQ");
	AddExState(&psg_chip->env_count, sizeof(psg_chip->env_count), 0, "ECNT");

	AddExState(&psg_chip->noise_seed, sizeof(psg_chip->noise_seed), 0, "NSED");
	AddExState(&psg_chip->noise_scaler, sizeof(psg_chip->noise_scaler), 0, "NSCL");
	AddExState(&psg_chip->noise_count, sizeof(psg_chip->noise_count), 0, "NCNT");
	AddExState(&psg_chip->noise_freq, sizeof(psg_chip->noise_freq), 0, "NFRQ");
}

#else /* !USE_EMU2149 */
static void (*sfun[3])(void);
static uint8 sndcmd, sreg[14];

static int32 vcount[3];
static int32 dcount[3];
static int32 CAYBC[3];

static void AYSound(int Count);
static void AYSoundHQ(void);
static void AYHiSync(int ts);
static void DoAYSQ(int x);
static void DoAYSQHQ(int x);

static void DoAYSQ(int x) {
	int32 freq = ((sreg[x << 1] | ((sreg[(x << 1) + 1] & 15) << 8)) + 1) << (4 + 17);
	int32 amp = (sreg[0x8 + x] & 15) << 2;
	int32 start, end;
	int V;

	amp += amp >> 1;
	amp = GetOutput(SND_S5B, amp);

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
	amp = GetOutput(SND_S5B, amp);

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
	DoAYSQ1();
	DoAYSQ2();
	DoAYSQ3();
	CAYBC[0] = Count;
	CAYBC[1] = Count;
	CAYBC[2] = Count;
}

static void AYSoundHQ(void) {
	DoAYSQ1HQ();
	DoAYSQ2HQ();
	DoAYSQ3HQ();
}

static void AYHiSync(int32 ts) {
	CAYBC[0] = ts;
	CAYBC[1] = ts;
	CAYBC[2] = ts;
}

DECLFW(S5BSound_Write) {
	switch (A & 0xE000) {
	case 0xC000:
		sndcmd = V & 0x0F;
		break;
	case 0xE000:
		GameExpSound[SND_S5B - 6].Fill = AYSound;
		GameExpSound[SND_S5B - 6].HiFill = AYSoundHQ;
		GameExpSound[SND_S5B - 6].HiSync = AYHiSync;
		switch (sndcmd) {
		case 0:
		case 1:
		case 8:
			if (sfun[0]) {
				sfun[0]();
			}
			break;
		case 2:
		case 3:
		case 9:
			if (sfun[1]) {
				sfun[1]();
			}
			break;
		case 4:
		case 5:
		case 10:
			if (sfun[2]) {
				sfun[2]();
			}
			break;
		case 7:
			if (sfun[0]) {
				sfun[0]();
			}
			if (sfun[1]) {
				sfun[1]();
			}
			break;
		}
		sreg[sndcmd] = V;
		break;
	}
}

static void S5BSound_SC(void) {
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

void S5BSound_ESI(void) {
	GameExpSound[SND_S5B - 6].RChange = S5BSound_SC;
	S5BSound_SC();
}

void S5BSound_AddStateInfo(void) {
	AddExState(&sndcmd, 1, 0, "SCMD");
	AddExState(sreg, 14, 0, "SREG");
	AddExState(dcount, 12, 0, "DCNT");
	AddExState(vcount, 12, 0, "VCNT");
	AddExState(CAYBC, 12, 0, "BC00");
}
#endif