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

#include <string/stdstring.h>
#include <file/file_path.h>
#include <streams/file_stream.h>

#include "fceu-types.h"
#include "fceu.h"
#include "cart.h"
#include "fceu-memory.h"
#include "general.h"
#include "gamegenie.h"

int RWWrap = 0;
int geniestage = 0;
readfunc *AReadG = NULL;
writefunc *BWriteG = NULL;

static uint8 *VPageG[8];
static int modcon;
static uint8 genieval[3];
static uint8 geniech[3];
static uint32 genieaddr[3];
static uint8 *GENIEROM = 0;
static readfunc GenieBackup[3];

extern uint8 **VPageR;

static void FlushGenieRW(void);

/* Called when a game(file) is opened successfully. */
void FCEU_OpenGenie(void) {
	RFILE *fp = NULL;
	int x;

	if (!GENIEROM) {
		char *fn;

		if (!(GENIEROM = (uint8 *)FCEU_malloc(4096 + 1024))) {
			return;
		}

		fn = FCEU_MakeFName(FCEUMKF_GGROM, 0, 0);

		if (!string_is_empty(fn) && path_is_valid(fn)) {
			fp = filestream_open(fn, RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);
		}

		free(fn);
		fn = NULL;

		if (!fp) {
			FCEU_PrintError("Error opening Game Genie ROM image!\n");
			FCEUD_DispMessage(RETRO_LOG_WARN, 3000, "Game Genie ROM image (gamegenie.nes) missing");
			free(GENIEROM);
			GENIEROM = 0;
			return;
		}
		if (filestream_read(fp, GENIEROM, 16) != 16) {
grerr:
			FCEU_PrintError("Error reading from Game Genie ROM image!\n");
			FCEUD_DispMessage(RETRO_LOG_WARN, 3000, "Failed to read Game Genie ROM image (gamegenie.nes)");
			free(GENIEROM);
			GENIEROM = 0;
			filestream_close(fp);
			return;
		}
		if (GENIEROM[0] == 0x4E) { /* iNES ROM image */
			if (filestream_read(fp, GENIEROM, 4096) != 4096) {
				goto grerr;
			}
			if (filestream_seek(fp, 16384 - 4096, RETRO_VFS_SEEK_POSITION_CURRENT)) {
				goto grerr;
			}
			if (filestream_read(fp, GENIEROM + 4096, 256) != 256) {
				goto grerr;
			}
		} else {
			if (filestream_read(fp, GENIEROM + 16, 4352 - 16) != (4352 - 16)) {
				goto grerr;
			}
		}
		filestream_close(fp);

		/* Workaround for the FCE Ultra CHR page size only being 1KB */
		for (x = 0; x < 4; x++) {
			memcpy(GENIEROM + 4096 + (x << 8), GENIEROM + 4096, 256);
		}
	}

	geniestage = 1;
}

/* Called when a game is closed. */
void FCEU_CloseGenie(void) {
	/* No good reason to free() the Game Genie ROM image data. */
	geniestage = 0;
	FlushGenieRW();
	VPageR = VPage;
}

void FCEU_KillGenie(void) {
	if (GENIEROM) {
		free(GENIEROM);
		GENIEROM = 0;
	}
}

static int AllocGenieRW(void) {
	if (!AReadG) {
		if (!(AReadG = (readfunc *)FCEU_malloc(0x8000 * sizeof(readfunc)))) {
			return 0;
		}
	} else {
		memset(AReadG, 0, 0x8000 * sizeof(readfunc));
	}

	if (!BWriteG) {
		if (!(BWriteG = (writefunc *)FCEU_malloc(0x8000 * sizeof(writefunc)))) {
			return 0;
		}
	} else {
		memset(BWriteG, 0, 0x8000 * sizeof(writefunc));
	}

	RWWrap = 1;
	return 1;
}

static void FlushGenieRW(void) {
	int32 x;

	if (RWWrap) {
		for (x = 0; x < 0x8000; x++) {
			ARead[x + 0x8000]  = AReadG[x];
			BWrite[x + 0x8000] = BWriteG[x];
		}
		free(AReadG);
		free(BWriteG);
		AReadG  = NULL;
		BWriteG = NULL;
	}
	RWWrap = 0;
}

static DECLFR(GenieFix1) {
	uint8 r = GenieBackup[0](A);

	if ((modcon >> 1) & 1)	{ /* No check */
		return genieval[0];
	}

	if (r == geniech[0]) {
		return genieval[0];
	}

	return r;
}

static DECLFR(GenieFix2) {
	uint8 r = GenieBackup[1](A);

	if ((modcon >> 2) & 1)	{ /* No check */
		return genieval[1];
	}

	if (r == geniech[1]) {
		return genieval[1];
	}

	return r;
}

static DECLFR(GenieFix3) {
	uint8 r = GenieBackup[2](A);

	if ((modcon >> 3) & 1)	{ /* No check */
		return genieval[2];
	}

	if (r == geniech[2]) {
		return genieval[2];
	}

	return r;
}

static void FixGenieMap(void) {
	int x;

	geniestage = 2;

	for (x = 0; x < 8; x++) {
		VPage[x] = VPageG[x];
	}

	VPageR = VPage;
	FlushGenieRW();
	for (x = 0; x < 3; x++) {
		if ((modcon >> (4 + x)) & 1) {
			readfunc tmp[3] = { GenieFix1, GenieFix2, GenieFix3 };
			GenieBackup[x] = GetReadHandler(genieaddr[x]);
			SetReadHandler(genieaddr[x], genieaddr[x], tmp[x]);
		}
	}
}

static DECLFR(GenieRead) {
	return GENIEROM[A & 4095];
}

static DECLFW(GenieWrite) {
	switch (A) {
	case 0x800c:
	case 0x8008:
	case 0x8004:
		genieval[((A - 4) & 0xF) >> 2] = V;
		break;

	case 0x800b:
	case 0x8007:
	case 0x8003:
		geniech[((A - 3) & 0xF) >> 2] = V;
		break;

	case 0x800a:
	case 0x8006:
	case 0x8002:
		genieaddr[((A - 2) & 0xF) >> 2] &= 0xFF00;
		genieaddr[((A - 2) & 0xF) >> 2] |= V;
		break;

	case 0x8009:
	case 0x8005:
	case 0x8001:
		genieaddr[((A - 1) & 0xF) >> 2] &= 0xFF;
		genieaddr[((A - 1) & 0xF) >> 2] |= (V | 0x80) << 8;
		break;

	case 0x8000:
		if (!V) {
			FixGenieMap();
		} else {
			modcon = V ^ 0xFF;
			if (V == 0x71) {
				modcon = 0;
			}
		}
		break;
	}
}

void FCEU_GeniePower(void) {
	uint32 x;

	if (!geniestage) {
		return;
	}

	geniestage = 1;
	for (x = 0; x < 3; x++) {
		genieval[x] = 0xFF;
		geniech[x] = 0xFF;
		genieaddr[x] = 0xFFFF;
	}
	modcon = 0;

	SetWriteHandler(0x8000, 0xFFFF, GenieWrite);
	SetReadHandler(0x8000, 0xFFFF, GenieRead);

	for (x = 0; x < 8; x++) {
		VPage[x] = GENIEROM + 4096 - 0x400 * x;
	}

	if (AllocGenieRW()) {
		VPageR = VPageG;
	} else {
		geniestage = 2;
	}
}
