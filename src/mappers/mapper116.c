/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2011 CaH4e3
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
 * SL12 Protected 3-in-1 mapper hardware (VRC2, MMC3, MMC1)
 * the same as 603-5052 board (TODO: add reading registers, merge)
 * SL1632 2-in-1 protected board, similar to SL12 (TODO: find difference)
 *
 * Known PCB:
 *
 * Garou Densetsu Special (G0904.PCB, Huang-1, GAL dip: W conf.)
 * Kart Fighter (008, Huang-1, GAL dip: W conf.)
 * Somari (008, C5052-13, GAL dip: P conf., GK2-P/GK2-V maskroms)
 * Somari (008, Huang-1, GAL dip: W conf., GK1-P/GK1-V maskroms)
 * AV Mei Shao Nv Zhan Shi (aka AV Pretty Girl Fighting) (SL-12 PCB, Hunag-1, GAL dip: unk conf. SL-11A/SL-11B maskroms)
 * Samurai Spirits (Full version) (Huang-1, GAL dip: unk conf. GS-2A/GS-4A maskroms)
 * Contra Fighter (603-5052 PCB, C5052-3, GAL dip: unk conf. SC603-A/SCB603-B maskroms)
 *
 */

#include "mapinc.h"
#include "vrc2and4.h"
#include "mmc3.h"
#include "mmc1.h"

#define MODE_MMC1 mode & 0x02
#define MODE_MMC3 mode & 0x01

static uint8 mode = 0;
static uint8 game = 0;

static SFORMAT StateRegs[] = {
	{ &mode, 1, "MODE"},
	{ &game, 1, "GAME"},
	{ 0 }
};

static uint32 GetPRGMask(void) {
	if (iNESCart.submapper != 3) {
		return 0x3F;
	}
	return (game ? 0x0F : 0x1F);
}

static uint32 GetPRGBase(void) {
	if (game) {
		return (game + 1) * 0x10;
	}
	return 0;
}

static uint32 GetCHRMask(void) {
	return (game ? 0x7F : 0xFF);
}

static uint32 GetCHRBase(void) {
	return (game ? (game + 1) * 0x80 : 0);
}

static void M116VRC24PW(uint16 A, uint16 V) {
	setprg8(A, GetPRGBase() | (V & GetPRGMask()));
}

static void M116VRC24CW(uint16 A, uint16 V) {
	setchr1(A, ((mode << 6) & 0x100) | GetCHRBase() | (V & GetCHRMask()));
}

static void M116MMC3PW(uint16 A, uint16 V) {
	setprg8(A, GetPRGBase() | (V & GetPRGMask()));
}

static void M116MMC3CW(uint16 A, uint16 V) {
	setchr1(A, ((mode << 6) & 0x100) | GetCHRBase() | (V & GetCHRMask()));
}

static void M116MMC1PW(uint16 A, uint16 V) {
	if (iNESCart.submapper == 2) {
		setprg16(A, V >> 1);
	} else {
		setprg16(A, (GetPRGBase() >> 1) | (V & (GetPRGMask() >> 1)));
	}
}

static void M116MMC1CW(uint16 A, uint16 V) {
	setchr4(A, (GetCHRBase() >> 2) | (V & (GetCHRMask() >> 2)));
}

static void Sync(void) {
	if (MODE_MMC1) {
		MMC1_FixPRG();
		MMC1_FixCHR();
		MMC1_FixMIR();
	} else if (MODE_MMC3) {
		MMC3_FixPRG();
		MMC3_FixCHR();
		MMC3_FixMIR();
	} else {
		VRC24_FixPRG();
		VRC24_FixCHR();
		VRC24_FixMIR();
	}
}

static void applyMode(void) {
	if (MODE_MMC1) {
		SetWriteHandler(0x8000, 0xFFFF, MMC1_Write);
		if (iNESCart.submapper != 1) {
			MMC1_Write(0x8000, 0x80);
		}
	} else if (MODE_MMC3) {
		SetWriteHandler(0x8000, 0xFFFF, MMC3_Write);
	} else {
		SetWriteHandler(0x8000, 0xFFFF, VRC24_Write);
	}
}

static DECLFW(M116ModeWrite) {
	if (A & 0x100) {
		mode = V;
		applyMode();
		Sync();
	}
}

static void M116HBIRQ(void) {
	if ((mode & 0x03) == 0x01) {
		MMC3_IRQHBHook();
	}
}

static void StateRestore(int version) {
	Sync();
}

static void M116Reset(void) {
	if (iNESCart.submapper == 3) {
		game = game + 1;
		if (game > 4) {
			game = 0;
		}
	}
	applyMode();
	Sync();
}

static void M116Power(void) {
	game = (iNESCart.submapper == 3) ? 4 : 0;
	mode = 1;

	MMC3_Power();
	MMC1_Reset();
	VRC24_Power();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x4100, 0x5FFF, M116ModeWrite);

	vrc24.chr[0] = ~0;
	vrc24.chr[1] = ~0;
	vrc24.chr[2] = ~0;
	vrc24.chr[3] = ~0;

	applyMode();
	Sync();
}

void Mapper116_Init(CartInfo *info) {
	VRC24_Init(info, VRC2, 0x01, 0x02, FALSE, TRUE);
	VRC24_pwrap = M116VRC24PW;
	VRC24_cwrap = M116VRC24CW;

	MMC3_Init(info, FALSE, FALSE);
	MMC3_pwrap = M116MMC3PW;
	MMC3_cwrap = M116MMC3CW;

	MMC1_Init(info, FALSE, FALSE);
	MMC1_pwrap = M116MMC1PW;
	MMC1_cwrap = M116MMC1CW;
	mmc1_type = MMC1A;

	info->Power = M116Power;
	info->Reset = M116Reset;

	GameHBIRQHook = M116HBIRQ;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	/* PRG 128K and CHR 128K is Huang-2 (iNESCart.submapper 2) */
	if (((ROM.prg.size * 16) == 128) && ((ROM.chr.size * 8) == 128)) {
		info->submapper = 2;
	}
}
