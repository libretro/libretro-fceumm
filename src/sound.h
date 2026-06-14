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

#include "fceu-types.h"

/* Identifiers for the expansion-audio channels exposed to the
 * per-channel volume API (#512).  Indices into FSettings.ExpVolume[].
 * Numbering is internal-only - the savestate format and core options
 * key names do not depend on these values - but the count must match
 * FSettings.ExpVolume[]'s declared length in fceu.h. */
enum {
	SND_FDS  = 0,	/* Famicom Disk System 2C33 wavetable */
	SND_S5B  = 1,	/* Sunsoft 5B / YM2149F (mapper 69) */
	SND_N163 = 2,	/* Namco 163 (mapper 19 / "n106") */
	SND_VRC6 = 3,	/* Konami VRC6 (mappers 24, 26) */
	SND_VRC7 = 4,	/* Konami VRC7 / YM2413-clone (mapper 85) */
	SND_MMC5 = 5,	/* Nintendo MMC5 PCM + square (mapper 5) */
	SND_EXP_LAST = 6
};

/* Apply the per-channel volume modifier from FSettings.ExpVolume[].
 * Scale is 0..256: 256 (default) returns the sample unchanged so the
 * hot path stays free of multiplies on builds where the user has not
 * touched the new options; 0 returns silence; intermediate values
 * scale the sample by (in * vol) / 256.  Used by the six expansion-
 * audio modules (see fds_apu.c, boards/vrc6.c, boards/vrc7.c,
 * boards/n106.c, boards/mmc5.c, boards/69.c). */
int32_t GetExpOutput(int channel, int32_t in);

typedef struct {
	void (*Fill)(int Count);	/* Low quality ext sound. */

	/* NeoFill is for sound devices that are emulated in a more
		high-level manner(VRC7) in HQ mode.  Interestingly,
		this device has slightly better sound quality(updated more
		often) in lq mode than in high-quality mode.  Maybe that
		should be fixed. :)
	*/
	void (*NeoFill)(int32_t *WaveBuf, int Count);
	void (*HiFill)(void);
	void (*HiSync)(int32_t ts);

	void (*RChange)(void);
	void (*Kill)(void);
} EXPSOUND;

extern EXPSOUND GameExpSound;

extern int32_t nesincsize;

void SetSoundVariables(void);

int GetSoundBuffer(int32_t **W);
int FlushEmulateSound(void);
extern int32_t Wave[2048 + 512];
extern int32_t WaveFinal[2048 + 512];
extern int32_t WaveHi[];
extern uint32_t soundtsinc;

extern uint32_t soundtsoffs;
#define SOUNDTS (sound_timestamp + soundtsoffs)

void SetNESSoundMap(void);
void FrameSoundUpdate(void);

void FCEUSND_Power(void);
void FCEUSND_Reset(void);
void FCEUSND_SaveState(void);
void FCEUSND_LoadState(int version);

void FASTAPASS(1) FCEU_SoundCPUHook(int);

#endif
