/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 *
 * VRC-6 Sound
 *
 */

#include "mapinc.h"
#include "vrc6sound.h"

enum { SQUARE1, SQUARE2, SAW };

typedef struct __VRC6SQUARE {
	/* regs */
	uint8 volume;
	uint8 duty;
	uint8 mode;
	uint8 enabled;
	uint16 freq;

	/* timers */
	uint8 dcount;
	int32 vcount;
	int32 cvbc;
} VRC6SQUARE;

typedef struct __VRC6SAW {
	/* regs */
	uint8 accumrate;
	uint8 enabled;
	uint16 freq;

	uint8 phaseacc;

	/* timers */
	uint8 dcount;
	int32 vcount;
	int32 cvbc;
} VRC6SAW;

typedef struct __VRC6SOUND {
	VRC6SQUARE square[2];
	VRC6SAW saw;
	uint8 haltsound;
	uint8 freqshift;
} VR6SOUND;

static VR6SOUND VRC6Sound;

static void (*sfun[3])(void);

static INLINE void DoSQV(VRC6SQUARE *channel) {
	int32 start, end, V;

	start = channel->cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	channel->cvbc = end;

	if (!VRC6Sound.haltsound && channel->enabled) {
		int32 amp = ((channel->volume << 8) * 6 / 8) >> 4;
		int32 out = GetOutput(SND_VRC6, amp);

		for (V = start; V < end; V++) {
			channel->vcount -= nesincsize;
			while (channel->vcount <= 0) {
				channel->dcount = (channel->dcount + 1) & 0x0F;
				channel->vcount += ((channel->freq >> VRC6Sound.freqshift) + 1) << 17;
			}
			if (channel->mode || (channel->dcount <= channel->duty)) {
				Wave[V >> 4] += out;
			}
		}
	}
}

static void DoSQV1(void) {
	DoSQV(&VRC6Sound.square[0]);
}

static void DoSQV2(void) {
	DoSQV(&VRC6Sound.square[1]);
}

static void DoSawV(void) {
	int32 start, end, V;

	start = VRC6Sound.saw.cvbc;
	end = (SOUNDTS << 16) / soundtsinc;
	if (end <= start) {
		return;
	}
	VRC6Sound.saw.cvbc = end;

	if (VRC6Sound.saw.enabled) {
		int32 out;

		for (V = start; V < end; V++) {
			VRC6Sound.saw.vcount -= nesincsize;
			while (VRC6Sound.saw.vcount <= 0) {
				VRC6Sound.saw.dcount++;
				VRC6Sound.saw.dcount %= 14;
				if (VRC6Sound.saw.dcount == 0) {
					VRC6Sound.saw.phaseacc = 0;
				} else if (!(VRC6Sound.saw.dcount & 0x01)) {
					VRC6Sound.saw.phaseacc += VRC6Sound.saw.accumrate;
				}
				VRC6Sound.saw.vcount += ((VRC6Sound.saw.freq >> VRC6Sound.freqshift) + 1) << 17;
			}
			out = (((VRC6Sound.saw.phaseacc >> 3) & 0x1F) << 3) * 6 / 8;
			Wave[V >> 4] += GetOutput(SND_VRC6, out);
		}
	}
}

static INLINE void DoSQVHQ(VRC6SQUARE *channel) {
	int32 V;

	if (channel->enabled) {
		int32 amp = (channel->volume << 8) * 6 / 8;
		int32 out = GetOutput(SND_VRC6, amp);

		for (V = channel->cvbc; V < (int32)SOUNDTS; V++) {
			channel->vcount--;
			if (channel->vcount <= 0) {
				channel->dcount++;
				channel->dcount &= 0x0F;
				channel->vcount = (channel->freq >> VRC6Sound.freqshift) + 1;
			}
			if ((channel->mode || (channel->dcount > channel->duty))) {
				WaveHi[V] += out;
			}
		}
	}
	channel->cvbc = SOUNDTS;
}

static void DoSQV1HQ(void) {
	DoSQVHQ(&VRC6Sound.square[0]);
}

static void DoSQV2HQ(void) {
	DoSQVHQ(&VRC6Sound.square[1]);
}

static void DoSawVHQ(void) {
	int32 V;

	if (VRC6Sound.saw.enabled) {
		int32 out = 0;

		for (V = VRC6Sound.saw.cvbc; V < (int32)SOUNDTS; V++) {
			VRC6Sound.saw.vcount--;
			while (VRC6Sound.saw.vcount <= 0) {
				VRC6Sound.saw.dcount++;
				VRC6Sound.saw.dcount %= 14;
				if (VRC6Sound.saw.dcount == 0) {
					VRC6Sound.saw.phaseacc = 0;
				} else if (!(VRC6Sound.saw.dcount & 0x01)) {
					VRC6Sound.saw.phaseacc += VRC6Sound.saw.accumrate;
				}
				VRC6Sound.saw.vcount += ((VRC6Sound.saw.freq >> VRC6Sound.freqshift) + 1) << 0;
			}
			out = (((VRC6Sound.saw.phaseacc >> 3) & 0x1F) << 7) * 6 / 8;
			WaveHi[V] += GetOutput(SND_VRC6, out);
		}
	}
	VRC6Sound.saw.cvbc = SOUNDTS;
}

static void VRC6RunSound(int32 Count) {
	DoSQV1();
	DoSQV2();
	DoSawV();
	VRC6Sound.square[0].cvbc = Count;
	VRC6Sound.square[1].cvbc = Count;
	VRC6Sound.saw.cvbc = Count;

}

static void VRC6RunSoundHQ(void) {
	DoSQV1HQ();
	DoSQV2HQ();
	DoSawVHQ();
}

static void VRC6SyncHQ(int32 ts) {
	VRC6Sound.square[0].cvbc = ts;
	VRC6Sound.square[1].cvbc = ts;
	VRC6Sound.saw.cvbc = ts;
}

static void VRC6Square_Write(VRC6SQUARE *channel, uint8 reg, uint8 V) {
	switch (reg & 0x03) {
	case 0:
		channel->volume = V & 0x0F;
		channel->duty = (V & 0x70) >> 4;
		channel->mode = (V & 0x80) == 0x80;
		break;

	case 1:
		channel->freq = (channel->freq & 0xF00) | V;
		break;

	case 2:
		channel->freq = (channel->freq & 0xFF) | ((V & 0x0F) << 8);
		channel->enabled = (V & 0x80) == 0x80;
		if (!channel->enabled) {
			channel->dcount = 0;
		}
		break;
	}
}

DECLFW(VRC6Sound_Write) {
	switch (A & 0xF003) {
	case 0x9000:
	case 0x9001:
	case 0x9002:
		VRC6Square_Write(&VRC6Sound.square[0], A & 0x03, V);
		if (sfun[SQUARE1]) {
			sfun[SQUARE1]();
		}
		break;

	case 0x9003:
		VRC6Sound.haltsound = (V & 0x01) == 0x01;
		if (V & 0x04) {
			VRC6Sound.freqshift = 8;
		} else if (V & 0x02) {
			VRC6Sound.freqshift = 4;
		} else {
			VRC6Sound.freqshift = 0;
		}
		break;

	case 0xA000:
	case 0xA001:
	case 0xA002:
		VRC6Square_Write(&VRC6Sound.square[1], A & 0x03, V);
		if (sfun[SQUARE2]) {
			sfun[SQUARE2]();
		}
		break;
	
	case 0xB000:
	case 0xB001:
	case 0xB002:
		switch (A & 0x03) {
		case 0:
			VRC6Sound.saw.accumrate = V & 0x3F;
			break;

		case 1:
			VRC6Sound.saw.freq = (VRC6Sound.saw.freq & 0xF00) | V;
			break;

		case 2:
			VRC6Sound.saw.freq = (VRC6Sound.saw.freq & 0xFF) | ((V & 0x0F) << 8);
			VRC6Sound.saw.enabled = (V & 0x80) == 0x80;
			if (!VRC6Sound.saw.enabled) {
				VRC6Sound.saw.dcount = 0;
			}
			break;
		}
		if (sfun[SAW]) {
			sfun[SAW]();
		}
		break;
	}
}

static void VRC6Sound_SC(void) {
	GameExpSound[SND_VRC6 - 6].Fill   = VRC6RunSound;
	GameExpSound[SND_VRC6 - 6].HiFill = VRC6RunSoundHQ;
	GameExpSound[SND_VRC6 - 6].HiSync = VRC6SyncHQ;

	VRC6Sound.square[0].vcount = 1;
	VRC6Sound.square[1].vcount = 1;
	VRC6Sound.saw.vcount       = 1;

	if (FSettings.SndRate) {
		if (FSettings.soundq >= 1) {
			sfun[SQUARE1] = DoSQV1HQ;
			sfun[SQUARE2] = DoSQV2HQ;
			sfun[SAW]     = DoSawVHQ;
		} else {
			sfun[SQUARE1] = DoSQV1;
			sfun[SQUARE2] = DoSQV2;
			sfun[SAW]     = DoSawV;
		}
	} else {
		memset(sfun, 0, sizeof(sfun));
	}
}

void VRC6Sound_ESI(void) {
	memset(&VRC6Sound, 0, sizeof(VRC6Sound));
	GameExpSound[SND_VRC6 - 6].RChange = VRC6Sound_SC;
	VRC6Sound_SC();
}

void VRC6Sound_AddStateInfo(void) {
	AddExState(&VRC6Sound.haltsound,         1, 0, "HLTS");
	AddExState(&VRC6Sound.freqshift,         1, 0, "FRSH");

	AddExState(&VRC6Sound.square[0].enabled, 1, 0, "S0EN");
	AddExState(&VRC6Sound.square[0].volume,  1, 0, "S0VL");
	AddExState(&VRC6Sound.square[0].freq,    2, 0, "S0FQ");
	AddExState(&VRC6Sound.square[0].duty,    1, 0, "S0DT");
	AddExState(&VRC6Sound.square[0].mode,    1, 0, "S0MD");
	AddExState(&VRC6Sound.square[0].dcount,  1, 0, "S0DC");
	AddExState(&VRC6Sound.square[0].vcount,  4, 0, "S0VC");
	AddExState(&VRC6Sound.square[0].cvbc,    4, 0, "S0BC");

	AddExState(&VRC6Sound.square[1].enabled, 1, 0, "S1EN");
	AddExState(&VRC6Sound.square[1].volume,  1, 0, "S1VL");
	AddExState(&VRC6Sound.square[1].freq,    2, 0, "S1FQ");
	AddExState(&VRC6Sound.square[1].duty,    1, 0, "S1DT");
	AddExState(&VRC6Sound.square[1].mode,    1, 0, "S1MD");
	AddExState(&VRC6Sound.square[1].dcount,  1, 0, "S1DC");
	AddExState(&VRC6Sound.square[1].vcount,  4, 0, "S1VC");
	AddExState(&VRC6Sound.square[1].cvbc,    4, 0, "S1BC");

	AddExState(&VRC6Sound.saw.enabled,       1, 0, "SWEN");
	AddExState(&VRC6Sound.saw.accumrate,     1, 0, "SWAR");
	AddExState(&VRC6Sound.saw.phaseacc,      1, 0, "SWAC");
	AddExState(&VRC6Sound.saw.freq,          2, 0, "SWFR");
	AddExState(&VRC6Sound.saw.dcount,        1, 0, "SWDC");
	AddExState(&VRC6Sound.saw.vcount,        4, 0, "SWVC");
	AddExState(&VRC6Sound.saw.cvbc,          4, 0, "SWBC");
}