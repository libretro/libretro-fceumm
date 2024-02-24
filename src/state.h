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

#ifndef _FCEU_STATE_H_
#define _FCEU_STATE_H_

#include "fceu-memory.h"

/* last known fceu-mm version */
#define FCEU_VERSION_MAJOR 0
#define FCEU_VERSION_MINOR 98
#define FCEU_VERSION_PATCH 13

#define FCEU_VERSION_NUMERIC ((FCEU_VERSION_MAJOR * 10000) + (FCEU_VERSION_MINOR * 100) + (FCEU_VERSION_PATCH))

void FCEUSS_Load_Mem(void);
void FCEUSS_Save_Mem(void);

typedef struct {
	void *v;
	uint32 s;
	char desc[5];
} SFORMAT;

void ResetExState(void (*PreSave)(void), void (*PostSave)(void));
void AddExState(void *v, uint32 s, int type, char *desc);

/* indicates that the value is a multibyte integer that needs to be put in the correct byte order */
#define FCEUSTATE_RLSB 0x80000000

/* void*v is actually a void** which will be indirected before reading */
#define FCEUSTATE_INDIRECT 0x40000000

/* all FCEUSTATE flags together so that we can mask them out and get the size */
#define FCEUSTATE_FLAGS (FCEUSTATE_RLSB | FCEUSTATE_INDIRECT)

#endif
