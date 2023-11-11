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
#include <stdlib.h>
#include "share.h"

static uint32 FTVal, FTValR;
static char side;

static uint8 FT_Read(int w, uint8 ret) {
	if (w)
		ret |= FTValR;
	return(ret);
}

static void FT_Write(uint8 V) {
	FTValR = 0;

	if (!(V & 0x1))
		FTValR = (FTVal >> 8);
	else if (!(V & 0x2))
		FTValR = (FTVal >> 4);
	else if (!(V & 0x4))
		FTValR = FTVal;

	FTValR = (~FTValR) & 0xF;
	if (side == 'B')
		FTValR = ((FTValR & 0x8) >> 3) | ((FTValR & 0x4) >> 1) | ((FTValR & 0x2) << 1) | ((FTValR & 0x1) << 3);
	FTValR <<= 1;
}

static void FT_Update(void *data, int arg) {
	FTVal = *(uint32*)data;
}

static INPUTCFC FamilyTrainer = { FT_Read, FT_Write, 0, FT_Update, 0, 0 };

INPUTCFC *FCEU_InitFamilyTrainerA(void) {
	side = 'A';
	FTVal = FTValR = 0;
	return(&FamilyTrainer);
}

INPUTCFC *FCEU_InitFamilyTrainerB(void) {
	side = 'B';
	FTVal = FTValR = 0;
	return(&FamilyTrainer);
}

