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

extern EXPSOUND GameExpSound;

extern int32 nesincsize;

void SetSoundVariables(void);

int GetSoundBuffer(int32 **W);
int FlushEmulateSound(void);
extern int32 Wave[2048 + 512];
extern int32 WaveFinal[2048 + 512];
extern int32 WaveHi[];
extern uint32 soundtsinc;

extern uint32 soundtsoffs;
#define SOUNDTS (sound_timestamp + soundtsoffs)

void SetNESSoundMap(void);
void FrameSoundUpdate(void);

void FCEUSND_Power(void);
void FCEUSND_Reset(void);
void FCEUSND_SaveState(void);
void FCEUSND_LoadState(int version);

void FCEU_SoundCPUHook(int);

#define APU_SQUARE1        0
#define APU_SQUARE2        1
#define APU_TRIANGLE       2
#define APU_NOISE          3
#define APU_DPCM           4
#define APU_FDS            5
#define APU_S5B            6
#define APU_N163           7
#define APU_VRC6           8
#define APU_VRC7           9
#define APU_MMC5           10

#include "fceu.h"
/* Modify channel wave volume based on volume modifiers
 * Note: the formulat x = x * y /256 does not yield exact results,
 * but is "close enough" and avoids the need for using double values
 * or implicit cohersion which are slower (we need speed here) */
/* TODO: Optimize this. */
static INLINE int32 GetVolume(int channel, int32 in) {
	int mod = FSettings.volume[channel];

	if (in == 0) return 0; /* do nothing */
	if (mod == 256) return in; /* no change */
	return ((in * mod) >> 8); /* return modified volume levels */
}

#endif
