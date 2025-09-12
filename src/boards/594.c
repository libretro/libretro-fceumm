/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
 *
 */

#include "mapinc.h"
#include "asic_mmc3.h"
#include "fifo.h"
#include "msm6585.h"

static uint8 reg[4];
static FIFO fifo;
static MSM6585 adpcm;
static int32 soundOffset = 0;

static void sync () {
	int prgAND = 0x3F;
	int chrAND = reg[2] &0xC0? 0x0FF: 0x1FF;
	int prgOR = (reg[2] &0x40? 0x0C0: 0x000) | (reg[2] &0x80? 0x100: 0x000);
	int chrOR = (reg[2] &0x40? 0x200: 0x000) | (reg[2] &0x80? 0x300: 0x000);
	setprg8(0x6000, reg[0] | prgOR);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static int getCHRBank (uint8 bank) {
	return MMC3_getCHRBank(bank) | bank <<6 &0x100;
}

static DECLFR (readADPCM) {
	return FIFO_halfFull(&fifo)? 0x00: 0x40;
}

static DECLFW (writeADPCM) {
	if (A &1) {
		MSM6585_setRate(&adpcm, V >>6);
		FIFO_reset(&fifo);
	} else
		FIFO_add(&fifo, V);
}

static int serveADPCM (void) {
	return FIFO_retrieve(&fifo);
}

static DECLFW (writeReg) {
	reg[A >>12 &2 | A &1] = V;
	sync();
}

static void reset () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_clear();
	FIFO_reset(&fifo);
	MSM6585_reset(&adpcm);
}

static void power () {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	MMC3_power();
	FIFO_reset(&fifo);
	MSM6585_reset(&adpcm);
	SetReadHandler(0x5000, 0x5FFF, readADPCM);
	SetWriteHandler(0x5000, 0x5FFF, writeADPCM);
	SetWriteHandler(0x9000, 0x9FFF, writeReg);
	SetWriteHandler(0xB000, 0xBFFF, writeReg);
}

static void mapperSound_fillBufferLow (int count) {
	int i;
	int end = (SOUNDTS <<16) /soundtsinc;
	for (i = soundOffset; i < end; i++) {
		MSM6585_run(&adpcm);
		Wave[i >>4] += MSM6585_getOutput(&adpcm) >>1;
	}
	soundOffset = count;
}

static void mapperSound_fillBufferHigh () {
	int i;
	for (i = soundOffset; i < SOUNDTS; i++) {
		MSM6585_run(&adpcm);
		WaveHi[i] += MSM6585_getOutput(&adpcm)*8 +16384;
	}
	soundOffset = SOUNDTS;
}

static void mapperSound_setSoundOffset (int32 newSoundOffset) {
	soundOffset = newSoundOffset;
}

static void mapperSound_init (void) {
	if (FSettings.SndRate) {
		GameExpSound.Fill = mapperSound_fillBufferLow;
		GameExpSound.HiFill = mapperSound_fillBufferHigh;
		GameExpSound.HiSync = mapperSound_setSoundOffset;
		GameExpSound.RChange = mapperSound_init;
	}
	MSM6585_init(&adpcm, FSettings.soundq >=1? 1789773: FSettings.SndRate*16, serveADPCM);
}

static void close () {
	FIFO_close(&fifo);
}

void Mapper594_Init (CartInfo *info) {
	MMC3_init(info, sync, MMC3_TYPE_AX5202P, NULL, getCHRBank, NULL, NULL);
	FIFO_init(&fifo, 1024);
	info->Power = power;
	info->Reset = reset;
	info->Close = close;
	mapperSound_init();
	AddExState(reg, 4, 0, "EXPR");
}
