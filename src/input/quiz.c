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

static uint8 QZVal, QZValR;
static uint8 FunkyMode;

static uint8 QZ_Read(int w, uint8 ret) {
	if (w) {
		ret |= (QZValR & 0x7) << 2;
		QZValR = QZValR >> 3;

		if (FunkyMode) {
			QZValR |= 0x28;
		} else {
			QZValR |= 0x38;
		}
	}
	return(ret);
}

static void QZ_Strobe(void) {
	QZValR = QZVal;
}

static void QZ_Write(uint8 V) {
	FunkyMode = V & 4;
}

static void QZ_Update(void *data, int arg) {
	QZVal = *(uint8*)data;
}

static INPUTCFC QuizKing = { QZ_Read, QZ_Write, QZ_Strobe, QZ_Update, 0, 0 };

INPUTCFC *FCEU_InitQuizKing(void) {
	QZVal = QZValR = 0;
	return(&QuizKing);
}
