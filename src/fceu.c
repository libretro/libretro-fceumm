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

#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <time.h>

#include  "fceu.h"
#include  "fceu-types.h"
#include  "x6502.h"
#include  "ppu.h"
#include  "sound.h"
#include  "general.h"
#include  "fceu-endian.h"
#include  "fceu-memory.h"

#include  "cart.h"
#include  "nsf.h"
#include  "fds.h"
#include  "ines.h"
#include  "unif.h"
#include  "cheat.h"
#include  "palette.h"
#include  "state.h"
#include  "video.h"
#include  "input.h"
#include  "file.h"
#include  "crc32.h"
#include  "vsuni.h"
#include  "gamegenie.h"

uint64 timestampbase;

FCEUGI *GameInfo = NULL;
void (*GameInterface)(int h);

void (*GameStateRestore)(int version);

readfunc ARead[0x10000];
writefunc BWrite[0x10000];

static DECLFW(BNull) { }

static DECLFR(ANull) {
	return (cpu.openbus);
}

readfunc GetReadHandler(uint16 a) {
	if (a >= 0x8000 && RWWrap) {
		return AReadG[a - 0x8000];
	} else {
		return ARead[a];
	}
}

void SetReadHandler(uint16 start, uint16 end, readfunc func) {
	int32 x;

	if (!func) {
		func = ANull;
	}

	if (RWWrap) {
		for (x = end; x >= start; x--) {
			if (x >= 0x8000) {
				AReadG[x - 0x8000] = func;
			} else {
				ARead[x] = func;
			}
		}
	} else {
		for (x = end; x >= start; x--) {
			ARead[x] = func;
		}
	}
}

writefunc GetWriteHandler(uint16 a) {
	if (RWWrap && a >= 0x8000) {
		return BWriteG[a - 0x8000];
	} else {
		return BWrite[a];
	}
}

void SetWriteHandler(uint16 start, uint16 end, writefunc func) {
	int32 x;

	if (!func) {
		func = BNull;
	}

	if (RWWrap) {
		for (x = end; x >= start; x--) {
			if (x >= 0x8000) {
				BWriteG[x - 0x8000] = func;
			} else {
				BWrite[x] = func;
			}
		}
	} else {
		for (x = end; x >= start; x--) {
			BWrite[x] = func;
		}
	}
}

uint8 *RAM;

uint8 isPAL = FALSE;
uint8 isDendy = FALSE;

static int AllocBuffers(void) {
	RAM = (uint8 *)FCEU_malloc(RAM_SIZE);
	if (!RAM) {
		return FALSE;
	}
	return TRUE;
}

static void FreeBuffers(void) {
	if (RAM) {
		FCEU_free(RAM);
	}
	RAM = NULL;
}

static DECLFW(BRAML) {
	RAM[A] = V;
}

static DECLFR(ARAML) {
	return RAM[A];
}

static DECLFW(BRAMH) {
	RAM[A & RAM_MASK] = V;
}

static DECLFR(ARAMH) {
	return RAM[A & RAM_MASK];
}

void FCEUI_CloseGame(void) {
	if (!GameInfo) {
		return;
	}

	if (GameInfo->name) {
		free(GameInfo->name);
	}
	GameInfo->name = NULL;
	if (GameInfo->type != GIT_NSF) {
		FCEU_FlushGameCheats();
	}
	GameInterface(GI_CLOSE);
	ResetExState(0, 0);
	FCEU_CloseGenie();
	free(GameInfo);
	GameInfo = NULL;
}

void ResetGameLoaded(void) {
	int x;

	if (GameInfo) {
		FCEUI_CloseGame();
	}

	GameStateRestore = NULL;
	PPU_hook         = NULL;
	GameHBIRQHook    = NULL;

	for (x = 0; x < GAMEEXPSOUND_COUNT; x++) {
		if (GameExpSound[x].Kill) {
			GameExpSound[x].Kill();
		}
	}

	memset(&GameExpSound, 0, sizeof(GameExpSound));

	MapIRQHook = NULL;
	MMC5Hack   = FALSE;
	PEC586Hack = FALSE;
	isPAL &= 1;
	palette_nes_selected = PAL_NES_DEFAULT;
}

static int FCEU_GetFileType(FCEUFILE *fp) {
	uint8 header[66];

	FCEU_fread(header, 1, 66, fp);
	FCEU_fseek(fp, 0, SEEK_SET);

	if (memcmp(header, "NES\x1a", 4) == 0) {
		if ((header[15] == 0) && ((header[7] & 0xC) == 0x8)) {
			return (FTYPE_INES20);
		}
		return (FTYPE_INES);
	}

	if (memcmp(header, "UNIF", 4) == 0) {
		return (FTYPE_UNIF);
	}

	if (memcmp(header, "FDS\x1a", 4) == 0) {
		return (FTYPE_FDS);
	}

	if (memcmp(header, "\x01*NINTENDO-HVC*", 15) == 0) {
		return (FTYPE_RAWFDS);
	}

	if (memcmp(header, "NESM\x1a", 5) == 0) {
		return (FTYPE_NSF);
	}

	if (memcmp(header, "NSFE", 4) == 0) {
		return (FTYPE_NSFE);
	}

	if (memcmp(header + 3, "\x01*NINTENDO-HVC*", 15) == 0 && header[0] == 0 && header[1] == 0) {
		if (header[61] == 2 && header[63] == 0 && header[64] == 0 && header[65] == 3) {
			return (FTYPE_DOCTOR);
		}
	}

	return (FTYPE_UNKNOWN);
}

FCEUGI *FCEUI_LoadGame(const char *name, const uint8_t *databuf, size_t databufsize,
    frontend_post_load_init_cb_t frontend_post_load_init_cb) {
	FCEUFILE *fp = FCEU_fopen(name, databuf, databufsize);
	int type = FTYPE_UNKNOWN;
	int result = 0;

	if (!fp) {
		FCEU_PrintError("Error opening \"%s\"!", name);
		return FALSE;
	}

	type = FCEU_GetFileType(fp);
	if (type == FTYPE_UNKNOWN) {
		FCEU_PrintError( "Unknown file type: %d\n", type);
		FCEU_fclose(fp);
		return FALSE;
	}

	ResetGameLoaded();

	if (!(GameInfo = FCEU_malloc(sizeof(FCEUGI)))) {
		FCEU_PrintError( "Unable to allocate memory!\n");
		FCEU_fclose(fp);
		return FALSE;
	};

	GameInfo->type = GIT_CART;
	GameInfo->vidsys = GIV_USER;
	GameInfo->input[0] = -1;
	GameInfo->input[1] = -1;
	GameInfo->inputfc = -1;

	switch (type) {
	case FTYPE_INES:
	case FTYPE_INES20:
		result = iNESLoad(name, fp);
		break;
	case FTYPE_UNIF:
		result = UNIFLoad(NULL, fp);
		break;
	case FTYPE_FDS:
	case FTYPE_RAWFDS:
		result = FDSLoad(NULL, fp);
		break;
	case FTYPE_NSF:
	case FTYPE_NSFE:
		result = NSFLoad(fp);
		break;
	}

	if (!result) {
		FCEU_PrintError("An error occurred while loading the file.\n");
		FCEU_fclose(fp);

		if (GameInfo->name) {
			free(GameInfo->name);
			GameInfo->name = NULL;
		}

		free(GameInfo);
		GameInfo = NULL;

		return NULL;
	}

	FCEU_fclose(fp);

	if (frontend_post_load_init_cb) {
		(*frontend_post_load_init_cb)();
	}

	FCEU_ResetVidSys();
	if (GameInfo->type != GIT_NSF) {
		if (FSettings.GameGenie) {
			FCEU_OpenGenie();
		}
	}

	PowerNES();

	if (GameInfo->type != GIT_NSF) {
		FCEU_LoadGamePalette();
		FCEU_LoadGameCheats();
	}

	FCEU_ResetPalette();

	return (GameInfo);
}

int FCEUI_Initialize(void) {
	srand(time(0));

	if (!AllocBuffers()) {
		return FALSE;
	}

	if (!FCEU_InitVirtualVideo()) {
		return FALSE;
	}

	memset(&FSettings, 0, sizeof(FSettings));

	FSettings.UsrFirstSLine[0]     = 8;
	FSettings.UsrFirstSLine[1]     = 0;
	FSettings.UsrLastSLine[0]      = 231;
	FSettings.UsrLastSLine[1]      = 239;
	FSettings.volume[SND_MASTER]   = 100;
	FSettings.volume[SND_SQUARE1]  = 256;
	FSettings.volume[SND_SQUARE2]  = 256;
	FSettings.volume[SND_TRIANGLE] = 256;
	FSettings.volume[SND_NOISE]    = 256;
	FSettings.volume[SND_DMC]      = 256;
	FSettings.volume[SND_FDS]      = 256;
	FSettings.volume[SND_MMC5]     = 256;
	FSettings.volume[SND_N163]     = 256;
	FSettings.volume[SND_S5B]      = 256;
	FSettings.volume[SND_VRC6]     = 256;
	FSettings.volume[SND_VRC7]     = 256;

	FCEUPPU_Init();
	X6502_Init();

	return TRUE;
}

void FCEUI_Kill(void) {
	FCEU_KillVirtualVideo();
	FCEU_KillGenie();
	FreeBuffers();
}

void FCEUI_Emulate(uint8 **pXBuf, uint8 **pXDBuf, int32 **SoundBuf, int32 *SoundBufSize, int skip) {
	int ssize;

	FCEU_UpdateInput();
	if (geniestage != 1) {
		FCEU_ApplyPeriodicCheats();
	}
	FCEUPPU_Loop(skip);

	ssize = FlushEmulateSound();

	timestampbase += timestamp;

	timestamp       = 0;
	sound_timestamp = 0;

	*pXBuf  = skip ? 0 : XBuf;
	*pXDBuf = skip ? 0 : XDBuf;

	*SoundBuf     = WaveFinal;
	*SoundBufSize = ssize;
}

void ResetNES(void) {
	if (!GameInfo) {
		return;
	}
	GameInterface(GI_RESETM2);
	FCEUSND_Reset();
	FCEUPPU_Reset();
	X6502_Reset();
}

static int ram_init_seed = 0;

FCEU_MAYBE_UNUSED
static uint64 splitmix64(uint32 input) {
	uint64 z = (input + 0x9e3779b97f4a7c15);
	z        = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z        = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

static INLINE uint64 xoroshiro128plus_rotl(const uint64 x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64 xoroshiro128plus_s[2];
static void xoroshiro128plus_seed(uint32 input) {
	/* http://xoroshiro.di.unimi.it/splitmix64.c */
	uint64 x, z;

	x = input;
	z = (x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	xoroshiro128plus_s[0] = z ^ (z >> 31);

	z = (x += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

	xoroshiro128plus_s[1] = z ^ (z >> 31);
}

/* http://vigna.di.unimi.it/xorshift/xoroshiro128plus.c */
static uint64 xoroshiro128plus_next(void) {
	const uint64 s0     = xoroshiro128plus_s[0];
	uint64 s1           = xoroshiro128plus_s[1];
	const uint64 result = s0 + s1;

	s1 ^= s0;
	xoroshiro128plus_s[0] = xoroshiro128plus_rotl(s0, 55) ^ s1 ^ (s1 << 14); /* a, b */
	xoroshiro128plus_s[1] = xoroshiro128plus_rotl(s1, 36);                   /* c */

	return result;
}

void FCEU_MemoryRand(uint8 *ptr, uint32 size) {
	int x = 0;

	if (!ptr || !size) {
		return;
	}

	switch (FSettings.RamInitState) {
	case 0:
		for (x = 0; x < (int)size; x++) {
			ptr[x] = 0xFF;
		}
		break;
	case 1:
		for (x = 0; x < (int)size; x++) {
			ptr[x] = 0x00;
		}
		break;
	case 2:
		for (x = 0; x < (int)size; x++) {
			ptr[x] = (uint8)(xoroshiro128plus_next() & 0xFF);
		}
		break;
	}
}

void PowerNES(void) {
	if (!GameInfo) {
		return;
	}

	/* reseed random, unless we're in a movie */
	ram_init_seed = rand() ^ (uint32)xoroshiro128plus_next();

	/* always reseed the PRNG with the current seed, for deterministic results (for that seed) */
	xoroshiro128plus_seed(ram_init_seed);

	FCEU_CheatResetRAM();
	FCEU_CheatAddRAM(2, 0, RAM);

	FCEU_GeniePower();

	FCEU_MemoryRand(RAM, RAM_SIZE);

	SetReadHandler(0x0000, 0xFFFF, ANull);
	SetWriteHandler(0x0000, 0xFFFF, BNull);

	SetReadHandler(0, 0x7FF, ARAML);
	SetWriteHandler(0, 0x7FF, BRAML);

	SetReadHandler(0x800, 0x1FFF, ARAMH);  /* Part of a little */
	SetWriteHandler(0x800, 0x1FFF, BRAMH); /* hack for a small speed boost. */

	FCEUINPUT_Power();
	FCEUSND_Power();
	FCEUPPU_Power();

	/* Have the external game hardware "powered" after the internal NES stuff.
	    Needed for the NSF code and VS System code.
	*/
	GameInterface(GI_POWER);
	if (GameInfo->type == GIT_VSUNI) {
		FCEU_VSUniPower();
	}

	timestampbase = 0;
	X6502_Power();
	FCEU_PowerCheats();
}

void FCEU_ResetVidSys(void) {
	int w;

	if (GameInfo->vidsys == GIV_NTSC) {
		w = 0;
	} else if (GameInfo->vidsys == GIV_PAL) {
		w       = 1;
		isDendy = FALSE;
	} else {
		w = FSettings.PAL;
	}

	isPAL = w ? TRUE : FALSE;

	if (isPAL) {
		isDendy = FALSE;
	}

	ppu.normal_scanlines = isDendy ? 290 : 240;
	ppu.totalscanlines   = ppu.normal_scanlines;
	if (FSettings.PPUOverclockEnabled && ppu.overclock.postrender_scanlines) {
		ppu.totalscanlines += ppu.overclock.postrender_scanlines;
	}

	FCEUPPU_SetVideoSystem(w || isDendy);
	SetSoundVariables();
}

FCEUS FSettings;

void FCEU_DispMessage(enum retro_log_level level, unsigned duration, const char *format, ...) {
	static char msg[512] = { 0 };
	va_list ap;

	if (!format || (*format == '\0')) {
		return;
	}

	va_start(ap, format);
	vsprintf(msg, format, ap);
	va_end(ap);

	FCEUD_DispMessage(level, duration, msg);
}

void FCEU_printf(char *format, ...) {
	char temp[2048];

	va_list ap;

	va_start(ap, format);
	vsprintf(temp, format, ap);
	FCEUD_Message(temp);

	va_end(ap);
}

void FCEU_PrintDebug(char *format, ...) {
	char temp[2048];

	va_list ap;

	va_start(ap, format);
	vsprintf(temp, format, ap);
	FCEUD_PrintDebug(temp);

	va_end(ap);
}

void FCEU_PrintError(char *format, ...) {
	char temp[2048];

	va_list ap;

	va_start(ap, format);
	vsprintf(temp, format, ap);
	FCEUD_PrintError(temp);

	va_end(ap);
}

void FCEUI_SetRenderedLines(int ntscf, int ntscl, int palf, int pall) {
	FSettings.UsrFirstSLine[0] = ntscf;
	FSettings.UsrLastSLine[0]  = ntscl;
	FSettings.UsrFirstSLine[1] = palf;
	FSettings.UsrLastSLine[1]  = pall;
	if (isPAL || isDendy) {
		FSettings.FirstSLine = FSettings.UsrFirstSLine[1];
		FSettings.LastSLine  = FSettings.UsrLastSLine[1];
	} else {
		FSettings.FirstSLine = FSettings.UsrFirstSLine[0];
		FSettings.LastSLine  = FSettings.UsrLastSLine[0];
	}
}

void FCEUI_SetVidSystem(int a) {
	FSettings.PAL = a ? TRUE : FALSE;

	if (!GameInfo) {
		return;
	}

	FCEU_ResetVidSys();
	FCEU_ResetPalette();
}

int FCEUI_GetCurrentVidSystem(int *slstart, int *slend) {
	if (slstart) {
		*slstart = FSettings.FirstSLine;
	}
	if (slend) {
		*slend = FSettings.LastSLine;
	}
	return (isPAL);
}

void FCEUI_SetGameGenie(int a) {
	FSettings.GameGenie = a ? TRUE : FALSE;
}

int32 FCEUI_GetDesiredFPS(void) {
	if (isPAL || isDendy) {
		return (838977920); /* ~50.007 */
	}
	return (1008307711);    /* ~60.1 */
}
