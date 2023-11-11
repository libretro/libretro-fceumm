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

/* dec-12-19                                    */
/* 4024(w), 4025(w), 4031(r) by dink(fbneo)     */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "fds.h"
#include "fdssound.h"
#include "sound.h"
#include "general.h"
#include "state.h"
#include "file.h"
#include "fceu-memory.h"
#include "cart.h"
#include "md5.h"

/*	TODO:  Add code to put a delay in between the time a disk is inserted
 *	and the when it can be successfully read/written to.  This should
 *	prevent writes to wrong places OR add code to prevent disk ejects
 *	when the virtual motor is on(mmm...virtual motor).
 */

static DECLFR(FDSRead);
static DECLFW(FDSWrite);

static void FDSInit(void);
static void FDSClose(void);

static void FDSFix(int a);

static int32 IRQLatch, IRQCount;
static uint8 IRQa;
static uint8 IRQr;

static uint8 *FDSDISK = NULL;  /* fds disks raw data */
static uint32 FDSDISKSize = 0; /* fds disks total data size */
static uint8 *FDSRAM = NULL;
static uint32 FDSRAMSize;
static uint8 *FDSBIOS = NULL;
static uint32 FDSBIOSsize;
static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize;

/* Original disk data backup, to help in creating save states. */
static uint8 *diskdatao[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static uint8 *diskdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

static uint32 TotalSides;
static int32 DiskSeekIRQ;
static uint8 SelectDisk, InDisk;

enum FDS_DiskBlockIDs {
	DSK_INIT = 0,
	DSK_VOLUME,
	DSK_FILECNT,
	DSK_FILEHDR,
	DSK_FILEDATA
};

static uint8 DiskRegsEnabled;

/*
FDS Control ($4025)
7  bit  0
---------
IS1B MRTD
|||| ||||
|||| |||+- Drive Motor Control  
|||| |||     0: Stop motor
|||| |||     1: Turn on motor
|||| ||+-- Transfer Reset
|||| ||        Set 1 to reset transfer timing to the initial state.
|||| |+--- Read / Write mode
|||| |     (0: write; 1: read)
|||| +---- Mirroring (0: vertical; 1: horizontal)
|||+------ CRC control (set during CRC calculation of transfer)
||+------- Always set to '1'
|+-------- Read/Write Start  
|            Turn on motor.  Set to 1 when the drive becomes ready for read/write
+--------- Interrupt Transfer  
             0: Transfer without using IRQ
             1: Enable IRQ when the drive becomes ready for
*/

static uint8  mapperFDS_control;    /* 4025(w) control register */
static uint16 mapperFDS_filesize;   /* size of file being read/written */
static uint8  mapperFDS_blockID;      /* block-id of current block */
static uint16 mapperFDS_blockstart; /* start-address of current block */
static uint16 mapperFDS_blocklen;   /* length of current block */
static uint16 mapperFDS_diskaddr;   /* current address relative to blockstart */
static uint8  mapperFDS_diskaccess; /* disk needs to be accessed at least once before writing */

#define BYTES_PER_SIDE 65500

static uint8 FDSDISK_read(void) {
	uint32 offset = mapperFDS_blockstart + mapperFDS_diskaddr;
	return diskdata[InDisk][offset];
}

static void FDSDISK_write(uint8 V) {
	uint32 offset = mapperFDS_blockstart + mapperFDS_diskaddr;
	uint8 ret = diskdata[InDisk][offset];
	if (V != ret) {
		diskdata[InDisk][offset] = V;
	}
}

uint8 *FDSROM_ptr(void) {
	return (FDSDISK);
}

uint32 FDSROM_size(void) {
	return (FDSDISKSize);
}

void FDSGI(int h) {
	switch (h) {
	case GI_CLOSE: FDSClose(); break;
	case GI_POWER: FDSInit(); break;
	}
}

static void FDSStateRestore(int version) {
	uint32 x;

	setmirror(((mapperFDS_control & 8) >> 3) ^ 1);

	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			diskdata[x][b] ^= diskdatao[x][b];
		}
	}
}

static void FDSInit(void) {
	DiskRegsEnabled = 0;
	DiskSeekIRQ = 0;

	setmirror(1);
	setprg8(0xE000, 0);			/* BIOS */
	setprg32r(1, 0x6000, 0);	/* 32KB RAM */
	setchr8(0);					/* 8KB CHR RAM */

	MapIRQHook = FDSFix;
	GameStateRestore = FDSStateRestore;

	SetReadHandler(0x4030, 0x4033, FDSRead);
	SetWriteHandler(0x4020, 0x4025, FDSWrite);

	SetWriteHandler(0x6000, 0xDFFF, CartBW);
	SetReadHandler(0x6000, 0xFFFF, CartBR);

	IRQCount = IRQLatch = IRQa = 0;

	FDSSound_Reset();
	InDisk = 0;
	SelectDisk = 0;

	mapperFDS_control = 0;
	mapperFDS_filesize = 0;
	mapperFDS_blockID = 0;
	mapperFDS_blockstart = 0;
	mapperFDS_blocklen = 0;
	mapperFDS_diskaddr = 0;
	mapperFDS_diskaccess = 0;
}

static uint8 isDiskInserted(void) {
	return (InDisk != 255);
}

void FCEU_FDSEject(void) {
	InDisk = 255;
}

void FCEU_FDSInsert(int oride) {
	if (!isDiskInserted()) {
		FCEU_DispMessage(RETRO_LOG_INFO, 2000, "Disk %d of %d Side %s Inserted",
				1 + (SelectDisk >> 1), (TotalSides + 1) >> 1, (SelectDisk & 1) ? "B" : "A");
		InDisk = SelectDisk;
	} else {
		FCEU_DispMessage(RETRO_LOG_INFO, 2000, "Disk %d of %d Side %s Ejected",
				1 + (SelectDisk >> 1), (TotalSides + 1) >> 1, (SelectDisk & 1) ? "B" : "A");
		FCEU_FDSEject();
	}
}

void FCEU_FDSSelect(void) {
	if (isDiskInserted()) {
		FCEUD_DispMessage(RETRO_LOG_WARN, 2000, "Eject disk before selecting");
		return;
	}
	SelectDisk = ((SelectDisk + 1) % TotalSides) & 3;
	FCEU_DispMessage(RETRO_LOG_INFO, 2000, "Disk %d of %d Side %s Selected",
			1 + (SelectDisk >> 1), (TotalSides + 1) >> 1, (SelectDisk & 1) ? "B" : "A");
}

/* 2018/12/15 - update irq timings */
static void FDSFix(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			IRQCount = IRQLatch;
			if (!IRQr) {
				IRQa = 0;
			}
		}
	}

	if (DiskSeekIRQ > 0) {
		DiskSeekIRQ -= a;
		if (DiskSeekIRQ <= 0) {
			if (mapperFDS_control & 0x80) {
				X6502_IRQBegin(FCEU_IQEXT2);
			}
		}
	}
}

static DECLFR(FDSRead) {
	uint8 ret = X.DB;

	if (!DiskRegsEnabled) {
		return X.DB;
	}

	switch (A) {
	case 0x4030:
		ret &= ~0xC3;
		/* Cheap hack. */
		if (X.IRQlow & FCEU_IQEXT)
			ret |= 1;
		if (X.IRQlow & FCEU_IQEXT2)
			ret |= 2;
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
		{
			X6502_IRQEnd(FCEU_IQEXT);
			X6502_IRQEnd(FCEU_IQEXT2);
		}
		return ret;

	case 0x4031:
		if (isDiskInserted() && (mapperFDS_control & 0x04)) {
			uint8 data = 0xFF;
			mapperFDS_diskaccess = 1;
			if (mapperFDS_diskaddr < mapperFDS_blocklen) {
				data = FDSDISK_read();
				switch (mapperFDS_blockID) {
				case DSK_FILEHDR:
					switch (mapperFDS_diskaddr) {
					case 13: mapperFDS_filesize = data; break;
					case 14: mapperFDS_filesize |= (data << 8); break;
					}
					break;
				}
				mapperFDS_diskaddr++;
			}
			DiskSeekIRQ = 150;
			X6502_IRQEnd(FCEU_IQEXT2);
			return data;
		}
		return 0xFF;

	case 0x4032:
		ret &= ~0x07;
		if (!isDiskInserted())
			ret |= 5;
		if (!isDiskInserted() || !(mapperFDS_control & 0x01) || (mapperFDS_control & 0x02))
			ret |= 2;
		return ret;

	case 0x4033:
		return 0x80; /* battery */
	}

	return ret;
}

static DECLFW(FDSWrite) {
	if (A == 0x4026) FCEU_printf("%04x:%02x\n", A, V);
	if (!DiskRegsEnabled && (A >= 0x4024)) {
		return;
	}

	switch (A) {
	case 0x4020:
		IRQLatch &= 0xFF00;
		IRQLatch |= V;
		break;

	case 0x4021:
		IRQLatch &= 0xFF;
		IRQLatch |= V << 8;
		break;

	case 0x4022:
		/* irq repeat */
		IRQr = (V & 0x01) == 0x01;
		/* irq enabled */
		IRQa = DiskRegsEnabled ? ((V >> 1) & 0x01) : 0x00;
		if (IRQa) {
			IRQCount = IRQLatch;
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;

	case 0x4023:
		DiskRegsEnabled = V & 0x01;
		if (!DiskRegsEnabled) {
			IRQa = 0;
			X6502_IRQEnd(FCEU_IQEXT);
			X6502_IRQEnd(FCEU_IQEXT2);
		}
		break;

	case 0x4024:
		if (isDiskInserted() && (~mapperFDS_control & 0x04)) {
			if (mapperFDS_diskaccess == 0) {
				mapperFDS_diskaccess = 1;
			} else if (mapperFDS_diskaddr < mapperFDS_blocklen) {
				FDSDISK_write(V);
				switch (mapperFDS_blockID) {
				case DSK_FILEHDR:
					switch (mapperFDS_diskaddr) {
					case 13: mapperFDS_filesize = V; break;
					case 14: mapperFDS_filesize |= V << 8; break;
					}
					break;
				}
				mapperFDS_diskaddr++;
			}
			DiskSeekIRQ = 150;
			X6502_IRQEnd(FCEU_IQEXT2);
		}
		break;

	case 0x4025:
		X6502_IRQEnd(FCEU_IQEXT2);
		if (isDiskInserted()) {
			if ((V & 0x40) && (~mapperFDS_control & 0x40)) {
				mapperFDS_diskaccess = 0;
				DiskSeekIRQ = 150;
				/* blockstart  - address of block on disk
				 * diskaddr    - address relative to blockstart
				 * _block -> _blockID ?
				 */
				mapperFDS_blockstart += mapperFDS_diskaddr;
				mapperFDS_diskaddr = 0;
				mapperFDS_blockID++;
				if (mapperFDS_blockID > DSK_FILEDATA)
					mapperFDS_blockID = DSK_FILEHDR;
				switch (mapperFDS_blockID) {
				case DSK_VOLUME: mapperFDS_blocklen = 0x38; break;
				case DSK_FILECNT: mapperFDS_blocklen = 0x02; break;
				case DSK_FILEHDR: mapperFDS_blocklen = 0x10; break;
				case DSK_FILEDATA: mapperFDS_blocklen = 0x01 + mapperFDS_filesize; break;
				}
			}
			if (V & 0x02) { /* transfer reset */
				mapperFDS_blockID = DSK_INIT;
				mapperFDS_blockstart = 0;
				mapperFDS_blocklen = 0;
				mapperFDS_diskaddr = 0;
				DiskSeekIRQ = 150;
			}
			if (V & 0x40) { /* turn on motor */
				DiskSeekIRQ = 150;
			}
		}
		mapperFDS_control = V;
		setmirror(((V >> 3) & 1) ^ 1);
		break;
	}
}

struct codes_t {
	uint8 code;
	char *name;
};

static const struct codes_t list[] = {
	{ 0x01, "Nintendo" },
	{ 0x02, "Rocket Games" },
	{ 0x08, "Capcom" },
	{ 0x09, "Hot B Co." },
	{ 0x0A, "Jaleco" },
	{ 0x0B, "Coconuts Japan" },
	{ 0x0C, "Coconuts Japan/G.X.Media" },
	{ 0x13, "Electronic Arts Japan" },
	{ 0x18, "Hudson Soft Japan" },
	{ 0x19, "S.C.P." },
	{ 0x1A, "Yonoman" },
	{ 0x20, "Destination Software" },
	{ 0x22, "VR 1 Japan" },
	{ 0x25, "San-X" },
	{ 0x28, "Kemco Japan" },
	{ 0x29, "Seta" },
	{ 0x36, "Codemasters" },
	{ 0x37, "GAGA Communications" },
	{ 0x38, "Laguna" },
	{ 0x39, "Telstar Fun and Games" },
	{ 0x41, "Ubi Soft Entertainment" },
	{ 0x42, "Sunsoft" },
	{ 0x47, "Spectrum Holobyte" },
	{ 0x49, "Irem" },
	{ 0x4A, "Gakken" },
	{ 0x4D, "Malibu Games" },
	{ 0x4F, "Eidos/U.S. Gold" },
	{ 0x50, "Absolute Entertainment" },
	{ 0x51, "Acclaim" },
	{ 0x52, "Activision" },
	{ 0x53, "American Sammy Corp." },
	{ 0x54, "Take 2 Interactive" },
	{ 0x55, "Hi Tech" },
	{ 0x56, "LJN LTD." },
	{ 0x58, "Mattel" },
	{ 0x5A, "Mindscape/Red Orb Ent." },
	{ 0x5C, "Taxan" },
	{ 0x5D, "Midway" },
	{ 0x5F, "American Softworks" },
	{ 0x60, "Titus Interactive Studios" },
	{ 0x61, "Virgin Interactive" },
	{ 0x62, "Maxis" },
	{ 0x64, "LucasArts Entertainment" },
	{ 0x67, "Ocean" },
	{ 0x69, "Electronic Arts" },
	{ 0x6E, "Elite Systems Ltd." },
	{ 0x6F, "Electro Brain" },
	{ 0x70, "Infogrames" },
	{ 0x71, "Interplay" },
	{ 0x72, "JVC Musical Industries Inc" },
	{ 0x73, "Parker Brothers" },
	{ 0x75, "SCI" },
	{ 0x78, "THQ" },
	{ 0x79, "Accolade" },
	{ 0x7A, "Triffix Ent. Inc." },
	{ 0x7C, "Microprose Software" },
	{ 0x7D, "Universal Interactive Studios" },
	{ 0x7F, "Kemco" },
	{ 0x80, "Misawa" },
	{ 0x83, "LOZC" },
	{ 0x8B, "Bulletproof Software" },
	{ 0x8C, "Vic Tokai Inc." },
	{ 0x91, "Chun Soft" },
	{ 0x92, "Video System" },
	{ 0x93, "BEC" },
	{ 0x96, "Yonezawa/S'pal" },
	{ 0x97, "Kaneko" },
	{ 0x99, "Victor Interactive Software" },
	{ 0x9A, "Nichibutsu/Nihon Bussan" },
	{ 0x9B, "Tecmo" },
	{ 0x9C, "Imagineer" },
	{ 0x9F, "Nova" },
	{ 0xA0, "Telenet" },
	{ 0xA1, "Hori" },
	{ 0xA2, "Scorpion Soft " },
	{ 0xA4, "Konami" },
	{ 0xA6, "Kawada" },
	{ 0xA7, "Takara" },
	{ 0xA8, "Royal Industries" },
	{ 0xA9, "Technos Japan Corp." },
	{ 0xAA, "JVC" },
	{ 0xAC, "Toei Animation" },
	{ 0xAD, "Toho" },
	{ 0xAF, "Namco" },
	{ 0xB0, "Acclaim Japan" },
	{ 0xB1, "ASCII" },
	{ 0xB2, "Bandai" },
	{ 0xB3, "Soft Pro Inc." },
	{ 0xB4, "Enix" },
	{ 0xB6, "HAL Laboratory" },
	{ 0xB7, "SNK" },
	{ 0xB9, "Pony Canyon Hanbai" },
	{ 0xBA, "Culture Brain" },
	{ 0xBB, "Sunsoft" },
	{ 0xBC, "Toshiba EMI" },
	{ 0xBD, "Sony Imagesoft" },
	{ 0xBF, "Sammy" },
	{ 0xC0, "Taito" },
	{ 0xC1, "Sunsoft / Ask Co., Ltd." },
	{ 0xC2, "Kemco" },
	{ 0xC3, "Square Soft" },
	{ 0xC4, "Tokuma Shoten " },
	{ 0xC5, "Data East" },
	{ 0xC6, "Tonkin House" },
	{ 0xC7, "East Cube" },
	{ 0xC8, "Koei" },
	{ 0xCA, "Konami/Palcom/Ultra" },
	{ 0xCB, "Vapinc/NTVIC" },
	{ 0xCC, "Use Co.,Ltd." },
	{ 0xCD, "Meldac" },
	{ 0xCE, "FCI/Pony Canyon" },
	{ 0xCF, "Angel" },
	{ 0xD1, "Sofel" },
	/*{ 0xD2, "Quest" },*/
	{ 0xD2, "Bothtec, Inc." },
	{ 0xD3, "Sigma Enterprises" },
	{ 0xD4, "Ask Kodansa" },
	{ 0xD6, "Naxat" },
	{ 0xD7, "Copya System" },
	{ 0xD9, "Banpresto" },
	{ 0xDA, "TOMY" },
	/*{ 0xDB, "LJN Japan" },*/
	{ 0xDB, "Hiro Co., Ltd." },
	{ 0xDD, "NCS" },
	{ 0xDF, "Altron Corporation" },
	{ 0xE2, "Yutaka" },
	{ 0xE3, "Varie" },
	{ 0xE5, "Epoch" },
	{ 0xE7, "Athena" },
	{ 0xE8, "Asmik Ace Entertainment Inc." },
	{ 0xE9, "Natsume" },
	{ 0xEA, "King Records" },
	{ 0xEB, "Atlus" },
	{ 0xEC, "Epic/Sony Records" },
	{ 0xEE, "IGS" },
	{ 0xF0, "A Wave" },
	{ 0 }
 };

static const char *getManufacturer(uint8 code)
{
	int x = 0;
	char *ret = "unlicensed";

	while (list[x].code != 0) {
		if (list[x].code == code) {
			ret = list[x].name;
			break;
		}
		x++;
	} 

	return ret;
}

static void FreeFDSMemory(void) {
	if (FDSDISK)
		free(FDSDISK);
	FDSDISK = NULL;
	if (FDSBIOS)
		free(FDSBIOS);
	FDSBIOS = NULL;
	if (FDSRAM)
		free(FDSRAM);
	FDSRAM = NULL;
	if (CHRRAM)
		free(CHRRAM);
	CHRRAM = NULL;
}

static int SubLoad(FCEUFILE *fp) {
	struct md5_context md5;
	uint8 header[16];
	uint32 x;

	FCEU_fread(header, 16, 1, fp);

	if (memcmp(header, "FDS\x1a", 4)) {
		if (!(memcmp(header, "\x1*NINTENDO-HVC*", 15))) {
			long t;
			t = FCEU_fgetsize(fp);
			if (t < BYTES_PER_SIDE)
				t = BYTES_PER_SIDE;
			TotalSides = t / BYTES_PER_SIDE;
			FCEU_fseek(fp, 0, SEEK_SET);
		} else {
			return(0);
		}
	} else {
		TotalSides = header[4];
	}

	if (TotalSides > 8) TotalSides = 8;
	if (TotalSides < 1) TotalSides = 1;

	FDSDISKSize = TotalSides * BYTES_PER_SIDE;
	FDSDISK = (uint8*)FCEU_malloc(FDSDISKSize);

	if (!FDSDISK) {
		return (0);
	}

	for (x = 0; x < TotalSides; x++) {
		diskdata[x] = &FDSDISK[x * BYTES_PER_SIDE];
	}

	md5_starts(&md5);

	for (x = 0; x < TotalSides; x++) {
		FCEU_fread(diskdata[x], 1, BYTES_PER_SIDE, fp);
		md5_update(&md5, diskdata[x], BYTES_PER_SIDE);
	}
	md5_finish(&md5, GameInfo->MD5);
	return(1);
}

static void PreSave(void) {
	uint32 x;
	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			diskdata[x][b] ^= diskdatao[x][b];
		}
	}
}

static void PostSave(void) {
	uint32 x;
	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			diskdata[x][b] ^= diskdatao[x][b];
		}
	}
}

int FDSLoad(const char *name, FCEUFILE *fp) {
	FCEUFILE *zp;
	uint32 x;

	char *fn = FCEU_MakeFName(FCEUMKF_FDSROM, 0, 0);

	if (!(zp = FCEU_fopen(fn, NULL, 0))) {
		FCEU_PrintError("FDS BIOS ROM image missing!\n");
		FCEUD_DispMessage(RETRO_LOG_ERROR, 3000, "FDS BIOS image (disksys.rom) missing");
		free(fn);
		return 0;
	}

	free(fn);

	FreeFDSMemory();

	ResetCartMapping();

	FDSBIOSsize = 8192;
	FDSBIOS = (uint8*)FCEU_gmalloc(FDSBIOSsize);
	SetupCartPRGMapping(0, FDSBIOS, FDSBIOSsize, 0);

	if (FCEU_fread(FDSBIOS, 1, FDSBIOSsize, zp) != FDSBIOSsize) {
		if (FDSBIOS)
			free(FDSBIOS);
		FDSBIOS = NULL;
		FCEU_fclose(zp);
		FCEU_PrintError("Error reading FDS BIOS ROM image.\n");
		FCEUD_DispMessage(RETRO_LOG_ERROR, 3000, "Error reading FDS BIOS image (disksys.rom)");
		return 0;
	}

	FCEU_fclose(zp);

	FCEU_fseek(fp, 0, SEEK_SET);

	if (!SubLoad(fp)) {
		if (FDSBIOS)
			free(FDSBIOS);
		FDSBIOS = NULL;
		return(0);
	}

	for (x = 0; x < TotalSides; x++) {
		diskdatao[x] = (uint8*)FCEU_malloc(BYTES_PER_SIDE);
		memcpy(diskdatao[x], diskdata[x], BYTES_PER_SIDE);
	}
	
	GameInfo->type = GIT_FDS;
	GameInterface = FDSGI;

	SelectDisk = 0;
	FCEU_FDSEject();

	ResetExState(PreSave, PostSave);
	FDSSound_AddSaveState();

	for (x = 0; x < TotalSides; x++) {
		char temp[5];
		sprintf(temp, "DDT%d", x);
		AddExState(diskdata[x], BYTES_PER_SIDE, 0, temp);
	}

	AddExState(&DiskRegsEnabled, 1, 0, "DREG");
	AddExState(&IRQCount, 4 | FCEUSTATE_RLSB, 1, "IRQC");
	AddExState(&IRQLatch, 4 | FCEUSTATE_RLSB, 1, "IQL1");
	AddExState(&IRQa, 1, 0, "IRQA");
	AddExState(&DiskSeekIRQ, 4 | FCEUSTATE_RLSB, 1, "DSIR");
	AddExState(&SelectDisk, 1, 0, "SELD");
	AddExState(&InDisk, 1, 0, "INDI");

	AddExState(&mapperFDS_control, 1, 0, "CTRG");
	AddExState(&mapperFDS_filesize, 2 | FCEUSTATE_RLSB, 1, "FLSZ");
	AddExState(&mapperFDS_blockID, 1, 0, "BLCK");
	AddExState(&mapperFDS_blockstart, 2 | FCEUSTATE_RLSB, 1, "BLKS");
	AddExState(&mapperFDS_blocklen, 2 | FCEUSTATE_RLSB, 1, "BLKL");
	AddExState(&mapperFDS_diskaddr, 2 | FCEUSTATE_RLSB, 1, "DADR");
	AddExState(&mapperFDS_diskaccess, 1, 0, "DACC");

	CHRRAMSize = 8192;
	CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CHRR");

	FDSRAMSize = 32768;
	FDSRAM = (uint8*)FCEU_gmalloc(FDSRAMSize);
	SetupCartPRGMapping(1, FDSRAM, FDSRAMSize, 1);
	AddExState(FDSRAM, FDSRAMSize, 0, "FDSR");

	SetupCartMirroring(0, 0, 0);

	FCEU_printf(" Code         : %02x\n", diskdata[0][0xf]);
	FCEU_printf(" Manufacturer : %s\n", getManufacturer(diskdata[0][0xf]));
	FCEU_printf(" # of Sides   : %d\n", TotalSides);
	FCEU_printf(" ROM MD5      : 0x%s\n", md5_asciistr(GameInfo->MD5));

	FCEUI_SetVidSystem(0);

	return 1;
}

void FDSClose(void) {
	uint32 x;

	for (x = 0; x < TotalSides; x++) {
		if (diskdatao[x]) {
			free(diskdatao[x]);
			diskdatao[x] = 0;
		}
	}

	FreeFDSMemory();
}
