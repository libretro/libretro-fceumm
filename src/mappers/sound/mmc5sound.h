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

#ifndef _MMC5_AUDIO_H
#define _MMC5_AUDIO_H

typedef struct __MMC5SOUND {
	uint16 wl[2];
	uint8 env[2];
	uint8 enable;
	uint8 running;
	uint8 raw;
	uint8 rawcontrol;
	int32 dcount[2];
	int32 BC[3];
	int32 vcount[2];
} MMC5SOUND;

extern MMC5SOUND MMC5Sound;

DECLFW(MMC5Sound_Write);

void MMC5Sound_ESI(void);

#endif /* _MMC5_AUDIO_H */
