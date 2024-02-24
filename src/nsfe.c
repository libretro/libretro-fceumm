/* FCEUmm - Multi-system Emulator
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <string/stdstring.h>

#include "fceu.h"
#include "x6502.h"
#include "sound.h"
#include "cart.h"
#include "nsf.h"
#include "nsfe.h"
#include "fceu-endian.h"
#include "file.h"
#include "general.h"

typedef struct {
	uint8 LoadAddressLow;
	uint8 LoadAddressHigh;
	uint8 InitAddressLow;
	uint8 InitAddressHigh;
	uint8 PlayAddressLow;
	uint8 PlayAddressHigh;
	uint8 VideoSystem;
	uint8 SoundChip;
	uint8 TotalSongs;
	uint8 StartingSong;
} NSFE_INFO;

typedef struct {
	uint32 info;
	char ID[4];
} NSFE_HEADER;

static NSFE_HEADER nchead;

static void GetString(FCEUFILE *fp, uint32 *chunk_size, uint8 *str) {
	unsigned count = 0;

	while (*chunk_size) {
		uint8 c = FCEU_fgetc(fp);

		(*chunk_size)--;
		if (!c) {
			break;
		}
		if (c < 0x20) {
			c = 0x20;
		}
		*str++ = c;
		count++;
	}
}

static void FreeNFSE(void) {
	if (NSFInfo->NSFDATA) {
		free(NSFInfo->NSFDATA);
		NSFInfo->NSFDATA = NULL;
	}
}

static uint8 nsfe_INFO(FCEUFILE *fp) {
	NSFE_INFO nsfe_info;

	if (NSFInfo->TotalSongs) {
		FCEU_PrintError("NSFE chunk \"%.4s\" is duplicate.\n", (char *)nchead.ID);
		return FALSE;
	}

	if (nchead.info < 8) {
		FCEU_PrintError("NSFE chunk \"%.4s\" size(%u) is invalid.\n", (char *)nchead.ID, nchead.info);
		return FALSE;
	}

	if (!FCEU_fread(&nsfe_info, 1, 10, fp)) {
		return FALSE;
	}

	NSFInfo->TotalSongs = 1;
	NSFInfo->StartingSong = 0;

	NSFInfo->LoadAddr = nsfe_info.LoadAddressLow | nsfe_info.LoadAddressHigh << 8;
	NSFInfo->InitAddr = nsfe_info.InitAddressLow | nsfe_info.InitAddressHigh << 8;
	NSFInfo->PlayAddr = nsfe_info.PlayAddressLow | nsfe_info.PlayAddressHigh << 8;
	NSFInfo->VideoSystem = nsfe_info.VideoSystem;
	NSFInfo->SoundChip = nsfe_info.SoundChip;

	nchead.info -= 8;

	if (nchead.info) {
		NSFInfo->TotalSongs = nsfe_info.TotalSongs;
		if (!NSFInfo->TotalSongs) {
			NSFInfo->TotalSongs = 255;
		}
		nchead.info--;
	}

	if (nchead.info) {
		NSFInfo->StartingSong = nsfe_info.StartingSong;
		nchead.info--;
	}

	return TRUE;
}

static uint8 nsfe_DATA(FCEUFILE *fp) {
	if (!NSFInfo->TotalSongs) {
		FCEU_PrintError("NSFE chunk \"%.4s\" is out of order.\n", (char *)nchead.ID);
		return FALSE;
	}

	if (NSFInfo->NSFDATA) {
		FCEU_PrintError("NSFE chunk \"%.4s\" is duplicate.\n", (char *)nchead.ID);
		return FALSE;
	}

	NSFInfo->NSFSize = nchead.info;
	NSFInfo->NSFMaxBank = uppow2((NSFInfo->NSFSize + (NSFInfo->LoadAddr & 0xfff) + 0xfff) / 0x1000) - 1;

	NSFInfo->NSFDATA = (uint8 *)FCEU_malloc((NSFInfo->NSFMaxBank + 1) * 4096);
	if (!NSFInfo->NSFDATA) {
		return FALSE;
	}
	memset(NSFInfo->NSFDATA, 0, (NSFInfo->NSFMaxBank + 1) * 4096);

	if (!FCEU_fread(NSFInfo->NSFDATA + (NSFInfo->LoadAddr & 0xfff), 1, NSFInfo->NSFSize, fp)) {
		return FALSE;
	}

	nchead.info -= NSFInfo->NSFSize;

	return TRUE;
}

static uint8 nsfe_BANK(FCEUFILE *fp) {
	uint64 tr = (uint64)MIN((uint64)nchead.info, (uint64)8);

	if (!FCEU_fread(NSFInfo->BankSwitch, 1, tr, fp) != 0) {
		return FALSE;
	}
	nchead.info -= 8;

	return TRUE;
}

static uint8 nsfe_NEND(FCEUFILE *fp) {
	if (nchead.info != 0) {
		/* chunk size here can be anything */
	} else if (!NSFInfo->NSFDATA) {
		FCEU_PrintError("NEND reached without preceding DATA chunk.\n");
		return FALSE;
	}

	return TRUE;
}

static uint8 nsfe_tlbl(FCEUFILE *fp) {
	unsigned ws;

	if (!NSFInfo->TotalSongs) {
		FCEU_PrintError("NSFE chunk \"%.4s\" is out of order.\n", (char *)nchead.ID);
		return FALSE;
	}

	for (ws = 0; ws < NSFInfo->TotalSongs && nchead.info > 0; ws++) {
		GetString(fp, &nchead.info, (uint8 *)NSFInfo->SongNames[ws]);
	}

	return TRUE;
}

static uint8 nsfe_auth(FCEUFILE *fp) {
	unsigned count;

	for (count = 0; count < 4 && nchead.info > 0; count++) {
		switch (count) {
		case 0:
			GetString(fp, &nchead.info, (uint8 *)&NSFInfo->SongName);
			break;
		case 1:
			GetString(fp, &nchead.info, (uint8 *)&NSFInfo->Artist);
			break;
		case 2:
			GetString(fp, &nchead.info, (uint8 *)&NSFInfo->Copyright);
			break;
		case 3:
			GetString(fp, &nchead.info, (uint8 *)&NSFInfo->Dumper);
			break;
		}
	}
	return TRUE;
}

static uint8 nsfe_UNUSED(FCEUFILE *fp) {
	/* unused chunk, just move our rom pointer at the end of chunk */
	if (nchead.info) {
		if (FCEU_fseek(fp, nchead.info, SEEK_CUR) != 0) {
			return FALSE;
		}
	}
	nchead.info = 0;
	return TRUE;
}

int LoadNSFE(FCEUFILE *fp) {
	if (FCEU_fseek(fp, 4, SEEK_CUR) != 0) {
		return FALSE;
	}

	for (;;) {
		if (!FCEU_fread(&nchead, 1, 8, fp)) {
			FreeNFSE();
			return FALSE;
		}
		/* FCEU_printf(" Chunk: \"%.4s\" %d\n", nchead.ID, nchead.info); */
		if (!memcmp(nchead.ID, "INFO", 4)) {
			if (!nsfe_INFO(fp)) {
				FreeNFSE();
				return FALSE;
			}
		} else if (!memcmp(nchead.ID, "BANK", 4)) {
			if (!nsfe_BANK(fp)) {
				FreeNFSE();
				return FALSE;
			}
		} else if (!memcmp(nchead.ID, "auth", 4)) {
			if (!nsfe_auth(fp)) {
				FreeNFSE();
				return FALSE;
			}
		} else if (!memcmp(nchead.ID, "tlbl", 4)) {
			if (!nsfe_tlbl(fp)) {
				FreeNFSE();
				return FALSE;
			}
		} else if (!memcmp(nchead.ID, "DATA", 4)) {
			if (!nsfe_DATA(fp)) {
				FreeNFSE();
				return FALSE;
			}
		} else if (!memcmp(nchead.ID, "NEND", 4)) {
			if (!nsfe_NEND(fp)) {
				FreeNFSE();
				return FALSE;
			}
			/* ignore remaining data from this point */
			return TRUE;
		} else {
			if (!nsfe_UNUSED(fp)) {
				FreeNFSE();
				return FALSE;
			}
		}
		if (nchead.info) {
			/* this should not happen */
			if (FCEU_fseek(fp, nchead.info, SEEK_CUR) != 0) {
				FreeNFSE();
				return FALSE;
			}
			nchead.info = 0;
		}
	}
	FreeNFSE();
	return FALSE;
}