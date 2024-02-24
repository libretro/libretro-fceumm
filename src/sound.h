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

#ifndef _FCEU_SOUND_H
#define _FCEU_SOUND_H

enum AUDIO_CHANNEL {
	/* NES APU */
	SND_MASTER  = 0,
	SND_SQUARE1  = 1,
	SND_SQUARE2  = 2,
	SND_TRIANGLE = 3,
	SND_NOISE    = 4,
	SND_DMC      = 5,

	/* EXPANSION AUDIO */
	/* index beyond this line also affects GameExpSound struct index */

	SND_VRC6     = 6,
	SND_VRC7     = 7,
	SND_FDS      = 8,
	SND_N163     = 9,
	SND_S5B      = 10,
	SND_MMC5     = 11,
	
	SND_LAST = 12
};

typedef struct {
	uint8 Speed;
	uint8 Mode;
	uint8 DecCountTo1;
	uint8 decvolume;
	int reloaddec;
} ENVUNIT;

typedef struct {
	void (*Fill)(int Count);	/* Low quality ext sound. */

	/* NeoFill is for sound devices that are emulated in a more
		high-level manner(VRC7) in HQ mode.  Interestingly,
		this device has slightly better sound quality(updated more
		often) in lq mode than in high-quality mode.  Maybe that
		should be fixed. :)
	*/
	void (*NeoFill)(int32 *Wave, int Count);
	void (*HiFill)(void);
	void (*HiSync)(int32 ts);

	void (*RChange)(void);
	void (*Kill)(void);
} EXPSOUND;

#define GAMEEXPSOUND_COUNT 6
extern EXPSOUND GameExpSound[GAMEEXPSOUND_COUNT];

extern int32 nesincsize;

void SetSoundVariables(void);

int GetSoundBuffer(int32 **W);
int FlushEmulateSound(void);
extern int32 Wave[8192 + 512];
extern int32 WaveFinal[8192 + 512];
extern int32 WaveHi[];
extern uint32 soundtsinc;

extern uint32 soundtsoffs;
#define SOUNDTS (sound_timestamp + soundtsoffs)

void FCEUSND_Power(void);
void FCEUSND_Reset(void);
void FCEUSND_SaveState(void);
void FCEUSND_LoadState(int version);

void FCEU_SoundCPUHook(int);

/* Modify channel wave volume based on volume modifiers
 * Note: the formulat x = x * y /256 does not yield exact results,
 * but is "close enough" and avoids the need for using double values
 * or implicit cohersion which are slower (we need speed here) */
/* TODO: Optimize this. */
int32 GetOutput(int channel, int32 in);

#endif
