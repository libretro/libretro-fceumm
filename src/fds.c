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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
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

static void FDSPower(void);
static void FDSReset(void);
static void FDSClose(void);

static void FDSFix(int a);

static int32 IRQLatch, IRQCount;
static uint8 IRQa;
static uint8 IRQr;

static uint8 *CHRRAM = NULL;
static uint32 CHRRAMSize;

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

static uint8 DiskIOEnabled;

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
|            Turn on motor.  Set to 1 when the drive becomes ready for
read/write
+--------- Interrupt Transfer
             0: Transfer without using IRQ
             1: Enable IRQ when the drive becomes ready for
*/

static uint8 mapperFDS_control;     /* 4025(w) control register */
static uint16 mapperFDS_filesize;   /* size of file being read/written */
static uint8 mapperFDS_blockID;     /* block-id of current block */
static uint16 mapperFDS_blockstart; /* start-address of current block */
static uint16 mapperFDS_blocklen;   /* length of current block */
static uint16 mapperFDS_diskaddr;   /* current address relative to blockstart */
static uint8 mapperFDS_diskaccess;  /* disk needs to be accessed at least once
                                       before writing */

#define BYTES_PER_SIDE 65500

static uint8 disk_read(uint32 A) {
	return ROM.disk.data[(BYTES_PER_SIDE * InDisk) + A];
}

static void disk_write(uint32 A, uint8 V) {
	ROM.disk.data[(BYTES_PER_SIDE * InDisk) + A] = V;
}

static void FDSGI(int h) {
	switch (h) {
	case GI_CLOSE:   FDSClose(); break;
	case GI_RESETM2: FDSReset(); break;
	case GI_POWER:   FDSPower(); break;
	}
}

static void FDSStateRestore(int version) {
	uint32 x;

	setmirror(((mapperFDS_control & 8) >> 3) ^ 1);

	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			ROM.disk.data[(BYTES_PER_SIDE * x) + b] ^=
			    ROM.disko.data[(BYTES_PER_SIDE * x) + b];
		}
	}
}

#define BOOT_DELAY (NTSC_CLOCK_SPEED * 6)
static int32 disk_insert_at_boot_delay = BOOT_DELAY;

static void FDSReset(void) {
	disk_insert_at_boot_delay = BOOT_DELAY;
}

static void FDSPower(void) {
	DiskIOEnabled = 0;
	DiskSeekIRQ = 0;

	setmirror(1);
	setprg8(0xE000, 0);      /* BIOS */
	setprg32r(1, 0x6000, 0); /* 32KB RAM */
	setchr8(0);              /* 8KB CHR RAM */

	MapIRQHook = FDSFix;
	GameStateRestore = FDSStateRestore;

	SetReadHandler(0x4020, 0x5FFF, FDSRead);
	SetWriteHandler(0x4020, 0x5FFF, FDSWrite);

	SetWriteHandler(0x6000, 0xDFFF, CartBW);
	SetReadHandler(0x6000, 0xFFFF, CartBR);

	FDSSoundRegReset();
	FDSSound_SC();

	IRQCount = IRQLatch = IRQa = 0;

	InDisk = 255;
	SelectDisk = 0;
	disk_insert_at_boot_delay = BOOT_DELAY;

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
		    1 + (SelectDisk >> 1), (TotalSides + 1) >> 1,
		    (SelectDisk & 1) ? "B" : "A");
		InDisk = SelectDisk;
	} else {
		FCEU_DispMessage(RETRO_LOG_INFO, 2000, "Disk %d of %d Side %s Ejected",
		    1 + (SelectDisk >> 1), (TotalSides + 1) >> 1,
		    (SelectDisk & 1) ? "B" : "A");
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
	    1 + (SelectDisk >> 1), (TotalSides + 1) >> 1,
	    (SelectDisk & 1) ? "B" : "A");
}

/* 2018/12/15 - update irq timings */
static void FDSFix(int a) {
	if (!isDiskInserted() && (disk_insert_at_boot_delay > 0)) {
		disk_insert_at_boot_delay -= a;
		if (disk_insert_at_boot_delay <= 0) {
			disk_insert_at_boot_delay = 0;
			FCEU_FDSInsert(0);
		}
	} else {
		disk_insert_at_boot_delay = 0;
	}

	if (IRQa && (IRQCount > 0)) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
			if (IRQr) {
				IRQCount = IRQLatch;
			} else {
				IRQa = 0; /* does not clear latch, fix Druid */
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
	uint8 ret = 0;

	if (!DiskIOEnabled) {
		return ret;
	}

	if ((A >= 0x4040) && (A <= 0x407F)) {
		return FDSWaveRead(A);
	}

	switch (A) {
	case 0x4030:
		ret = 0x80;

		/* Cheap hack. */
		if (cpu.IRQlow & FCEU_IQEXT) {
			ret |= 1;
		}
		if (cpu.IRQlow & FCEU_IQEXT2) {
			ret |= 2;
		}
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
		{
			X6502_IRQEnd(FCEU_IQEXT);
			X6502_IRQEnd(FCEU_IQEXT2);
		}
		return ret;

	case 0x4031:
		ret = 0xFF;
		if (isDiskInserted() && (mapperFDS_control & 0x04)) {
			mapperFDS_diskaccess = 1;
			if (mapperFDS_diskaddr < mapperFDS_blocklen) {
				ret = disk_read(mapperFDS_blockstart + mapperFDS_diskaddr);
				switch (mapperFDS_blockID) {
				case DSK_FILEHDR:
					switch (mapperFDS_diskaddr) {
					case 13: mapperFDS_filesize = ret; break;
					case 14: mapperFDS_filesize |= (ret << 8); break;
					default: break;
					}
					break;
				}
				mapperFDS_diskaddr++;
			}
			DiskSeekIRQ = 150;
			X6502_IRQEnd(FCEU_IQEXT2);
		}
		return ret;

	case 0x4032:
		ret = 0x40;
		if (!isDiskInserted()) {
			ret |= 5;
		}
		if (!isDiskInserted() || !(mapperFDS_control & 0x01) ||
		    (mapperFDS_control & 0x02)) {
			ret |= 2;
		}
		return ret;

	case 0x4033:
		return 0x80; /* battery */

	case 0x4090:
		return FDSEnvVolumeRead(A);

	case 0x4092:
		return FDSEnvModRead(A);
	}

	return cpu.openbus;
}

static DECLFW(FDSWrite) {
	if (!DiskIOEnabled && (A >= 0x4024)) {
		return;
	}

	if ((A >= 0x4040) && (A <= 0x407F)) {
		FDSWaveWrite(A, V);
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
		IRQr = (V & 0x01);
		/* irq enabled */
		IRQa = DiskIOEnabled ? ((V >> 1) & 0x01) : 0x00;
		if (IRQa) {
			IRQCount = IRQLatch;
		} else {
			X6502_IRQEnd(FCEU_IQEXT);
		}
		break;

	case 0x4023:
		DiskIOEnabled = V & 0x01;
		if (!DiskIOEnabled) {
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
				disk_write(mapperFDS_blockstart + mapperFDS_diskaddr, V);
				switch (mapperFDS_blockID) {
				case DSK_FILEHDR:
					switch (mapperFDS_diskaddr) {
					case 13: mapperFDS_filesize = V; break;
					case 14: mapperFDS_filesize |= V << 8; break;
					default: break;
					}
					break;
				}
				mapperFDS_diskaddr++;
			}
		}
		break;

	case 0x4025:
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
				if (mapperFDS_blockID > DSK_FILEDATA) {
					mapperFDS_blockID = DSK_FILEHDR;
				}

				switch (mapperFDS_blockID) {
				case DSK_VOLUME:   mapperFDS_blocklen = 0x38; break;
				case DSK_FILECNT:  mapperFDS_blocklen = 0x02; break;
				case DSK_FILEHDR:  mapperFDS_blocklen = 0x10; break;
				case DSK_FILEDATA: mapperFDS_blocklen = mapperFDS_filesize + 1; break;
				default: break;
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
		X6502_IRQEnd(FCEU_IQEXT2);
		mapperFDS_control = V;
		setmirror(((V >> 3) & 1) ^ 1);
		break;

	case 0x4080:
		FDSSReg0Write(A, V);
		break;

	case 0x4082:
		FDSSReg1Write(A, V);
		break;

	case 0x4083:
		FDSSReg2Write(A, V);
		break;

	case 0x4084:
		FDSSReg3Write(A, V);
		break;

	case 0x4085:
		FDSSReg4Write(A, V);
		break;

	case 0x4086:
		FDSSReg5Write(A, V);
		break;

	case 0x4087:
		FDSSReg6Write(A, V);
		break;

	case 0x4088:
		FDSSReg7Write(A, V);
		break;

	case 0x4089:
		FDSSReg8Write(A, V);
		break;

	case 0x408A:
		FDSSReg9Write(A, V);
		break;
	}
}

struct codes_t {
	uint8 code;
	char name[50];
} list[] = {
	{ 0x01, "Nintendo" },
	/* { 0x02, "Rocket Games" }, */
	{ 0x07, "Nomora Securities (unverified)" },
	{ 0x08, "Capcom" },
	{ 0x09, "Hot B Co." },
	{ 0x0A, "Jaleco" },
	{ 0x0B, "Coconuts Japan" },
	{ 0x0C, "Coconuts Japan/G.X.Media" },
	{ 0x13, "Electronic Arts Japan" },
	{ 0x18, "Hudson Soft Japan" },
	/* { 0x19, "S.C.P." }, */
	/* { 0x1A, "Yonoman" }, */
	/* { 0x20, "Destination Software" }, */
	{ 0x21, "Tokai Engineering" },
	/* { 0x22, "VR 1 Japan" }, */
	/* { 0x25, "San-X" }, */
	{ 0x28, "Kemco Japan" },
	{ 0x29, "SETA (Japan)" },
	{ 0x2B, "Tamtex" },
	{ 0x35, "Hector Playing Interface (Hect)" },
	/*{ 0x36, "Codemasters" },*/
	/*{ 0x37, "GAGA Communications" },*/
	/*{ 0x38, "Laguna" },*/
	/*{ 0x39, "Telstar Fun and Games" },*/
	{ 0x3D, "Loriciel" },
	{ 0x3E, "Gremlin" },
	{ 0x40, "Seika Corporation" },
	{ 0x41, "Ubi Soft Entertainment" },
	/*{ 0x42, "Sunsoft" },*/
	{ 0x46, "System 3" },
	/*{ 0x47, "Spectrum Holobyte" },*/
	{ 0x49, "Irem" },
	{ 0x4A, "Gakken" },
	/*{ 0x4D, "Malibu Games" },*/
	/*{ 0x4F, "Eidos/U.S. Gold" },*/
	{ 0x50, "Absolute Entertainment" },
	{ 0x51, "Acclaim" },
	{ 0x52, "Activision" },
	{ 0x53, "American Sammy Corp." },
	/*{ 0x54, "Take 2 Interactive" },*/
	{ 0x54, "Gametek" },
	{ 0x55, "Hi Tech Expressions" },
	{ 0x56, "LJN LTD." },
	{ 0x57, "Matchbox Toys" },
	{ 0x58, "Mattel" },
	{ 0x59, "Mitton Bradley" },
	{ 0x5A, "Mindscape/Software Toolworks" },
	{ 0x5B, "SETA (NA)" },
	{ 0x5C, "Taxan" },
	{ 0x5D, "Tradewest" },
	{ 0x5E, "INTV Corporation" },
	/*{ 0x5F, "American Softworks" },*/
	{ 0x60, "Titus Interactive Studios" },
	{ 0x61, "Virgin Interactive" },
	/*{ 0x62, "Maxis" },*/
	/*{ 0x64, "LucasArts Entertainment" },*/
	{ 0x67, "Ocean" },
	{ 0x69, "Electronic Arts (NA)" },
	{ 0x6B, "Bream Software" },
	{ 0x6E, "Elite Systems Ltd." },
	{ 0x6F, "Electro Brain" },
	{ 0x70, "Infogrames" },
	/*{ 0x71, "Interplay" },*/
	{ 0x72, "JVC Musical Industries Inc" },
	{ 0x73, "Parker Brothers" },
	{ 0x75, "SCI" },
	{ 0x78, "THQ" },
	{ 0x79, "Accolade" },
	{ 0x7A, "Triffix Ent. Inc." },
	{ 0x7C, "Microprose Software" },
	/*{ 0x7D, "Universal Interactive Studios" },*/
	{ 0x7F, "Kemco" },
	{ 0x80, "Misawa" },
	{ 0x83, "G. Amusements Co." },
	{ 0x85, "G.O 1" },
	{ 0x86, "Tokuma Shoten Intermedia" },
	{ 0x89, "Nihon Maicom Kaihatsu (NMK)" },
	{ 0x8B, "Bulletproof Software" },
	{ 0x8C, "Vic Tokai Inc." },
	{ 0x8D, "Sanritsu" },
	{ 0x8E, "Character Soft" },
	{ 0x8F, "I'Max" },
	/*{ 0x91, "Chun Soft" },*/
	/*{ 0x92, "Video System" },*/
	/*{ 0x93, "BEC" },*/
	{ 0x94, "Toaplan" },
	{ 0x95, "Varie" },
	{ 0x96, "Yonezawa/S'pal" },
	/*{ 0x97, "Kaneko" },*/
	{ 0x99, "Victor Interactive Software" },
	{ 0x9A, "Nichibutsu/Nihon Bussan" },
	{ 0x9B, "Tecmo" },
	{ 0x9C, "Imagineer" },
	{ 0x9E, "Face" },
	/*{ 0x9F, "Nova" },*/
	/*{ 0xA0, "Telenet" },*/
	/*{ 0xA1, "Hori" },*/
	{ 0xA2, "Scorpion Soft " },
	{ 0xA3, "Broderbund" },
	{ 0xA4, "Konami" },
	{ 0xA5, "K. Amusement Leasing Co. (KAC)" },
	{ 0xA6, "Kawada" },
	{ 0xA7, "Takara" },
	{ 0xA8, "Royal Industries" },
	{ 0xA9, "Tecnos Japan Corp." },
	{ 0xAA, "Victor Musical Industries" },
	{ 0xAB, "	Hi-Score Media Work" },
	{ 0xAC, "Toei Animation" },
	{ 0xAD, "Toho" },
	{ 0xAE, "TSS" },
	{ 0xAF, "Namco" },
	{ 0xB0, "Acclaim Japan" },
	{ 0xB1, "ASCII" },
	{ 0xB2, "Bandai" },
	{ 0xB3, "Soft Pro Inc." },
	{ 0xB4, "Enix" },
	{ 0xB5, "dB-SOFT" },
	{ 0xB6, "HAL Laboratory" },
	{ 0xB7, "SNK" },
	{ 0xB9, "Pony Canyon Hanbai" },
	{ 0xBA, "Culture Brain" },
	{ 0xBB, "Sunsoft" },
	{ 0xBC, "Toshiba EMI" },
	/*{ 0xBD, "Sony Imagesoft" },*/
	{ 0xBF, "Sammy" },
	{ 0xC0, "Taito" },
	{ 0xC1, "Sunsoft / Ask Co., Ltd." },
	{ 0xC2, "Kemco" },
	{ 0xC3, "Square / Disk Original Group (DOG)" },
	{ 0xC4, "Tokuma Shoten " },
	{ 0xC5, "Data East" },
	{ 0xC6, "Tonkin House" },
	{ 0xC7, "East Cube" },
	{ 0xC8, "Koei" },
	{ 0xC9, "UPL" },
	{ 0xCA, "Konami/Palcom/Ultra" },
	{ 0xCB, "Vapinc/NTVIC" },
	{ 0xCC, "Use Co.,Ltd." },
	{ 0xCD, "Meldac" },
	{ 0xCE, "FCI/Pony Canyon" },
	{ 0xCF, "Angel" },
	{ 0xD0, "Disco" },
	{ 0xD1, "Sofel" },
	{ 0xD2, "Bothtec, Inc." },
	{ 0xD3, "Sigma Enterprises" },
	{ 0xD4, "Ask Kodansa" },
	{ 0xD5, "Kyugo Trading Co." },
	{ 0xD6, "Naxat Soft / Kaga Tech" },
	/*{ 0xD7, "Copya System" },*/
	{ 0xD9, "Banpresto" },
	{ 0xDA, "TOMY" },
	{ 0xDB, "Hiro Co., Ltd." },
	{ 0xDD, "Nippon Computer Systems (NCS) / Masaya Games" },
	{ 0xDF, "Altron Corporation" },
	{ 0xE0, "K.K. DCE" },
	{ 0xE1, "Towa Chiki" },
	{ 0xE2, "Yutaka" },
	/* { 0xE3, "Varie" }, */
	{ 0xE3, "Kaken Corporation" },
	{ 0xE5, "Epoch" },
	{ 0xE7, "Athena" },
	{ 0xE8, "Asmik Ace Entertainment Inc." },
	{ 0xE9, "Natsume" },
	{ 0xEA, "King Records" },
	{ 0xEB, "Atlus" },
	{ 0xEC, "Epic/Sony Records" },
	/*{ 0xEE, "IGS" },*/
	{ 0xEF, "Fujimic" },
	{ 0xF0, "A Wave" },
	{ 0 },
};

static const char *GetCode(uint8 code) {
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
	if (ROM.disk.data) {
		free(ROM.disk.data);
	}
	ROM.disk.data = NULL;
	if (ROM.prg.data) {
		free(ROM.prg.data);
	}
	ROM.prg.data = NULL;
	if (WRAM) {
		free(WRAM);
	}
	WRAM = NULL;
	if (CHRRAM) {
		free(CHRRAM);
	}
	CHRRAM = NULL;
}

static int SubLoad(FCEUFILE *fp) {
	struct md5_context md5;
	uint8 header[16];

	FCEU_fread(header, 16, 1, fp);

	if (memcmp(header, "FDS\x1a", 4)) {
		if (!(memcmp(header, "\x1*NINTENDO-HVC*", 15))) {
			long t;
			t = FCEU_fgetsize(fp);
			if (t < BYTES_PER_SIDE) {
				t = BYTES_PER_SIDE;
			}
			TotalSides = t / BYTES_PER_SIDE;
			FCEU_fseek(fp, 0, SEEK_SET);
		} else {
			return (0);
		}
	} else {
		TotalSides = header[4];
	}

	if (TotalSides > 8) {
		TotalSides = 8;
	}
	if (TotalSides < 1) {
		TotalSides = 1;
	}

	ROM.disk.size = TotalSides * BYTES_PER_SIDE;
	ROM.disk.data = (uint8 *)FCEU_malloc(ROM.disk.size);

	if (!ROM.disk.data) {
		return (0);
	}

	md5_starts(&md5);

	FCEU_fread(ROM.disk.data, 1, ROM.disk.size, fp);

	md5_update(&md5, ROM.disk.data, ROM.disk.size);
	md5_finish(&md5, GameInfo->MD5);

	return (1);
}

static void PreSave(void) {
	uint32 x;
	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			ROM.disk.data[(BYTES_PER_SIDE * x) + b] ^= ROM.disko.data[(BYTES_PER_SIDE * x) + b];
		}
	}
}

static void PostSave(void) {
	uint32 x;
	for (x = 0; x < TotalSides; x++) {
		int b;
		for (b = 0; b < BYTES_PER_SIDE; b++) {
			ROM.disk.data[(BYTES_PER_SIDE * x) + b] ^= ROM.disko.data[(BYTES_PER_SIDE * x) + b];
		}
	}
}

static FCEUFILE *LoadBIOS(const char *name) {
	FCEUFILE *tmp = NULL;
	char *fn = FCEU_MakeFName(FCEUMKF_FDSROM, 0, 0);

	if (!(tmp = FCEU_fopen(fn, NULL, 0))) {
		FCEU_PrintError("FDS BIOS ROM image missing!\n");
		FCEUD_DispMessage(
		    RETRO_LOG_ERROR, 3000, "FDS BIOS image (disksys.rom) missing");
		free(fn);
		return FALSE;
	}

	free(fn);
	return (tmp);
}

int FDSLoad(const char *name, FCEUFILE *fp) {
	FCEUFILE *biosfile;

	if (!(biosfile = LoadBIOS(name))) {
		return FALSE;
	}

	FreeFDSMemory();

	ResetCartMapping();

	ROM.prg.size = 8192;
	ROM.prg.data = (uint8 *)FCEU_gmalloc(ROM.prg.size);
	SetupCartPRGMapping(0, ROM.prg.data, ROM.prg.size, 0);

	if (FCEU_fread(ROM.prg.data, 1, ROM.prg.size, biosfile) != ROM.prg.size) {
		if (ROM.prg.data) {
			free(ROM.prg.data);
		}
		ROM.prg.data = NULL;
		FCEU_fclose(biosfile);
		FCEU_PrintError("Error reading FDS BIOS ROM image.\n");
		FCEUD_DispMessage(RETRO_LOG_ERROR, 3000,
		    "Error reading FDS BIOS image (disksys.rom)");
		return FALSE;
	}

	FCEU_fclose(biosfile);

	FCEU_fseek(fp, 0, SEEK_SET);

	if (!SubLoad(fp)) {
		if (ROM.prg.data) {
			free(ROM.prg.data);
		}
		ROM.prg.data = NULL;
		return FALSE;
	}

	/* Original disk data backup, to help in creating save states. */
	ROM.disko.size = ROM.disk.size;
	ROM.disko.data = (uint8 *)FCEU_malloc(ROM.disko.size);
	memcpy(ROM.disko.data, ROM.disk.data, ROM.disk.size);

	GameInfo->type = GIT_FDS;
	GameInterface = FDSGI;

	InDisk = 0;
	SelectDisk = 0;

	ResetExState(PreSave, PostSave);

	AddExState(ROM.disk.data, ROM.disk.size, 0, "DDTA");

	AddExState(&DiskIOEnabled, 1, 0, "DREG");
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

	FDSSound_AddStateInfo();

	CHRRAMSize = 8192;
	CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CHRR");

	WRAMSIZE = 32768;
	WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(1, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "FDSR");

	setmirror(MI_H);

	FCEU_printf(" Code         : %02x\n", ROM.disk.data[0x0f]);
	FCEU_printf(" Manufacturer : %s\n", GetCode(ROM.disk.data[0x0f]));
	FCEU_printf(" # of Sides   : %d\n", TotalSides);
	FCEU_printf(" ROM MD5      : 0x%s\n", md5_asciistr(GameInfo->MD5));

	FCEUI_SetVidSystem(0);

	return TRUE;
}

void FDSClose(void) {
	if (ROM.disko.data) {
		free(ROM.disko.data);
		ROM.disko.data = NULL;
	}
	FreeFDSMemory();
}

uint8 *FDSROM_ptr(void) {
	return (ROM.disk.data);
}

uint32 FDSROM_size(void) {
	return (ROM.disk.size);
}
