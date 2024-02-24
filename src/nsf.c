/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <string/stdstring.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "video.h"
#include "sound.h"
#include "nsf.h"
#include "nsfe.h"
#include "general.h"
#include "fceu-memory.h"
#include "file.h"
#include "fds.h"
#include "fdssound.h"
#include "cart.h"
#include "input.h"
#include "state.h"

#include "mappers/sound/vrc6sound.h"
#include "mappers/sound/vrc7sound.h"
#include "mappers/sound/fdssound.h"
#include "mappers/sound/mmc5sound.h"
#include "mappers/sound/n163sound.h"
#include "mappers/sound/s5bsound.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FIXED_EXWRAM_SIZE (32768 + 8192)

static DECLFW(NSF_write);
static DECLFR(NSF_read);

static int vismode = 1;

static uint8 NSFROM[0x30 + 6] =
{
/* 0x00 - NMI */
	0x8D, 0xF4, 0x3F,	/* Stop play routine NMIs. */
	0xA2, 0xFF, 0x9A,	/* Initialize the stack pointer. */
	0xAD, 0xF0, 0x3F,	/* See if we need to init. */
	0xF0, 0x09,			/* If 0, go to play routine playing. */

	0xAD, 0xF1, 0x3F,	/* Confirm and load A      */
	0xAE, 0xF3, 0x3F,	/* Load X with PAL/NTSC byte */

	0x20, 0x00, 0x00,	/* JSR to init routine     */

	0xA9, 0x00,
	0xAA,
	0xA8,
	0x20, 0x00, 0x00,	/* JSR to play routine  */
	0x8D, 0xF5, 0x3F,	/* Start play routine NMIs. */
	0x90, 0xFE,			/* Loopie time. */

/* 0x20 */
	0x8D, 0xF3, 0x3F,	/* Init init NMIs */
	0x18,
	0x90, 0xFE			/* Loopie time. */
};

static DECLFR(NSFROMRead) {
	return (NSFROM - 0x3800)[A];
}

static int lastJoy;
static int doreset = 0;
static int NSFNMIFlags;

static uint8 BSon;
static int special = 0;
static uint8 *ExWRAM = 0;

static uint8 SongReload;

static uint8 *mmc5_exram;
static uint8 mmc5_mul[2];

NSFINFO *NSFInfo;

static SFORMAT StateRegs[] = {
	{ &lastJoy,     1, "JOY" },
	{ &SongReload,  1, "SREL" },
	{ &doreset,     1, "DORE" },
	{ &NSFNMIFlags, 1, "NMIF" },
	{ 0 }
};

static void FreeNSF(void) {
	if (mmc5_exram) {
		free(mmc5_exram);
		mmc5_exram = NULL;
	}

	if (ExWRAM) {
		free(ExWRAM);
		ExWRAM = 0;
	}

	if (NSFInfo) {
		if (NSFInfo->NSFDATA) {
			free(NSFInfo->NSFDATA);
			NSFInfo->NSFDATA = NULL;
		}

		free(NSFInfo);
		NSFInfo = NULL;
	}
}

static void NSFGI(int h) {
	switch (h) {
	case GI_CLOSE:
		FreeNSF();
		break;
	case GI_RESETM2:
	case GI_POWER:
		NSF_init();
		break;
	}
}

/* First 32KB is reserved for sound chip emulation in the iNES mapper code. */

static INLINE void BANKSET(uint32 A, uint32 bank) {
	bank &= NSFInfo->NSFMaxBank;
	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		memcpy(ExWRAM + (A - 0x6000), NSFInfo->NSFDATA + (bank << 12), 4096);
	} else {
		setprg4(A, bank);
	}
}

static int LoadNSF(FCEUFILE *fp) {
	size_t tmp_size = FCEU_fgetsize(fp) - 0x80;
	NSF_HEADER NSFHeader;

	FCEU_fread(&NSFHeader, 1, 0x80, fp);

	/* NULL-terminate strings just in case. */
	NSFHeader.GameName[31] = NSFHeader.Artist[31] = NSFHeader.Copyright[31] = 0;

	sprintf((char *)NSFInfo->SongName,  "%s", (const char *)NSFHeader.GameName);
	sprintf((char *)NSFInfo->Artist,    "%s", (const char *)NSFHeader.Artist);
	sprintf((char *)NSFInfo->Copyright, "%s", (const char *)NSFHeader.Copyright);

	NSFInfo->LoadAddr = NSFHeader.LoadAddressLow | (NSFHeader.LoadAddressHigh << 8);
	NSFInfo->InitAddr = NSFHeader.InitAddressLow | (NSFHeader.InitAddressHigh << 8);
	NSFInfo->PlayAddr = NSFHeader.PlayAddressLow | (NSFHeader.PlayAddressHigh << 8);

	if (tmp_size > 16 * 1024 * 1024) {
		FCEU_PrintError("NSF is too large.\n");
		return FALSE;
	}
	
	NSFInfo->NSFSize = tmp_size;

	NSFInfo->NSFMaxBank = ((NSFInfo->NSFSize + (NSFInfo->LoadAddr & 0xfff) + 4095) / 4096);
	NSFInfo->NSFMaxBank = PRGsize[0] = uppow2(NSFInfo->NSFMaxBank);

	if (!(NSFInfo->NSFDATA = (uint8 *)FCEU_malloc(NSFInfo->NSFMaxBank * 4096))) {
		return FALSE;
	}

	FCEU_fseek(fp, 0x80, SEEK_SET);
	memset(NSFInfo->NSFDATA, 0x00, NSFInfo->NSFMaxBank * 4096);
	FCEU_fread(NSFInfo->NSFDATA + (NSFInfo->LoadAddr & 0xfff), 1, NSFInfo->NSFSize, fp);

	NSFInfo->NSFMaxBank--;

	NSFInfo->VideoSystem = NSFHeader.VideoSystem;
	NSFInfo->SoundChip = NSFHeader.SoundChip;
	NSFInfo->TotalSongs = NSFHeader.TotalSongs;

	if (NSFHeader.StartingSong == 0) {
		NSFHeader.StartingSong = 1;
	}

	NSFInfo->StartingSong = NSFHeader.StartingSong - 1;
	memcpy(NSFInfo->BankSwitch, NSFHeader.BankSwitch, 8);

	return TRUE;
}

int NSFLoad(FCEUFILE *fp) {
	char magic[5];
	int x;

	NSFInfo = (NSFINFO *)FCEU_malloc(sizeof(NSFINFO));

	FCEU_fseek(fp, 0, SEEK_SET);
	FCEU_fread(&magic, 1, 5, fp);

	if (!memcmp(magic, "NESM\x1a", 5)) {
		FCEU_fseek(fp, 0, SEEK_SET);
		if (!LoadNSF(fp)) {
			FreeNSF();
			return FALSE;
		}
	} else if (!memcmp(magic, "NSFE", 4)) {
		FCEU_fseek(fp, 0, SEEK_SET);
		if (!LoadNSFE(fp)) {
			FreeNSF();
			return FALSE;
		}
	} else {
		return FALSE;
	}

	if (NSFInfo->LoadAddr < 0x6000) {
		FCEU_PrintError("Invalid load address!");
		FreeNSF();
		return FALSE;
	}

	if (NSFInfo->TotalSongs < 1) {
		FCEU_PrintError("Total number of songs is less than 1\n");
		FreeNSF();
		return FALSE;
	}

	BSon = 0;
	for (x = 0; x < 8; x++) {
		BSon |= NSFInfo->BankSwitch[x];
	}

	if (BSon == 0) {
		uint8 BankCounter = 0x00;

		if (((NSFInfo->LoadAddr >> 8) & 0x70) >= 0x70) {
			BSon = 0xFF; /* Ice Climber, and other F000 base address tunes need this */
		} else {
			for (x = ((NSFInfo->LoadAddr >> 8) & 0x70) / 0x10; x < 8; x++) {
				NSFInfo->BankSwitch[x] = BankCounter;
				BankCounter += 0x01;
			}
			BSon = 0;
		}
	}

	for (x = 0; x < 8; x++) {
		BSon |= NSFInfo->BankSwitch[x];
	}

	GameInfo->type = GIT_NSF;
	GameInfo->input[0] = GameInfo->input[1] = SI_GAMEPAD;
	GameInfo->cspecial = SIS_NSF;

	for (x = 0;; x++) {
		if (NSFROM[x] == 0x20) {
			NSFROM[x + 1] = NSFInfo->InitAddr & 0xFF;
			NSFROM[x + 2] = NSFInfo->InitAddr >> 8;
			NSFROM[x + 8] = NSFInfo->PlayAddr & 0xFF;
			NSFROM[x + 9] = NSFInfo->PlayAddr >> 8;
			break;
		}
	}

	if (NSFInfo->VideoSystem == 0) {
		GameInfo->vidsys = GIV_NTSC;
	} else if (NSFInfo->VideoSystem == 1) {
		GameInfo->vidsys = GIV_PAL;
	}

	GameInterface = NSFGI;

	FCEU_printf("\n");
	FCEU_printf("NSF Loaded.\n");
	FCEU_printf("File information:\n");
	FCEU_printf(" Name:       %s\n", NSFInfo->SongName);
	FCEU_printf(" Artist:     %s\n", NSFInfo->Artist);
	FCEU_printf(" Copyright:  %s\n", NSFInfo->Copyright);
	if (NSFInfo->Dumper[0]) {
		FCEU_printf(" Dumper:     %s\n", NSFInfo->Dumper);
	}
	FCEU_printf("\n");
	if (NSFInfo->SoundChip) {
		static char *tab[6] = {
			"Konami VRC6",
			"Konami VRC7",
			"Nintendo FDS",
			"Nintendo MMC5",
			"Namco 163",
			"Sunsoft 5B",
		};

		for (x = 0; x < 6; x++) {
			if (NSFInfo->SoundChip & (1 << x)) {
				FCEU_printf(" Expansion hardware:  %s\n", tab[x]);
				#if 0
				NSFInfo->SoundChip = 1 << x; /* Prevent confusing weirdness if more than one bit is set. */
				break;
				#endif
			}
		}
	}
	if (BSon) {
		FCEU_printf(" Bank-switched.\n");
	}
	FCEU_printf(" Load address:  $%04x\n", NSFInfo->LoadAddr);
	FCEU_printf(" Init address:  $%04x\n", NSFInfo->InitAddr);
	FCEU_printf(" Play address:  $%04x\n", NSFInfo->PlayAddr);
	FCEU_printf(" %s\n", (NSFInfo->VideoSystem & 1) ? "PAL" : "NTSC");
	FCEU_printf(" Starting song:  %d / %d\n\n", NSFInfo->StartingSong + 1, NSFInfo->TotalSongs);

	ExWRAM = (uint8 *)FCEU_gmalloc(FIXED_EXWRAM_SIZE);

	FCEUI_SetVidSystem(NSFInfo->VideoSystem != 0);
	lastJoy = 0;

	/* TODO: hard-clip strings for now because of DrawNSF limtis */
	NSFInfo->SongName[31] = 0;
	NSFInfo->Artist[31] = 0;
	NSFInfo->Copyright[31] = 0;
	NSFInfo->Dumper[31] = 0;
	for (x = 0; x < (int)(sizeof(NSFInfo->SongNames) / sizeof(NSFInfo->SongNames[0])); x++) {
		NSFInfo->SongNames[x][31] = 0;
	}

	NSFInfo->CurrentSong = NSFInfo->StartingSong;

	return TRUE;
}

static DECLFR(NSFVectorRead) {
	if (((NSFNMIFlags & 1) && SongReload) || (NSFNMIFlags & 2) || doreset) {
		if (A == 0xFFFA) {
			return (0x00);
		} else if (A == 0xFFFB) {
			return (0x38);
		} else if (A == 0xFFFC) {
			return (0x20);
		} else if (A == 0xFFFD) {
			doreset = 0;
			return (0x38);
		}
		return (cpu.openbus);
	} else {
		return (CartBR(A));
	}
}

void NSF_init(void) {
	doreset = 1;

/*	NSFInfo->CurrentSong = NSFInfo->StartingSong; */
	SongReload = 0xFF;
	NSFNMIFlags = 0;

	ResetExState(0, 0);
	ResetCartMapping();

	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		SetupCartPRGMapping(0, ExWRAM, FIXED_EXWRAM_SIZE, 1);
		setprg32(0x6000, 0);
		setprg8(0xE000, 4);
		memset(ExWRAM, 0x00, FIXED_EXWRAM_SIZE);
		/* SetWriteHandler(0x6000, 0xDFFF, CartBW); */
		SetReadHandler(0x6000, 0xFFFF, CartBR);
	} else {
		memset(ExWRAM, 0x00, 8192);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		/* SetWriteHandler(0x6000, 0x7FFF, CartBW); */
		SetupCartPRGMapping(0, NSFInfo->NSFDATA, ((NSFInfo->NSFMaxBank + 1) * 4096), 0);
		SetupCartPRGMapping(1, ExWRAM, 8192, 1);
		setprg8r(1, 0x6000, 0);
		SetReadHandler(0x8000, 0xFFFF, CartBR);
	}

	if (BSon) {
		int x;
		for (x = 0; x < 8; x++) {
			if (NSFInfo->SoundChip & NSFSOUND_FDS && x >= 6) {
				BANKSET(0x6000 + (x - 6) * 4096, NSFInfo->BankSwitch[x]);
			}
			BANKSET(0x8000 + x * 4096, NSFInfo->BankSwitch[x]);
		}
	} else {
		int x;
		for (x = (NSFInfo->LoadAddr & 0xF000); x < 0x10000; x += 0x1000) {
			BANKSET(x, ((x - (NSFInfo->LoadAddr & 0xF000)) >> 12));
		}
	}

	SetReadHandler(0xFFFA, 0xFFFD, NSFVectorRead);

	SetWriteHandler(0x2000, 0x3fef, 0);
	SetReadHandler(0x2000, 0x37ff, 0);
	SetReadHandler(0x3836, 0x3FFF, 0);
	SetReadHandler(0x3800, 0x3835, NSFROMRead);

	SetWriteHandler(0x5ff6, 0x5fff, NSF_write);

	SetWriteHandler(0x3ff0, 0x3fff, NSF_write);
	SetReadHandler(0x3ff0, 0x3fff, NSF_read);

	AddExState(StateRegs, ~0, 0, NULL);
	AddExState(ExWRAM, FIXED_EXWRAM_SIZE, 0, "ERAM");
	AddExState(&NSFInfo->CurrentSong, 4 | FCEUSTATE_RLSB, 0, "CURS");

	/* init sound chips */

	if (NSFInfo->SoundChip & NSFSOUND_VRC6) {
		VRC6Sound_ESI();
		SetWriteHandler(0x9000, 0x9003, NSF_write);
		SetWriteHandler(0xA000, 0xA002, NSF_write);
		SetWriteHandler(0xB000, 0xB002, NSF_write);
	}
	if (NSFInfo->SoundChip & NSFSOUND_VRC7) {
		VRC7Sound_ESI();
		SetWriteHandler(0x9010, 0x9010, NSF_write);
		SetWriteHandler(0x9030, 0x9030, NSF_write);
	}
	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		FDSSoundRegReset();
		FDSSound_SC();
		SetReadHandler(0x4040, 0x407F, NSF_read);
		SetReadHandler(0x4090, 0x4090, NSF_read);
		SetReadHandler(0x4092, 0x4092, NSF_read);
		SetWriteHandler(0x4040, 0x408A, NSF_write);
	}
	if (NSFInfo->SoundChip & NSFSOUND_MMC5) {
		MMC5Sound_ESI();
		SetReadHandler(0x5C00, 0x5fef, NSF_read);
		SetReadHandler(0x5205, 0x5206, NSF_read);
		SetWriteHandler(0x5000, 0x5015, NSF_write);
		SetWriteHandler(0x5205, 0x5206, NSF_write);
		SetWriteHandler(0x5C00, 0x5FF5, NSF_write);
		if (mmc5_exram) {
			FCEU_free(mmc5_exram);
		}
		mmc5_exram = (uint8 *)FCEU_malloc(1024);
	}
	if (NSFInfo->SoundChip & NSFSOUND_N163) {
		N163Sound_ESI();
		SetReadHandler(0x4800, 0x4FFF, NSF_read);
		SetWriteHandler(0x4800, 0x4FFF, NSF_write);
		SetWriteHandler(0xF800, 0xFFFF, NSF_write);
	}
	if (NSFInfo->SoundChip & NSFSOUND_S5B) {
		S5BSound_ESI();
		SetWriteHandler(0xC000, 0xCFFF, NSF_write);
		SetWriteHandler(0xE000, 0xEFFF, NSF_write);
	}

	/* sound chips states */

	if (NSFInfo->SoundChip & NSFSOUND_VRC6) {
		VRC6Sound_AddStateInfo();
	}
	if (NSFInfo->SoundChip & NSFSOUND_VRC7) {
		VRC7Sound_AddStateInfo();
	}
	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		FDSSound_AddStateInfo();
	}
	if (NSFInfo->SoundChip & NSFSOUND_MMC5) {
		MMC5Sound_AddStateInfo();
		AddExState(mmc5_exram, 1024, 0, "ExRM");
		AddExState(mmc5_mul, 2, 0, "MUL0");
	}
	if (NSFInfo->SoundChip & NSFSOUND_N163) {
		N163Sound_AddStateInfo();
	}
	if (NSFInfo->SoundChip & NSFSOUND_S5B) {
		S5BSound_AddStateInfo();
	}
}

static DECLFW(NSF_write) {
	if (NSFInfo->SoundChip & NSFSOUND_VRC6) {
		switch (A) {
		case 0x9000:
		case 0x9001:
		case 0x9002:
		case 0x9003:
		case 0xA000:
		case 0xA001:
		case 0xA002:
		case 0xB000:
		case 0xB001:
		case 0xB002:
			VRC6Sound_Write(A, V);
			break;
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_VRC7) {
		switch (A) {
		case 0x9010:
		case 0x9030:
			VRC7Sound_Write(A, V);
			break;
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		if ((A >= 0x6000) && (A <= 0xDFFF)){
			/* FIXME: Conflicts in multi chip modes */
			if (!(NSFInfo->SoundChip & (NSFSOUND_VRC6)) && !(A >= 0xA000 && A <= 0xA002)) {
				CartBW(A, V);
			}
		} else if (A >= 0x4040 && A <= 0x407F) {
			FDSWaveWrite(A, V);
		} else {
			switch (A) {
			case 0x4080: FDSSReg0Write(A, V); break;
			case 0x4082: FDSSReg1Write(A, V); break;
			case 0x4083: FDSSReg2Write(A, V); break;
			case 0x4084: FDSSReg3Write(A, V); break;
			case 0x4085: FDSSReg4Write(A, V); break;
			case 0x4086: FDSSReg5Write(A, V); break;
			case 0x4087: FDSSReg6Write(A, V); break;
			case 0x4088: FDSSReg7Write(A, V); break;
			case 0x4089: FDSSReg8Write(A, V); break;
			case 0x408A: FDSSReg9Write(A, V); break;
			}
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_MMC5) {
		if ((A >= 0x5000) && (A <= 0x5015)) {
			MMC5Sound_Write(A, V);
		} else if (A == 0x5205) {
			mmc5_mul[0] = V;
		} else if (A == 0x5206) {
			mmc5_mul[1] = V;
		} else if ((A >= 0x5c00) && (A <= 0x5FF5)) {
			mmc5_exram[A & 0x3FF] = V;
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_N163) {
		switch (A & 0xF800) {
		case 0xF800:
			N163Sound_Write(A, V);
			break;
		case 0x4800:
			N163Sound_Write(A, V);
			break;
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_S5B) {
		switch (A & 0xF000) {
		case 0xC000:
		case 0xE000:
			S5BSound_Write(A, V);
			break;
		}
	}

	switch (A & 0xF000) {
	case 0x6000:
	case 0x7000:
		/* always mapped to ExWRAM */
		CartBW(A, V);
		break;
	}

	switch (A) {
	case 0x3FF3:
		NSFNMIFlags |= 1;
		break;
	case 0x3FF4:
		NSFNMIFlags &= ~2;
		break;
	case 0x3FF5:
		NSFNMIFlags |= 2;
		break;

	case 0x5FF6:
	case 0x5FF7:
		if (!(NSFInfo->SoundChip & NSFSOUND_FDS)) {
			return;
		}
	/* fallthrough */
	case 0x5FF8:
	case 0x5FF9:
	case 0x5FFA:
	case 0x5FFB:
	case 0x5FFC:
	case 0x5FFD:
	case 0x5FFE:
	case 0x5FFF:
		if (!BSon) {
			return;
		}
		A &= 0xF;
		BANKSET((A * 4096), V);
		break;
	}
}

static DECLFR(NSF_read) {
	int x;

	if (NSFInfo->SoundChip & NSFSOUND_FDS) {
		if ((A >= 0x4040) && (A <= 0x407F)) {
			return FDSWaveRead(A);
		}
		if (A == 0x4090) {
			return FDSEnvVolumeRead(A);
		}
		if (A == 0x4092) {
			return FDSEnvModRead(A);
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_MMC5) {
		if ((A >= 0x5C00) && (A <= 0x5FEF)) {
			return mmc5_exram[A & 0x3FF];
		}
		if (A == 0x5205) {
			return (uint8)(uint32)(mmc5_mul[0] * mmc5_mul[1]);
		}
		if (A == 0x5206) {
			return (uint8)((uint32)(mmc5_mul[0] * mmc5_mul[1]) >> 8);
		}
	}

	if (NSFInfo->SoundChip & NSFSOUND_N163) {
		if ((A >= 0x4800) && (A <= 0x4FFF)) {
			return N163Sound_Read(A);
		}
	}

	switch (A) {
	case 0x3ff0:
		x = SongReload;
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
			SongReload = 0;
		return x;
	case 0x3ff1:
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
		{
			for (x = 0; x < 0x800; x++) {
				BWrite[x](x, 0);
			}

			BWrite[0x4015](0x4015, 0x0);
			for (x = 0; x < 0x14; x++) {
				BWrite[0x4000 + x](0x4000 + x, 0);
			}
			BWrite[0x4015](0x4015, 0xF);

			if (NSFInfo->SoundChip & NSFSOUND_FDS) {
				BWrite[0x4017](0x4017, 0xC0); /* FDS BIOS writes $C0 */
				BWrite[0x4089](0x4089, 0x80);
				BWrite[0x408A](0x408A, 0xE8);
			} else {
				memset(ExWRAM, 0x00, 8192);
				BWrite[0x4017](0x4017, 0xC0);
				BWrite[0x4017](0x4017, 0xC0);
				BWrite[0x4017](0x4017, 0x40);
			}

			if (BSon) {
				for (x = 0; x < 8; x++) {
					BANKSET(0x8000 + x * 4096, NSFInfo->BankSwitch[x]);
				}
			}
			return (NSFInfo->CurrentSong);
		}
	case 0x3FF3:
		return isPAL;
	}
	return cpu.openbus;
}

void DrawNSF(uint8 *target) {
	char snbuf[16];
	int32 *Bufpl;
	int32 mul = 0;
	int len = GetSoundBuffer(&Bufpl);
	int x;
	uint8 bgFill = 0;
	uint8 textColor = 0x06;
	uint8 waveFormColor = 0x03;

	if (vismode == 0) {
		return;
	}

	memset(target, bgFill, 256 * 240);
	memset(XDBuf, 0, 256 * 240);

	if (special == 0) {
		if (FSettings.volume[SND_MASTER]) {
			mul = 8192 * 240 / (16384 * FSettings.volume[SND_MASTER] / 50);
		}
		for (x = 0; x < 256; x++) {
			uint32 y = 142 + ((((int32)(int16)Bufpl[(x * len) >> 8]) * mul) >> 14);
			if (y < 240) {
				target[x + y * 256] = waveFormColor;
			}
		}
	} else if (special == 1) {
		if (FSettings.volume[SND_MASTER]) {
			mul = 8192 * 240 / (8192 * FSettings.volume[SND_MASTER] / 50);
		}
		for (x = 0; x < 256; x++) {
			double r = (((int32)(int16)Bufpl[(x * len) >> 8]) * mul) >> 14;
			uint32 xp = 128 + r * cos(x * M_PI * 2 / 256);
			uint32 yp = 120 + r * sin(x * M_PI * 2 / 256);

			xp &= 255;
			yp %= 240;
			target[xp + yp * 256] = waveFormColor;
		}
	} else if (special == 2) {
		static double theta = 0;

		if (FSettings.volume[SND_MASTER]) {
			mul = 8192 * 240 / (16384 * FSettings.volume[SND_MASTER] / 50);
		}
		for (x = 0; x < 128; x++) {
			double xc = (double)128 - x;
			double yc = 0.0 - ((double)((((int32)(int16)Bufpl[(x * len) >> 8]) * mul) >> 14));
			double t = (M_PI + atan(yc / xc)) + theta;
			double r = sqrt(xc * xc + yc * yc);
			uint32 m = 128 + r * cos(t);
			uint32 n = 120 + r * sin(t);

			if (m < 256 && n < 240) {
				target[m + n * 256] = waveFormColor;
			}
		}
		for (x = 128; x < 256; x++) {
			double xc = (double)x - 128;
			double yc = (double)((((int32)(int16)Bufpl[(x * len) >> 8]) * mul) >> 14);
			double t = (atan(yc / xc)) + theta;
			double r = sqrt(xc * xc + yc * yc);
			uint32 m = 128 + r * cos(t);
			uint32 n = 120 + r * sin(t);

			if (m < 256 && n < 240) {
				target[m + n * 256] = waveFormColor;
			}
		}
		theta += (double)M_PI / 256;
	}

	DrawTextTrans(target + 10 * 256 + 4 + (((31 - strlen((char *)NSFInfo->SongName)) << 2)), 256, (uint8*)NSFInfo->SongName, textColor);
	DrawTextTrans(target + 26 * 256 + 4 + (((31 - strlen((char *)NSFInfo->Artist)) << 2)), 256, (uint8*)NSFInfo->Artist, textColor);
	DrawTextTrans( target + 42 * 256 + 4 + (((31 - strlen((char *)NSFInfo->Copyright)) << 2)), 256, (uint8*)NSFInfo->Copyright, textColor);

	if (NSFInfo->SongNames[0][0]) {
		DrawTextTrans(target + 70 * 256 + 4 + (((31 - strlen((char *)NSFInfo->SongNames[NSFInfo->CurrentSong])) << 2)), 256, (uint8 *)NSFInfo->SongNames[NSFInfo->CurrentSong], textColor);
	} else  {
		DrawTextTrans(target + 70 * 256 + 4 + (((31 - strlen("Song:")) << 2)), 256, (uint8 *)"Song:", textColor);
	}
	sprintf(snbuf, "<%d/%d>", NSFInfo->CurrentSong + 1, NSFInfo->TotalSongs);
	DrawTextTrans(target + 82 * 256 + 4 + (((31 - strlen(snbuf)) << 2)), 256, (uint8 *)snbuf, textColor);
}

void DoNSFFrame(void) {
	uint8 tmp = FCEU_GetJoyJoy();

	if (((NSFNMIFlags & 1) && SongReload) || (NSFNMIFlags & 2)) {
		TriggerNMI();
	}

	if ((tmp & JOY_RIGHT) && !(lastJoy & JOY_RIGHT)) {
		if (NSFInfo->CurrentSong < NSFInfo->TotalSongs - 1) {
			NSFInfo->CurrentSong++;
		} else {
			NSFInfo->CurrentSong = NSFInfo->StartingSong;
		}
		SongReload = 0xFF;
	} else if ((tmp & JOY_LEFT) && !(lastJoy & JOY_LEFT)) {
		if (NSFInfo->CurrentSong > 0) {
			NSFInfo->CurrentSong--;
		} else {
			NSFInfo->CurrentSong = NSFInfo->TotalSongs - 1;
		}
		SongReload = 0xFF;
	} else if ((tmp & JOY_UP) && !(lastJoy & JOY_UP)) {
		uint32 ns = NSFInfo->CurrentSong + (uint32)MIN(NSFInfo->TotalSongs - 1 - NSFInfo->CurrentSong, 10);
		if (NSFInfo->CurrentSong != ns) {
			NSFInfo->CurrentSong = ns;
			SongReload = 0xFF;
		}
	} else if ((tmp & JOY_DOWN) && !(lastJoy & JOY_DOWN)) {
		unsigned ns = NSFInfo->CurrentSong - (uint32)MIN(NSFInfo->CurrentSong, (uint32)10);

		if (NSFInfo->CurrentSong != ns) {
			NSFInfo->CurrentSong = ns;
			SongReload = 0xFF;
		}
	} else if ((tmp & JOY_SELECT) && !(lastJoy & JOY_SELECT)) {
		NSFInfo->CurrentSong = NSFInfo->StartingSong;
		SongReload = 0xFF;
	} else if ((tmp & JOY_START) && !(lastJoy & JOY_START)) {
		SongReload = 0xFF;
	} else if ((tmp & JOY_A) && !(lastJoy & JOY_A)) {
		special = (special + 1) % 3;
	}
	lastJoy = tmp;
}

void FCEUI_NSFSetVis(int mode) {
	vismode = mode;
}

int FCEUI_NSFChange(int amount) {
	NSFInfo->CurrentSong += amount;
	if (NSFInfo->CurrentSong < 1) {
		NSFInfo->CurrentSong = 1;
	} else if (NSFInfo->CurrentSong > NSFInfo->TotalSongs) {
		NSFInfo->CurrentSong = NSFInfo->TotalSongs;
	}
	SongReload = 0xFF;

	return (NSFInfo->CurrentSong);
}

/* Returns total songs */
int FCEUI_NSFGetInfo(uint8 *name, uint8 *artist, uint8 *copyright, int maxlen) {
	strncpy((char *)name, (const char *)NSFInfo->SongName, (size_t)maxlen);
	strncpy((char *)artist, (const char *)NSFInfo->Artist, (size_t)maxlen);
	strncpy((char *)copyright, (const char *)NSFInfo->Copyright, (size_t)maxlen);
	return (NSFInfo->TotalSongs);
}
