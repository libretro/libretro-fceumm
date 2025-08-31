/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2022 NewRisingSun
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

#include "mapinc.h"
#include "asic_h3001.h"
#include "asic_latch.h"
#include "asic_mmc1.h"
#include "asic_mmc2.h"
#include "asic_mmc3.h"
#include "asic_pt8154.h"
#include "asic_qj.h"
#include "asic_tc3294.h"
#include "asic_vrc1.h"
#include "asic_vrc2and4.h"
#include "asic_vrc3.h"
#include "asic_vrc6.h"
#include "asic_vrc7.h"
#include "flashrom.h"
#include "wram.h"

static uint8 submapper;
static uint8 reg[8];

static void (*mapperSync)(int, int, int, int) = NULL;
static void applyMode (uint8);

static void sync () {
	int prgAND = reg[3] ^ (submapper == 2? 0x00: 0xFF);
	int prgOR  = reg[1] | reg[2] <<8;
	int chrAND = reg[4] <<2 &0xE0 ^0xFF;
	int chrOR  = reg[6];
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], !(reg[5] &0x04));
	if (mapperSync) mapperSync(prgAND, prgOR, chrAND, chrOR);
}

static void sync_152 (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	chrAND >>=3;
	prgOR >>=1;
	chrOR >>=3;
	setprg16(0x8000, Latch_data >>4 &prgAND | prgOR &~prgAND);
	setprg16(0xC000,                          prgOR | prgAND);
	setchr8(Latch_data &chrAND | chrOR &~chrAND);
	setmirror(Latch_data &0x80? MI_1: MI_0);
}

static void sync_AxROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=2;
	prgOR >>=2;
	setprg32(0x8000, Latch_data &prgAND | prgOR &~prgAND);
	setchr8(chrOR);
	setmirror(Latch_data &0x10? MI_1: MI_0);
}

static void sync_BNROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	setprg8(0x8000, (Latch_data <<2 |0) &prgAND |prgOR);
	setprg8(0xA000, (Latch_data <<2 |1) &prgAND |prgOR);
	setprg8(0xC000, (Latch_data <<2 |2) &prgAND |prgOR);
	setprg8(0xE000, (Latch_data <<2 |3) &prgAND |prgOR);
	setchr8(chrOR);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_CNROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	chrAND >>=3;
	chrOR >>=3;
	setprg8(0x8000, 0 &prgAND |prgOR);
	setprg8(0xA000, 1 &prgAND |prgOR);
	setprg8(0xC000, 2 &prgAND |prgOR);
	setprg8(0xE000, 3 &prgAND |prgOR);
	setchr8(Latch_data &(reg[4] &1? 7: 3));
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_CNROM_Konami (int prgAND, int prgOR, int chrAND, int chrOR) {
	chrAND >>=3;
	chrOR >>=3;
	setprg8(0x8000, 0 &prgAND |prgOR);
	setprg8(0xA000, 1 &prgAND |prgOR);
	setprg8(0xC000, 2 &prgAND |prgOR);
	setprg8(0xE000, 3 &prgAND |prgOR);
	setchr8(Latch_data <<1 &2 | Latch_data >>1 &1);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_GNROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=2;
	chrAND >>=3;
	prgOR >>=2;
	chrOR >>=3;
	setprg32(0x8000, Latch_data >>4 &prgAND | prgOR &~prgAND);
	setchr8(Latch_data &3);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_H3001 (int prgAND, int prgOR, int chrAND, int chrOR) {
	H3001_syncPRG(prgAND, prgOR &~prgAND);
	H3001_syncCHR(chrAND, chrOR &~chrAND);
	H3001_syncMirror();
}

static void sync_PNROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	MMC2_syncPRG(prgAND, prgOR &~prgAND);
	MMC2_syncCHR(chrAND, chrOR &~chrAND);
	MMC2_syncMirror();
}

static void sync_SKROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	chrAND >>=2;
	prgOR >>=1;
	chrOR >>=2;
	MMC1_syncWRAM(reg[5]);
	MMC1_syncPRG(prgAND, prgOR &~prgAND);
	MMC1_syncCHR(chrAND, chrOR &~chrAND);
	MMC1_syncMirror();
}

static void sync_SNROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	chrAND >>=2;
	prgOR >>=1;
	chrOR >>=2;
	MMC1_syncWRAM(reg[5]);
	MMC1_syncPRG(prgAND, prgOR &~prgAND);
	MMC1_syncCHR(chrAND, chrOR &~chrAND);
	MMC1_syncMirror();
}

static void sync_SUROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	chrAND =chrAND >>2 &0x0F; /* The highest CHR bit switches 256 KiB PRG banks, so don't use that as a CHR bank bit. */
	prgOR >>=1;
	chrOR >>=2;
	MMC1_syncWRAM(reg[5]);
	MMC1_syncPRG(prgAND, prgOR &~prgAND);
	MMC1_syncCHR(chrAND, chrOR &~chrAND);
	MMC1_syncMirror();
}

static void sync_PT8154 (int prgAND, int prgOR, int chrAND, int chrOR) {
	PT8154_syncPRG(prgAND, prgOR &~prgAND);
	PT8154_syncCHR(chrAND, chrOR &~chrAND);
	PT8154_syncMirror();
}

static void sync_QJ (int prgAND, int prgOR, int chrAND, int chrOR) {
	QJ_syncPRG(prgAND, prgOR &~prgAND);
	QJ_syncCHR(chrAND, chrOR &~chrAND);
	QJ_syncMirror();
}
static void sync_TC3294(int prgAND, int prgOR, int chrAND, int chrOR) {
	TC3294_syncWRAM(reg[5]);
	TC3294_syncPRG(prgAND, prgOR &~prgAND);
	setchr8(0);
	TC3294_syncMirror();
}

static void sync_TxROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	MMC3_syncWRAM(reg[5]);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND, chrOR &~chrAND);
	MMC3_syncMirror();
}

static void sync_TxSROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	MMC3_syncWRAM(reg[5]);
	MMC3_syncPRG(prgAND, prgOR &~prgAND);
	MMC3_syncCHR(chrAND &0x7F, chrOR &~chrAND);
	setmirror(MMC3_getCHRBank(0) &0x80? MI_1: MI_0);
}

static void sync_UxROM (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	prgOR >>=1;
	setprg16(0x8000, Latch_data &prgAND | prgOR &~prgAND);
	setprg16(0xC000,                      prgOR | prgAND);
	setchr8(chrOR);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_VRC1 (int prgAND, int prgOR, int chrAND, int chrOR) {
	VRC1_syncPRG(prgAND, prgOR &~prgAND);
	VRC1_syncCHR(chrAND, chrOR &~chrAND);
	VRC1_syncMirror();
}

static void sync_VRC3 (int prgAND, int prgOR, int chrAND, int chrOR) {
	prgAND >>=1;
	prgOR >>=1;
	VRC3_syncWRAM(reg[5]);
	VRC3_syncPRG(prgAND, prgOR &~prgAND);
	VRC3_syncCHR(chrAND, chrOR &~chrAND);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static void sync_VRC4 (int prgAND, int prgOR, int chrAND, int chrOR) {
	VRC24_syncWRAM(reg[5]);
	VRC24_syncPRG(prgAND, prgOR &~prgAND);
	VRC24_syncCHR(chrAND, chrOR &~chrAND);
	VRC24_syncMirror();
}

static void sync_VRC6 (int prgAND, int prgOR, int chrAND, int chrOR) {
	VRC6_syncWRAM(reg[5]);
	VRC6_syncPRG(prgAND, prgOR &~prgAND);
	VRC6_syncCHR(chrAND, chrOR &~chrAND);
	VRC6_syncMirror();
}

static void sync_VRC7 (int prgAND, int prgOR, int chrAND, int chrOR) {
	VRC7_syncWRAM(reg[5]);
	VRC7_syncPRG(prgAND, prgOR &~prgAND);
	VRC7_syncCHR(chrAND, chrOR &~chrAND);
	VRC7_syncMirror();
}

static void sync_supervisor (int prgAND, int prgOR, int chrAND, int chrOR) {
	setprg8(0x8000, prgOR);
	setprg8(0xA000, prgOR +1);
	setprg8(0xC000, submapper == 3? 0x1E: 0x3E);
	setprg8(0xE000, submapper == 3? 0x1F: 0x3F);
	setchr8(chrOR);
	setmirror(reg[4] &0x01? MI_V: MI_H);
}

static DECLFW(writeFlash) {
	flashrom_write(A &0x1FFF | (Page[A >>11] +A -PRGptr[0]) &~0x1FFF, V);
}

static int SUROM_getPRGBank(uint8 bank) {
	return MMC1_getPRGBank(bank) | MMC1_getCHRBank(0) &0x10;
}

static int Mapper22_getCHRBank(uint8 bank) {
	return VRC24_getCHRBank(bank &7) >>1;
}

static DECLFW(writeReg) {
	reg[A &7] = V;
	if ((A &7) == 0)
		applyMode(1);
	else
		sync();
}

static void applyMode (uint8 clear) {
	if (reg[0] &0x80) {
		SetWriteHandler(0x5000, 0x5FFF, CartBW);
		switch(submapper <<8 | reg[0] &0x1F) {
			case 0x000: case 0x100: case 0x200:
				mapperSync = sync_UxROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x001: case 0x105: case 0x205:
				mapperSync = sync_SKROM;
				MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
				break;
			case 0x002: case 0x102: case 0x202: /* NROM or BNROM */
				mapperSync = sync_BNROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x003: case 0x103: case 0x203:
				mapperSync = sync_CNROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x004: case 0x101: case 0x201: case 0x209: /* MMC3 or Namco 118 */
				mapperSync = sync_TxROM;
				MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
				if (clear) MMC3_write(0xA000, reg[4] &0x04? 0: 1);
				break;
			case 0x10E: case 0x20E: /* MMC3 with single-screen mirroring. 239-in-1's Goal! Two has a screen where MMC3 scanline counter emulation fails. */
				mapperSync = sync_TxSROM;
				MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
				break;
			case 0x006:
				mapperSync = sync_VRC4;
				VRC4_activate(clear, sync, 0x42, 0x84, 1, NULL, NULL, NULL, NULL, NULL);
				break;
			case 0x007: case 0x112: case 0x212:
				mapperSync = sync_VRC4;
				VRC2_activate(clear, sync, 0x02, 0x01, NULL, Mapper22_getCHRBank, NULL, NULL);
				break;
			case 0x008: case 0x118: case 0x218:
				mapperSync = sync_VRC4;
				VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
				break;
			case 0x009: case 0x110:
				mapperSync = sync_VRC6;
				VRC6_activate(clear, sync, 0x01, 0x02, NULL, NULL, NULL, NULL);
				break;
			case 0x00A: case 0x115: case 0x215:
				mapperSync = sync_VRC4;
				VRC4_activate(clear, sync, 0x0A, 0x05, 1, NULL, NULL, NULL, NULL, NULL);
				break;
			case 0x00B:
				mapperSync = sync_VRC6;
				VRC6_activate(clear, sync, 0x02, 0x01, NULL, NULL, NULL, NULL);
				break;
			case 0x00C:
				mapperSync = sync_VRC3;
				VRC3_activate(clear, sync);
				break;
			case 0x00D:
				mapperSync = sync_VRC7;
				VRC7_activate(clear, sync, 0x18);
				break;
			case 0x00E:
				mapperSync = sync_CNROM_Konami;
				Latch_activate(clear, sync, 0x6000, 0x7FFF, NULL);
				break;
			case 0x104: case 0x204:
				mapperSync = sync_AxROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x106: case 0x206:
				mapperSync = sync_SNROM;
				MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, NULL, NULL, NULL, NULL);
				break;
			case 0x107: case 0x208:
				mapperSync = sync_SUROM;
				MMC1_activate(clear, sync, MMC1_TYPE_MMC1B, SUROM_getPRGBank, NULL, NULL, NULL);
				break;
			case 0x108:
				mapperSync = sync_GNROM;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x109:
				mapperSync = sync_PNROM;
				MMC2_activate(clear, sync);
				break;
			case 0x10A: case 0x20A:
				mapperSync = sync_TxROM;
				MMC3_activate(clear, sync, MMC3_TYPE_MMC6, NULL, NULL, NULL, NULL);
				break;
			case 0x10B: case 0x20B:
				mapperSync = sync_152;
				Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
				break;
			case 0x10F:
				mapperSync = sync_PT8154;
				PT8154_activate(clear, sync);
				break;
			case 0x119:
				mapperSync = sync_QJ;
				QJ_activate(clear, sync);
				break;
			case 0x11A: case 0x21A:
				mapperSync = sync_VRC1;
				VRC1_activate(clear, sync);
				break;
			case 0x301:
				mapperSync = sync_H3001;
				H3001_activate(clear, sync);
				break;
			case 0x401:
				mapperSync = sync_TC3294;
				TC3294_activate(clear, sync);
				break;
			default:
				break;
		}
	} else {
		SetWriteHandler(0x5000, 0x5FFF, writeReg);
		SetReadHandler(0x8000, 0xFFFF, flashrom_read);
		SetWriteHandler(0x8000, 0xFFFF, writeFlash);
		mapperSync = sync_supervisor;
		PPU_hook = NULL;
		MapIRQHook =flashrom_cpuCycle;
		GameHBIRQHook = NULL;
		sync();
	}
}

static void power() {
	reg[0] = reg[1] = reg[2] = reg[3] = reg[4] = reg[5] = reg[6] = reg[7] = 0;
	applyMode(1);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void stateRestore(int version) {
	applyMode(0);
}

void Mapper446_Init(CartInfo *info) {
	submapper =info->submapper;
	H3001_addExState();
	Latch_addExState();
	MMC1_addExState();
	MMC2_addExState();
	MMC3_addExState();
	VRC1_addExState();
	VRC24_addExState();
	VRC3_addExState();
	VRC6_addExState();
	VRC7_addExState();
	QJ_addExState();
	PT8154_addExState();
	TC3294_addExState();
	WRAM_init(info, 32);
	flashrom_init (0x01, 0x7E, 131072, 0xAAA, 0x555, 0xFFF);
	info->Reset =power;
	info->Power =power;
	GameStateRestore =stateRestore;
	AddExState(reg, 8, 0, "REGS");
}
