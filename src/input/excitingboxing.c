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
Exciting Boxing
*/

static uint8 EBVal = 0;
static uint8 EBValR = 0;
static uint8 EBSensor = 0;

static uint8 EB_Read(int w, uint8 ret) {
	if (w) {
		/* The first read returns the state of buttons 1 to 3,
		the 2nd read gives buttons 4 to 6. The third read apparently returns a detection value ($14). */
		if (!EBSensor) {
			ret |= (
                ((EBValR & 0x01) ? 0 : 0x02) | /* Left Hook */
                ((EBValR & 0x02) ? 0 : 0x04) | /* Move Right */
                ((EBValR & 0x04) ? 0 : 0x08) | /* Move Left */
                ((EBValR & 0x08) ? 0 : 0x10)); /* Right Hook */
		} else {
			ret |= (
                ((EBValR & 0x10) ? 0 : 0x02) | /* Left Jab */
                ((EBValR & 0x20) ? 0 : 0x04) | /* Body */
                ((EBValR & 0x40) ? 0 : 0x08) | /* Right Jab */
                ((EBValR & 0x80) ? 0 : 0x10)); /* Straight */
		}
	}
	return(ret);
}

static void EB_Strobe(void) {
	EBValR = EBVal;
}

static void EB_Write(uint8 V) {
    EBSensor = (V & 0x02) >> 1;
}

static void EB_Update(void *data, int arg) {
	EBVal = *(uint32*)data;
}

static INPUTCFC ExcitingBoxing = { EB_Read, EB_Write, EB_Strobe, EB_Update, 0, 0 };

INPUTCFC *FCEU_InitExcitingBoxing(void) {
	EBVal = EBValR = 0;
	return(&ExcitingBoxing);
}
