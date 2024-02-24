/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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
 *
 */

#include "mapinc.h"
#include "latch.h"

static int32 adc_data;
static int32 adc_high;
static int32 adc_low;
static uint8 adc_state;

static SFORMAT StateRegs[] = {
	{ &adc_data,  sizeof(adc_data),  "DATA" },
	{ &adc_high,  sizeof(adc_high),  "DTHI" },
	{ &adc_low,   sizeof(adc_low),   "DTLO" },
	{ &adc_state, sizeof(adc_state), "STAT" },
	{ 0 },
};

static void Sync(void) {
	setprg16(0x8000, latch.data);
    setprg16(0xC000, ~0);
	setchr8(0);
}

static DECLFR(M517Read) {
	uint8 result = 0;
	if (A == 0x6000) {
		switch (adc_state) {
		case 0:
			adc_state = 1;
			result    = 0;
			break;
		case 1:
			adc_state = 2;
			result    = 1;
			break;
		case 2:
			if (adc_low > 0) {
				adc_low--;
				result = 1;
			} else {
				adc_state = 0;
				result    = 0;
			}
			break;
		}
	} else {
		result = adc_high-- > 0 ? 0 : 1;
	}
	return result;
}

static DECLFW(M517Write) {
	/* TODO: implement mic input from frontend */
	/* adc_data = MIC * 63.0; */
	adc_data   = 0.0 * 63.0;
	adc_high   = adc_data >> 2;
	adc_low    = 0x40 - adc_high - ((adc_data & 0x03) << 2);
	adc_state  = 0;
	Latch_Write(A,V);
}

static void M517Reset(void) {
	adc_data = 0;
	adc_state = 0;
	Sync();
}

static void M517Power(void) {
	adc_data = 0;
	adc_state = 0;
	Latch_Power();
	SetReadHandler(0x6000, 0x6FFF, M517Read);
	SetWriteHandler(0x8000, 0x8FFF, M517Write);
}

void Mapper517_Init(CartInfo *info) {
	Latch_Init(info, Sync, NULL, FALSE, FALSE);
	info->Power = M517Power;
	info->Reset = M517Reset;
	AddExState(StateRegs, ~0, 0, NULL);
}
