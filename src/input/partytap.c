/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2003 Xodnizel
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

#include <string.h>
#include "share.h"

/* supported games:
Casino Derby
Gimmi a Break - Shijou Saikyou no Quiz OuKetteisen
Gimmi a Break - Shijou Saikyou no Quiz OuKetteisen 2
Project Q
*/

static uint8 PTVal, PTValR;
static int readcount = 0;

static uint8 PT_Read(int w, uint8 ret) {
	if (w) {
		/* The first read returns the state of buttons 1 to 3,
		the 2nd read gives buttons 4 to 6. The third read apparently returns a detection value ($14). */
		if (readcount < 2) {
			ret |= (PTValR & 0x7) << 2;
			PTValR = PTValR >> 3;
			readcount++;
			return ret;
		} else {
			return (ret | 0x14);
		}
	}
	return(ret);
}

static void PT_Strobe(void) {
	readcount = 0;
	PTValR = PTVal;
}

static void PT_Write(uint8 V) {
}

static void PT_Update(void *data, int arg) {
	PTVal = *(uint8*)data;
}

static INPUTCFC PartyTap = { PT_Read, PT_Write, PT_Strobe, PT_Update, 0, 0 };

INPUTCFC *FCEU_InitPartyTap(void) {
	PTVal = PTValR = 0;
	return(&PartyTap);
}
