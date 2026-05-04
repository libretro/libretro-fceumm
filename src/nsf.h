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

#ifndef _FCEU_NSF_H
#define _FCEU_NSF_H

#include "file.h"

typedef struct {
	char ID[5];				/* NESM^Z */
	uint8_t Version;
	uint8_t TotalSongs;
	uint8_t StartingSong;
	uint8_t LoadAddressLow;
	uint8_t LoadAddressHigh;
	uint8_t InitAddressLow;
	uint8_t InitAddressHigh;
	uint8_t PlayAddressLow;
	uint8_t PlayAddressHigh;
	uint8_t SongName[32];
	uint8_t Artist[32];
	uint8_t Copyright[32];
	uint8_t NTSCspeed[2];		/* Unused */
	uint8_t BankSwitch[8];
	uint8_t PALspeed[2];		/* Unused */
	uint8_t VideoSystem;
	uint8_t SoundChip;
	uint8_t Expansion[4];
	uint8_t reserve[8];
} NSF_HEADER;
void NSF_init(void);
void DrawNSF(uint8_t *XBuf);
void NSFDealloc(void);
void NSFDodo(void);
void DoNSFFrame(void);
int NSFLoad(FCEUFILE *fp);

#endif
