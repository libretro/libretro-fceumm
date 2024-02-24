/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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
 */

/* mapper 111 - Cheapocabra board by Memblers
 * http://forums.nesdev.com/viewtopic.php?p=146039
 *
 * 512k PRG-ROM in 32k pages (flashable if battery backed is specified)
 * 32k CHR-RAM used as:
 *     2 x 8k pattern pages
 *     2 x 8k nametable pages
 *
 * Notes:
 * - CHR-RAM for nametables maps to $3000-3FFF as well, but FCEUX internally mirrors to 4k?
 */

#include "mapinc.h"
#include "flashrom.h"
#include "mmc1.h"

static uint8 reg;

static uint8 *FLASHROM = NULL;

static SFORMAT StateRegs[] = {
	{ &reg, 1, "REGS" },
	{ 0 }
};

static void M111MMC1PW(uint16 A, uint16 V) {
	setprg16(A, V & 0x0F);
}

static void M111MMC1CW(uint16 A, uint16 V) {
	setchr4(A, V & 0x3F);
}

static DECLFW(M111Write_mmc1) {
	mmc1.reg[(A >> 13) & 0x03] =  V;
	MMC1_FixPRG();
	MMC1_FixCHR();
	MMC1_FixMIR();
}

static void M111Power_mmc1(void) {
	MMC1_Power();
	SetWriteHandler(0x8000, 0xFFFF, M111Write_mmc1);
}

#define VRAM_OFFSET(x) (0x4000 + (0x400 * (x)))

static void Sync(void) {
	/* 7  bit  0
	 * ---- ----
	 * GRNC PPPP
	 * |||| ||||
	 * |||| ++++- Select 32 KB PRG ROM bank for CPU $8000-$FFFF
	 * |||+------ Select 8 KB CHR RAM bank for PPU $0000-$1FFF
	 * ||+------- Select 8 KB nametable for PPU $2000-$3EFF
	 * |+-------- Red LED - 0=On; 1=Off
	 * +--------- Green LED - 0=On; 1=Off */
	int nt  = (reg & 0x20) >> 5;
	int chr = (reg & 0x10) >> 4;
	int prg = (reg & 0x0F);

	setprg32r(FLASHROM ? 0x10 : 0, 0x8000, prg);
	setchr8(chr);
	setntamem(CHRptr[0] + VRAM_OFFSET(0) + (nt << 13), 1, 0);
	setntamem(CHRptr[0] + VRAM_OFFSET(1) + (nt << 13), 1, 1);
	setntamem(CHRptr[0] + VRAM_OFFSET(2) + (nt << 13), 1, 2);
	setntamem(CHRptr[0] + VRAM_OFFSET(3) + (nt << 13), 1, 3);
}

static DECLFR(M111ReadOB) {
	reg = cpu.openbus;
	Sync();
	return reg;
}

static DECLFW(M111WriteReg) {
	reg = V;
	Sync();
}

static DECLFR(M111ReadFlash) {
	return FlashROM_Read(A);
}

static DECLFW(M111WriteFlash) {
	FlashROM_Write(A, V);
}

static void M111CPUCycle(int a) {
	FlashROM_CPUCyle(a);
}

static void M111Power(void) {
	reg = 0xFF;
	Sync();

	SetReadHandler(0x5000, 0x5FFF, M111ReadOB);
	SetReadHandler(0x7000, 0x7FFF, M111ReadOB);

	SetWriteHandler(0x5000, 0x5FFF, M111WriteReg);
	SetWriteHandler(0x7000, 0x7FFF, M111WriteReg);

	SetReadHandler(0x8000, 0xFFFF, M111ReadFlash);
	SetWriteHandler(0x8000, 0xFFFF, M111WriteFlash);
}

static void M111Close(void) {
	if (FLASHROM) {
		FCEU_gfree(FLASHROM);
	}
	FLASHROM = NULL;
}

static void StateRestore(int version) {
	if (!UNIFchrrama) {
		MMC1_FixPRG();
		MMC1_FixCHR();
		MMC1_FixMIR();
		return;
	}

	Sync();
}

void Mapper111_Init(CartInfo *info) {
	if (!UNIFchrrama) {
		/* Prior to the introduction of GTROM, Mapper 111 was assigned to a Chinese Fan Translation
		 * of Ninja Ryukenden (Japanese Ninja Gaiden). This translation uses a non-serialized
		 * version of MMC1 and supports 256KiB of CHR-ROM, whereas the official MMC1 is limited to
		 * 128KiB */
		info->Power = M111Power_mmc1;
		MMC1_pwrap = M111MMC1PW;
		MMC1_cwrap = M111MMC1CW;
		return;
	}

	info->Power = M111Power;
	info->Close = M111Close;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	if (info->battery) {
		int fsize = PRGsize[0];

		FLASHROM = (uint8 *)FCEU_gmalloc(fsize);
		info->SaveGame[0] = FLASHROM;
		info->SaveGameLen[0] = fsize;
		AddExState(FLASHROM, fsize, 0, "FROM");
		memcpy(FLASHROM, PRGptr[0], fsize);
		SetupCartPRGMapping(0x10, FLASHROM, fsize, 0);

		FlashROM_Init(FLASHROM, fsize, 0xB7, 0xBF, 4096, 0x5555, 0x2AAA);
		MapIRQHook = M111CPUCycle;
	}
}
