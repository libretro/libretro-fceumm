/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 BERO
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

#include <file/file_path.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fceu-types.h"
#include "fceu.h"
#include "cart.h"
#include "ppu.h"
#include "x6502.h"

#include "cheat.h"
#include "crc32.h"
#include "fceu-memory.h"
#include "file.h"
#include "general.h"
#include "ines.h"
#include "md5.h"
#include "state.h"
#include "unif.h"
#include "vsuni.h"

#include "ines-correct.h"

struct console_type {
	uint8 id;
	char *name;
};

static const struct console_type console_types_str[] = {
	{ 0x00, "NES/Famicom/Dendy" },
	{ 0x01, "Nintendo Vs. System" },
	{ 0x02, "Playchoice 10" },
	{ 0x03, "Famiclone, but with CPU that supports Decimal Mode" },
	{ 0x04, "NES/Famicom with EPSM module or plug-through cartridge" },
	{ 0x05, "V.R. Technology VT01 with red/cyan STN palette" },
	{ 0x06, "V.R. Technology VT02" },
	{ 0x07, "V.R. Technology VT03" },
	{ 0x08, "V.R. Technology VT09" },
	{ 0x09, "V.R. Technology VT32" },
	{ 0x0A, "V.R. Technology VT369" },
	{ 0x0B, "UMC UM6578" },
	{ 0x0C, "Famicom Network System" },
	{ 0x0D, "reserved" },
	{ 0x0E, "reserved" },
	{ 0x0F, "reserved" },
	{ 0 },
};

struct input_type {
	uint8 id;
	char *name;
};

static const struct input_type input_type_str[] = {
	{ 0x00, "Unspecified" },
	{ 0x01, "Standard controllers" },
	{ 0x02, "NES Four Score/Satellite" },
	{ 0x03, "Famicom Four Players Adapter" },
	{ 0x04, "Vs. System (1P via $4016)" },
	{ 0x05, "Vs. System (1P via $4017)" },
	{ 0x06, "Reserved" },
	{ 0x07, "Vs. Zapper" },
	{ 0x08, "Zapper ($4017)" },
	{ 0x09, "Two Zappers" },
	{ 0x0A, "Bandai Hyper Shot Lightgun" },
	{ 0x0B, "Power Pad Side A" },
	{ 0x0C, "Power Pad Side B" },
	{ 0x0D, "Family Trainer Side A" },
	{ 0x0E, "Family Trainer Side B" },
	{ 0x0F, "Arkanoid Paddle (NES)" },
	{ 0x10, "Arkanoid Paddle (Famicom)" },
	{ 0x11, "Two Arkanoid Paddles plus Famicom Data Recorder" },
	{ 0x12, "Konami Hyper Shot Controller" },
	{ 0x13, "Coconuts Pachinko Controller" },
	{ 0x14, "Exciting Boxing Punching Bag" },
	{ 0x15, "Jissen Mahjong Controller" },
	{ 0x16, "Party Tap" },
	{ 0x17, "Oeka Kids Tablet" },
	{ 0x18, "Sunsoft Barcode Battler" },
	{ 0x19, "Miracle Piano Keyboard" },
	{ 0x1A, "Pokkun Moguraa" },
	{ 0x1B, "Top Rider" },
	{ 0x1C, "Double-Fisted" },
	{ 0x1D, "Famicom 3D System" },
	{ 0x1E, "Doremikko Keyboard" },
	{ 0x1F, "R.O.B. Gyro Set" },
	{ 0x20, "Famicom Data Recorder (\"silent\" keyboard)" },
	{ 0x21, "ASCII Turbo File" },
	{ 0x22, "IGS Storage Battle Box" },
	{ 0x23, "Family BASIC Keyboard" },
	{ 0x24, "Dongda PEC-586 Keyboard" },
	{ 0x25, "Bit Corp. Bit-79 Keyboard" },
	{ 0x26, "Subor Keyboard" },
	{ 0x27, "Subor Keyboard plus 3x8-bit mouse" },
	{ 0x28, "Subor Keyboard plus 24-bit mouse $4016" },
	{ 0x29, "SNES Mouse ($4017.d0)" },
	{ 0x2A, "Multicart" },
	{ 0x2B, "Two SNES controllers" },
	{ 0x2C, "RacerMate Bicycle" },
	{ 0x2D, "U-Force" },
	{ 0x2E, "R.O.B. Stack-Up" },
	{ 0x2F, "City Patrolman Lightgun" },
	{ 0x30, "Sharp C1 Cassette Interface" },
	{ 0x31, "Standard Controller with swapped Left-Right/Up-Down/B-A" },
	{ 0x32, "Excalibor Sudoku Pad" },
	{ 0x33, "ABL Pinball" },
	{ 0x34, "Golden Nugget Casino extra buttons" },
	{ 0x35, "Unknown famiclone keyboard used by the \"Golden Key\" educational cartridge" },
	{ 0x36, "Subor Keyboard plus 24-bit mouse (via $4017)" },
	{ 0x37, "Port test controller" },
	{ 0x38, "Bandai Multi Game Player Gamepad" },
	{ 0x39, "Venom TV Dance Mat" },
	{ 0x3A, "LG TV Remote Control" },
	{ 0 }
};

extern SFORMAT FCEUVSUNI_STATEINFO[];

CartInfo iNESCart = { 0 };

static int CHRRAMSize = -1;

static int iNES_Init(int num);
static DECLFR(TrainerRead) {
	return (ROM.trainer.data[A & 0x1FF]);
}

static void iNES_ExecPower(void) {
	if (CHRRAMSize != -1) {
		FCEU_MemoryRand(UNIFchrrama, CHRRAMSize);
	}

	if (iNESCart.Power) {
		iNESCart.Power();
	}

	if (ROM.trainer.data) {
		int x;
		for (x = 0; x < 512; x++) {
			X6502_DMW(0x7000 + x, ROM.trainer.data[x]);
			if (X6502_DMR(0x7000 + x) != ROM.trainer.data[x]) {
				SetReadHandler(0x7000, 0x71FF, TrainerRead);
				break;
			}
		}
	}
}

static void Cleanup(void) {
	if (ROM.prg.data) {
		free(ROM.prg.data);
		ROM.prg.data = NULL;
	}
	if (ROM.chr.data) {
		free(ROM.chr.data);
		ROM.chr.data = NULL;
	}
	if (ROM.trainer.data) {
		free(ROM.trainer.data);
		ROM.trainer.data = NULL;
	}
	if (ROM.misc.data) {
		free(ROM.misc.data);
		ROM.misc.data = NULL;
	}
	if (WRAM) {
		free(WRAM);
		WRAM = NULL;
		WRAMSIZE = 0;
	}
}

static void iNESGI(int h) {
	switch (h) {
	case GI_RESETM2:
		if (iNESCart.Reset) iNESCart.Reset();
		break;
	case GI_POWER:
		iNES_ExecPower();
		break;
	case GI_CLOSE:
		if (iNESCart.Close) iNESCart.Close();
		Cleanup();
		break;
	}
}

struct CRCMATCH {
	uint32 crc;
	char *name;
};

struct INPSEL {
	uint32 crc32;
	int input1;
	int input2;
	int inputfc;
};

static void SetInput(CartInfo *info) {
	static struct INPSEL inpsel_nes10[] = {
		{ 0x19b0a9f1, SI_GAMEPAD, SI_ZAPPER, SIFC_NONE }, /* 6-in-1 (MGC-023)(Unl)[!] */
		{ 0x29de87af, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Aerobics Studio */
		{ 0xd89e5a67, SI_UNSET, SI_UNSET, SIFC_ARKANOID }, /* Arkanoid (J) */
		{ 0x0f141525, SI_UNSET, SI_UNSET, SIFC_ARKANOID }, /* Arkanoid 2(J) */
		{ 0x32fb0583, SI_UNSET, SI_ARKANOID, SIFC_NONE }, /* Arkanoid(NES) */
		{ 0x60ad090a, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERA }, /* Athletic World */
		{ 0x48ca0ee1, SI_GAMEPAD, SI_GAMEPAD, SIFC_BWORLD }, /* Barcode World */
		{ 0x4318a2f8, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Barker Bill's Trick Shooting */
		{ 0x6cca1c1f, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Dai Undoukai */
		{ 0x24598791, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Duck Hunt */
		{ 0xd5d6eac4, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Edu (As) */
		{ 0xe9a7fe9e, SI_UNSET, SI_MOUSE, SIFC_NONE }, /* Educational Computer 2000 */
		{ 0x8f7b1669, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* FP BASIC 3.3 by maxzhou88 */
		{ 0xf7606810, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Family BASIC 2.0A */
		{ 0x895037bc, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Family BASIC 2.1a */
		{ 0xb2530afc, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Family BASIC 3.0 */
		{ 0xea90f3e2, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Family Trainer:  Running Stadium */
		{ 0xbba58be5, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Family Trainer: Manhattan Police */
		{ 0x3e58a87e, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Freedom Force */
		{ 0xd9f45be9, SI_GAMEPAD, SI_GAMEPAD, SIFC_PARTYTAP }, /* Gimme a Break ... */
		{ 0x1545bd13, SI_GAMEPAD, SI_GAMEPAD, SIFC_PARTYTAP }, /* Gimme a Break ... 2 */
		{ 0x4e959173, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Gotcha! - The Sport! */
		{ 0xbeb8ab01, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Gumshoe */
		{ 0xff24d794, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Hogan's Alley */
		{ 0x21f85681, SI_GAMEPAD, SI_GAMEPAD, SIFC_HYPERSHOT }, /* Hyper Olympic (Gentei Ban) */
		{ 0x980be936, SI_GAMEPAD, SI_GAMEPAD, SIFC_HYPERSHOT }, /* Hyper Olympic */
		{ 0x915a53a7, SI_GAMEPAD, SI_GAMEPAD, SIFC_HYPERSHOT }, /* Hyper Sports */
		{ 0x9fae4d46, SI_GAMEPAD, SI_GAMEPAD, SIFC_MAHJONG }, /* Ide Yousuke Meijin no Jissen Mahjong */
		{ 0x7b44fb2a, SI_GAMEPAD, SI_GAMEPAD, SIFC_MAHJONG }, /* Ide Yousuke Meijin no Jissen Mahjong 2 */
		{ 0x2f128512, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERA }, /* Jogging Race */
		{ 0xbb33196f, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Keyboard Transformer */
		{ 0x8587ee00, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Keyboard Transformer */
		{ 0x543ab532, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* LIKO Color Lines */
		{ 0x368c19a8, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* LIKO Study Cartridge */
		{ 0x5ee6008e, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Mechanized Attack */
		{ 0x370ceb65, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Meiro Dai Sakusen */
		{ 0x3a1694f9, SI_GAMEPAD, SI_GAMEPAD, SIFC_4PLAYER }, /* Nekketsu Kakutou Densetsu */
		{ 0x9d048ea4, SI_GAMEPAD, SI_GAMEPAD, SIFC_OEKAKIDS }, /* Oeka Kids */
		{ 0x2a6559a1, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Operation Wolf (J) */
		{ 0xedc3662b, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Operation Wolf */
		{ 0x912989dc, SI_UNSET, SI_UNSET, SIFC_FKB }, /* Playbox BASIC */
		{ 0x9044550e, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERA }, /* Rairai Kyonshizu */
		{ 0xea90f3e2, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Running Stadium */
		{ 0x851eb9be, SI_GAMEPAD, SI_ZAPPER, SIFC_NONE }, /* Shooting Range */
		{ 0x6435c095, SI_GAMEPAD, SI_POWERPADB, SIFC_UNSET }, /* Short Order/Eggsplode */
		{ 0xc043a8df, SI_UNSET, SI_MOUSE, SIFC_NONE }, /* Shu Qi Yu - Shu Xue Xiao Zhuan Yuan (Ch) */
		{ 0x2cf5db05, SI_UNSET, SI_MOUSE, SIFC_NONE }, /* Shu Qi Yu - Zhi Li Xiao Zhuan Yuan (Ch) */
		{ 0xad9c63e2, SI_GAMEPAD, SI_UNSET, SIFC_SHADOW }, /* Space Shadow */
		{ 0x61d86167, SI_GAMEPAD, SI_POWERPADB, SIFC_UNSET }, /* Street Cop */
		{ 0xabb2f974, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Study and Game 32-in-1 */
		{ 0x41ef9ac4, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Subor */
		{ 0x8b265862, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Subor */
		{ 0x82f1fb96, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Subor 1.0 Russian */
		{ 0x9f8f200a, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERA }, /* Super Mogura Tataki!! - Pokkun Moguraa */
		{ 0xd74b2719, SI_GAMEPAD, SI_POWERPADB, SIFC_UNSET }, /* Super Team Games */
		{ 0x74bea652, SI_GAMEPAD, SI_ZAPPER, SIFC_NONE }, /* Supergun 3-in-1 */
		{ 0x5e073a1b, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* Supor English (Chinese) */
		{ 0x589b6b0d, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* SuporV20 */
		{ 0x41401c6d, SI_UNSET, SI_UNSET, SIFC_SUBORKB }, /* SuporV40 */
		{ 0x23d17f5e, SI_GAMEPAD, SI_ZAPPER, SIFC_NONE }, /* The Lone Ranger */
		{ 0xc3c0811d, SI_GAMEPAD, SI_GAMEPAD, SIFC_OEKAKIDS }, /* The two "Oeka Kids" games */
		{ 0xde8fd935, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* To the Earth */
		{ 0x47232739, SI_GAMEPAD, SI_GAMEPAD, SIFC_TOPRIDER }, /* Top Rider */
		{ 0x8a12a7d9, SI_GAMEPAD, SI_GAMEPAD, SIFC_FTRAINERB }, /* Totsugeki Fuuun Takeshi Jou */
		{ 0xb8b9aca3, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Wild Gunman */
		{ 0x5112dc21, SI_UNSET, SI_ZAPPER, SIFC_NONE }, /* Wild Gunman */
		{ 0xaf4010ea, SI_GAMEPAD, SI_POWERPADB, SIFC_UNSET }, /* World Class Track Meet */
		{ 0xb3cc4d26, SI_GAMEPAD, SI_UNSET, SIFC_SHADOW }, /* 2-in-1 Uzi Lightgun (MGC-002) */

		{ 0x00000000, SI_UNSET, SI_UNSET, SIFC_UNSET }
	};
	int x = 0;

	while (inpsel_nes10[x].input1 >= 0 || inpsel_nes10[x].input2 >= 0 || inpsel_nes10[x].inputfc >= 0) {
		if (inpsel_nes10[x].crc32 == info->CRC32) {
			GameInfo->input[0] = inpsel_nes10[x].input1;
			GameInfo->input[1] = inpsel_nes10[x].input2;
			GameInfo->inputfc  = inpsel_nes10[x].inputfc;
			break;
		}
		x++;
	}
}

struct INPSEL_NES20 {
	uint8 id;
	int input1;
	int input2;
	int inputfc;
};

/*
* Function to set input controllers based on NES 2.0 header
*/

static void SetInputNes20(uint8 input_id) {
	static struct INPSEL_NES20 inpsel_nes20[] =
	{
		{ 0x01,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_UNSET		}, /* Standard NES/Famicom controllers */
		{ 0x02,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_NONE		}, /* NES Four Score/Satellite with two additional standard controllers */
		{ 0x03,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_4PLAYER	}, /* Famicom Four Players Adapter with two additional standard controllers using the "simple" protocol */
		{ 0x04,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_NONE		}, /* Vs. System (1P via $4016) */
		{ 0x05,	SI_GAMEPAD,		SI_GAMEPAD,		SIFC_NONE		}, /* Vs. System (1P via $4017) */
		{ 0x07,	SI_ZAPPER,		SI_NONE,		SIFC_NONE		}, /* Vs. Zapper */
		{ 0x08,	SI_UNSET,		SI_ZAPPER,		SIFC_NONE		}, /* Zapper ($4017) */
		{ 0x0A,	SI_UNSET,		SI_UNSET,		SIFC_SHADOW		}, /* Bandai Hyper Shot Lightgun */
		{ 0x0B,	SI_UNSET,		SI_POWERPADA,	SIFC_UNSET		}, /* Power Pad Side A */
		{ 0x0C,	SI_UNSET,		SI_POWERPADB,	SIFC_UNSET		}, /* Power Pad Side B */
		{ 0x0D,	SI_UNSET,		SI_UNSET,		SIFC_FTRAINERA	}, /* Family Trainer Side A */
		{ 0x0E,	SI_UNSET,		SI_UNSET,		SIFC_FTRAINERB	}, /* Family Trainer Side B */
		{ 0x0F,	SI_UNSET,		SI_ARKANOID,	SIFC_UNSET		}, /* Arkanoid Paddle (NES) */
		{ 0x10,	SI_UNSET,		SI_UNSET,		SIFC_ARKANOID	}, /* Arkanoid Paddle (Famicom) */
		{ 0x12,	SI_UNSET,		SI_UNSET,		SIFC_HYPERSHOT	}, /* Konami Hyper Shot Controller */
		{ 0x14, SI_GAMEPAD,		SI_UNSET,		SIFC_EXCITINGBOXING }, /* Exciting Boxing Punching Bag */
		{ 0x15,	SI_UNSET,		SI_UNSET,		SIFC_MAHJONG	}, /* Jissen Mahjong Controller */
		{ 0x17,	SI_UNSET,		SI_UNSET,		SIFC_OEKAKIDS	}, /* Oeka Kids Tablet */
		{ 0x18,	SI_UNSET,		SI_UNSET,		SIFC_BWORLD		}, /* Sunsoft Barcode Battler */
		{ 0x1B,	SI_UNSET,		SI_UNSET,		SIFC_TOPRIDER	}, /* Top Rider (Inflatable Bicycle) */
		{ 0x23,	SI_UNSET,		SI_UNSET,		SIFC_FKB		}, /* Family BASIC Keyboard plus Famicom Data Recorder */
		{ 0x24,	SI_UNSET,		SI_UNSET,		SIFC_PEC586KB	}, /* Dongda PEC-586 Keyboard */
		{ 0x26,	SI_UNSET,		SI_UNSET,		SIFC_SUBORKB	}, /* Subor Keyboard */
		/* { 0x27,	SI_UNSET,		SI_MOUSE,		SIFC_SUBORKB	}, */ /* Subor Keyboard plus mouse (3x8-bit protocol) */
		{ 0x28,	SI_UNSET,		SI_MOUSE,		SIFC_SUBORKB	}, /* Subor Keyboard plus mouse (24-bit protocol) */
		{ 0x29,	SI_UNSET,		SI_SNES_MOUSE,	SIFC_UNSET		}, /* SNES Mouse */
		{ 0x00,	SI_UNSET,		SI_UNSET,		SIFC_UNSET		}
	};

	int x = 0;

	/* four score adaptor */
	/*if (device == 0x02)
		eoptions |= 32768;*/
	if (input_id == 0x05)
		vsuni_system.ioption |= IOPTION_SWAPDIRAB;

	while (inpsel_nes20[x].id) {
		if (inpsel_nes20[x].id == input_id) {
			GameInfo->input[0] = inpsel_nes20[x].input1;
			GameInfo->input[1] = inpsel_nes20[x].input2;
			GameInfo->inputfc  = inpsel_nes20[x].inputfc;
			break;
		}
		x++;
	}
}

#define INESB_INCOMPLETE 1
#define INESB_CORRUPT    2
#define INESB_HACKED     4

struct BADINF {
	uint64 md5partial;
	uint8 *name;
	uint32 type;
};

static struct BADINF BadROMImages[] = {
#include "ines-bad.h"
};

static void CheckBad(uint64 md5partial) {
	int32 x = 0;
	while (BadROMImages[x].name) {
		if (BadROMImages[x].md5partial == md5partial) {
			FCEU_PrintError("The copy game you have loaded, \"%s\", is bad, and will not work properly in FCE Ultra.",
			                BadROMImages[x].name);
			return;
		}
		x++;
	}
}

static char *mirroring_str[] = {
	"Horizontal",
	"Vertical",
	"Single-screen 0",
	"Single-screen 1",
	"\"Four-screen\""
};

const char *tv_region_str[] = {
	"NTSC",
	"PAL",
	"Multi-region",
	"Dendy",
};

static void CheckHInfo(CartInfo *info, uint64 partialmd5) {
	int32 tofix = 0;
	int x = 0;

	CheckBad(partialmd5);

	do {
		if (nesdb[x].crc32 == info->CRC32) {
			if (nesdb[x].mapper >= 0) {
				if (nesdb[x].extra >= 0 && nesdb[x].extra == 0x800 && ROM.chr.size) {
					ROM.chr.size = 0;
					free(ROM.chr.data);
					ROM.chr.data = NULL;
					tofix |= 8;
				}
				if (info->mapper != (nesdb[x].mapper & 0xFFF)) {
					tofix |= 1;
					info->mapper = nesdb[x].mapper & 0xFFF;
				}
			}
			if (nesdb[x].submapper >= 0) {
				info->iNES2 = 1;
				if (nesdb[x].submapper != info->submapper) {
					info->submapper = nesdb[x].submapper;
				}
			}
			if (nesdb[x].mirror >= 0) {
				if (info->mirror != nesdb[x].mirror) {
					/* TODO: Verify this condition */
					if (info->mirror != (nesdb[x].mirror & ~MI_4))
						if ((nesdb[x].mirror & ~MI_4) <= 2) /* Don't complain if one-screen mirroring
						                                 needs to be set(the iNES header can't
						                                 hold this information).
						                                 */
							tofix |= 2;
					info->mirror = nesdb[x].mirror;
				}
			}
			if (nesdb[x].battery >= 0) {
				if ((info->battery == 0) && (nesdb[x].battery != 0)) {
					tofix |= 4;
					info->battery = 1;
				}
			}
			if (nesdb[x].region >= 0) {
				if (info->region != nesdb[x].region) {
					tofix |= 16;
					info->region = nesdb[x].region;
				}
			}

			if (nesdb[x].prgram >= 0) {
				int prgram = (nesdb[x].prgram & 0x0F) ? (64 << ((nesdb[x].prgram >> 0) & 0xF)) : 0;
				int prgsaveram = (nesdb[x].prgram & 0xF0) ? (64 << ((nesdb[x].prgram >> 4) & 0xF)) : 0;
				if (prgram != info->PRGRamSize || prgsaveram != info->PRGRamSaveSize) {
					tofix |= 32;
					info->iNES2          = 1;
					info->PRGRamSize     = (nesdb[x].prgram & 0x0F) ? (64 << ((nesdb[x].prgram >> 0) & 0xF)) : 0;
					info->PRGRamSaveSize = (nesdb[x].prgram & 0xF0) ? (64 << ((nesdb[x].prgram >> 4) & 0xF)) : 0;
				}
			}

			if (nesdb[x].chrram >= 0) {
				int chrram = (nesdb[x].chrram & 0x0F) ? (64 << ((nesdb[x].chrram >> 0) & 0xF)) : 0;
				int chrsaveram = (nesdb[x].chrram & 0xF0) ? (64 << ((nesdb[x].chrram >> 4) & 0xF)) : 0;
				if (chrram != info->CHRRamSize || chrsaveram != info->CHRRamSaveSize) {
					tofix |= 32;
					info->iNES2          = 1;
					info->CHRRamSize     = (nesdb[x].chrram & 0x0F) ? (64 << ((nesdb[x].chrram >> 0) & 0xF)) : 0;
					info->CHRRamSaveSize = (nesdb[x].chrram & 0xF0) ? (64 << ((nesdb[x].chrram >> 4) & 0xF)) : 0;
				}
			}

			break;
		}
		x++;
	} while (nesdb[x].mirror >= 0 || nesdb[x].mapper >= 0);

	/* Games that use these iNES mappers tend to have the four-screen bit set
	   when it should not be.
	   */
	if ((info->mapper == 118 || info->mapper == 24 || info->mapper == 26) && (info->mirror == MI_4)) {
		info->mirror = MI_H;
		tofix |= 2;
	}

	/* Four-screen mirroring implicitly set. */
	if (info->mapper == 99) {
		if (info->mirror != MI_4) {
			tofix |= 2;
			info->mirror = MI_4;
		}
	}

	if (tofix) {
		FCEU_printf(" The iNES header contains incorrect information.  For now, the information will be corrected in RAM.\n");
		if (tofix & 1) {
			FCEU_printf(" [DB] Mapper: %3d\n", info->mapper);
		}
		if (tofix & 2) {
			FCEU_printf(" [DB] Mirroring: %s\n", mirroring_str[info->mirror]);
		}
		if (tofix & 4) {
			FCEU_printf(" [DB] Battery: %s\n", info->battery ? "Yes" : "No");
		}
		if (tofix & 8) {
			FCEU_printf(" [DB] CHR ROM: %d\n", info->CHRRomSize);
		}
		if (tofix & 16) {
			FCEU_printf(" [DB] Region: %s\n", tv_region_str[info->region]);
		}
		if (tofix & 32) {
			size_t PRGRAM = info->PRGRamSize + info->PRGRamSaveSize;
			size_t CHRRAM = info->CHRRamSize + info->CHRRamSaveSize;
			if (PRGRAM || CHRRAM) {
				if (info->PRGRamSaveSize == 0) {
					FCEU_printf(" [DB] PRG RAM: %d KB, ", PRGRAM / 1024);
				} else {
					FCEU_printf(" [DB] PRG RAM: %d KB ( %dKB is battery-backed )\n", PRGRAM / 1024, info->PRGRamSaveSize / 1024);
				}
				FCEU_printf(" [DB] CHR RAM: %d KB\n", CHRRAM / 1024);
			}
		}
	}
}

typedef struct {
	int32 mapper;
	void (*init)(CartInfo *);
} NewMI;

typedef struct {
	uint8 *name;
	int32 number;
	void (*init)(CartInfo *);
} BMAPPINGLocal;

#define INES_BOARD_BEGIN() static BMAPPINGLocal bmap[] = {
#define INES_BOARD_END(void) { (uint8_t *)"", 0, NULL } };
#define INES_BOARD(a, b, c) { (uint8_t *)a, b, c },

INES_BOARD_BEGIN()
	INES_BOARD( "NROM",                       0, Mapper000_Init         )
	INES_BOARD( "MMC1",                       1, Mapper001_Init         )
	INES_BOARD( "UNROM",                      2, Mapper002_Init         )
	INES_BOARD( "CNROM",                      3, Mapper003_Init         )
	INES_BOARD( "MMC3",                       4, Mapper004_Init         )
	INES_BOARD( "MMC5",                       5, Mapper005_Init         )
	INES_BOARD( "FFE Rev. A",                 6, Mapper006_Init         )
	INES_BOARD( "ANROM",                      7, Mapper007_Init         )
	INES_BOARD( "",                           8, Mapper008_Init         ) /* no games, it's worthless */
	INES_BOARD( "MMC2",                       9, Mapper009_Init         )
	INES_BOARD( "MMC4",                      10, Mapper010_Init         )
	INES_BOARD( "Color Dreams",              11, Mapper011_Init         )
	INES_BOARD( "REX DBZ 5",                 12, Mapper012_Init         )
	INES_BOARD( "CPROM",                     13, Mapper013_Init         )
	INES_BOARD( "REX SL-1632",               14, Mapper014_Init         )
	INES_BOARD( "100-in-1",                  15, Mapper015_Init         )
	INES_BOARD( "BANDAI 24C02",              16, Mapper016_Init         )
	INES_BOARD( "FFE Rev. B",                17, Mapper017_Init         )
	INES_BOARD( "JALECO SS880006",           18, Mapper018_Init         ) /* JF-NNX (EB89018-30007) boards */
	INES_BOARD( "Namco 129/163",             19, Mapper019_Init         )
/*    INES_BOARD( "",                         20, Mapper20_Init ) */
	INES_BOARD( "Konami VRC2/VRC4 A",        21, Mapper021_Init         )
	INES_BOARD( "Konami VRC2/VRC4 B",        22, Mapper022_Init         )
	INES_BOARD( "Konami VRC2/VRC4 C",        23, Mapper023_Init         )
	INES_BOARD( "Konami VRC6 Rev. A",        24, Mapper024_Init         )
	INES_BOARD( "Konami VRC2/VRC4 D",        25, Mapper025_Init         )
	INES_BOARD( "Konami VRC6 Rev. B",        26, Mapper026_Init         )
	INES_BOARD( "CC-21 MI HUN CHE",          27, Mapper027_Init         ) /* Former dupe for VRC2/VRC4 mapper, redefined with crc to mihunche boards */
	INES_BOARD( "Action 53",                 28, Mapper028_Init         )
	INES_BOARD( "RET-CUFROM",                29, Mapper029_Init         )
	INES_BOARD( "UNROM 512",                 30, Mapper030_Init         )
	INES_BOARD( "infineteNesLives-NSF",      31, Mapper031_Init         )
	INES_BOARD( "IREM G-101",                32, Mapper032_Init         )
	INES_BOARD( "Taito TC0190FMC/TC0350FMR", 33, Mapper033_Init         )
	INES_BOARD( "BNROM/NINA-001",            34, Mapper034_Init         )
	INES_BOARD( "EL870914C",                 35, Mapper035_Init         )
	INES_BOARD( "TXC Policeman",             36, Mapper036_Init         )
	INES_BOARD( "PAL-ZZ SMB/TETRIS/NWC",     37, Mapper037_Init         )
	INES_BOARD( "Bit Corp.",                 38, Mapper038_Init         ) /* Crime Busters */
/*    INES_BOARD( "",                         39, Mapper39_Init ) */
	INES_BOARD( "SMB2j FDS",                 40, Mapper040_Init         )
	INES_BOARD( "CALTRON 6-in-1",            41, Mapper041_Init         )
	INES_BOARD( "BIO MIRACLE FDS",           42, Mapper042_Init         )
	INES_BOARD( "FDS SMB2j LF36",            43, Mapper043_Init         )
	INES_BOARD( "MMC3 BMC PIRATE A",         44, Mapper044_Init         )
	INES_BOARD( "MMC3 BMC PIRATE B",         45, Mapper045_Init         )
	INES_BOARD( "RUMBLESTATION 15-in-1",     46, Mapper046_Init         )
	INES_BOARD( "NES-QJ SSVB/NWC",           47, Mapper047_Init         )
	INES_BOARD( "Taito TC0690/TC190+PAL16R4", 48, Mapper048_Init        )
	INES_BOARD( "MMC3 BMC PIRATE C",         49, Mapper049_Init         )
	INES_BOARD( "SMB2j FDS Rev. A",          50, Mapper050_Init         )
	INES_BOARD( "11-in-1 BALL SERIES",       51, Mapper051_Init         ) /* 1993 year version */
	INES_BOARD( "MMC3 BMC PIRATE D",         52, Mapper052_Init         )
	INES_BOARD( "SUPERVISION 16-in-1",       53, Mapper053_Init         )
/*    INES_BOARD( "",                         54, Mapper54_Init ) */
    INES_BOARD( "MARIO1-MALEE2",             55, Mapper055_Init         )
	INES_BOARD( "UNLKS202",                  56, Mapper056_Init         )
	INES_BOARD( "SIMBPLE BMC PIRATE A",      57, Mapper057_Init         )
	INES_BOARD( "SIMBPLE BMC PIRATE B",      58, Mapper058_Init         )
	INES_BOARD( "BMC T3H53/D1038",           59, Mapper059_Init         )
	INES_BOARD( "Reset-based NROM-128 ",     60, Mapper060_Init         )
	INES_BOARD( "20-in-1 KAISER Rev. A",     61, Mapper061_Init         )
	INES_BOARD( "700-in-1",                  62, Mapper062_Init         )
	INES_BOARD( "Powerful 250-in-1 (NTDEC TH2291)", 63, Mapper063_Init  )
	INES_BOARD( "TENGEN RAMBO1",             64, Mapper064_Init         )
	INES_BOARD( "IREM-H3001",                65, Mapper065_Init         )
	INES_BOARD( "GNROM / MHROM",             66, Mapper066_Init         )
	INES_BOARD( "SUNSOFT-FZII",              67, Mapper067_Init         )
	INES_BOARD( "Sunsoft Mapper #4",         68, Mapper068_Init         )
	INES_BOARD( "SUNSOFT-5/FME-7",           69, Mapper069_Init         )
	INES_BOARD( "BA KAMEN DISCRETE",         70, Mapper070_Init         )
	INES_BOARD( "CAMERICA BF9093",           71, Mapper071_Init         )
	INES_BOARD( "JALECO JF-17",              72, Mapper072_Init         )
	INES_BOARD( "KONAMI VRC3",               73, Mapper073_Init          )
	INES_BOARD( "TW MMC3+VRAM Rev. A",       74, Mapper074_Init         )
	INES_BOARD( "KONAMI VRC1",               75, Mapper075_Init         )
	INES_BOARD( "NAMCOT 108 Rev. A",         76, Mapper076_Init         )
	INES_BOARD( "IREM LROG017",              77, Mapper077_Init         )
	INES_BOARD( "Irem 74HC161/32",           78, Mapper078_Init         )
	INES_BOARD( "AVE/C&E/TXC BOARD",         79, Mapper079_Init         )
	INES_BOARD( "TAITO X1-005 Rev. A",       80, Mapper080_Init         )
    INES_BOARD( "Super Gun (NTDEC N715021)", 81, Mapper081_Init         )
	INES_BOARD( "TAITO X1-017",              82, Mapper082_Init         )
	INES_BOARD( "YOKO VRC Rev. B",           83, Mapper083_Init         )
/*    INES_BOARD( "",                            84, Mapper84_Init ) */
	INES_BOARD( "KONAMI VRC7",               85, Mapper085_Init         )
	INES_BOARD( "JALECO JF-13",              86, Mapper086_Init         )
	INES_BOARD( "74*139/74 DISCRETE",        87, Mapper087_Init         )
	INES_BOARD( "NAMCO 3433",                88, Mapper088_Init         )
	INES_BOARD( "SUNSOFT-3",                 89, Mapper089_Init         ) /* SUNSOFT-2 mapper */
	INES_BOARD( "HUMMER/JY BOARD",           90, Mapper090_Init         )
	INES_BOARD( "JY830623C/YY840238C/EJ-006-1", 91, Mapper091_Init      )
	INES_BOARD( "JALECO JF-19",              92, Mapper072_Init         )
	INES_BOARD( "SUNSOFT-3R",                93, Mapper093_Init         ) /* SUNSOFT-2 mapper with VRAM, different wiring */
	INES_BOARD( "HVC-UN1ROM",                94, Mapper094_Init         )
	INES_BOARD( "NAMCOT 108 Rev. B",         95, Mapper095_Init         )
	INES_BOARD( "BANDAI OEKAKIDS",           96, Mapper096_Init         )
	INES_BOARD( "IREM TAM-S1",               97, Mapper097_Init         )
/*    INES_BOARD( "",                            98, Mapper98_Init ) */
	INES_BOARD( "Vs. System",                99, Mapper099_Init         )
/*    INES_BOARD( "",                            100, Mapper100_Init ) */
	INES_BOARD( "",                         101, Mapper101_Init         )
/*    INES_BOARD( "",                            102, Mapper102_Init ) */
	INES_BOARD( "FDS DOKIDOKI FULL",        103, Mapper103_Init         )
	INES_BOARD( "CAMERICA GOLDENFIVE",      104, Mapper104_Init         )
	INES_BOARD( "NES-EVENT NWC1990",        105, Mapper105_Init         )
	INES_BOARD( "SMB3 PIRATE A",            106, Mapper106_Init         )
	INES_BOARD( "MAGIC CORP A",             107, Mapper107_Init         )
	INES_BOARD( "FDS UNROM BOARD",          108, Mapper108_Init         )
/*    INES_BOARD( "",                            109, Mapper109_Init ) */
/*    INES_BOARD( "",                            110, Mapper110_Init ) */
	INES_BOARD( "Cheapocabra",              111, Mapper111_Init         )
	INES_BOARD( "ASDER/NTDEC BOARD",        112, Mapper112_Init         )
	INES_BOARD( "HACKER/SACHEN BOARD",      113, Mapper113_Init         )
	INES_BOARD( "MMC3 SG PROT. A",          114, Mapper114_Init         )
	INES_BOARD( "MMC3 PIRATE A",            115, Mapper115_Init         )
	INES_BOARD( "MMC1/MMC3/VRC PIRATE",     116, Mapper116_Init         )
	INES_BOARD( "FUTURE MEDIA BOARD",       117, Mapper117_Init         )
	INES_BOARD( "TSKROM",                   118, Mapper118_Init         )
	INES_BOARD( "TQROM",                119, Mapper119_Init         )
	INES_BOARD( "FDS TOBIDASE",             120, Mapper120_Init         )
	INES_BOARD( "MMC3 PIRATE PROT. A",      121, Mapper121_Init         )
/*    INES_BOARD( "",                            122, Mapper122_Init ) */
	INES_BOARD( "MMC3 PIRATE H2288",        123, Mapper123_Init         )
    INES_BOARD( "Super Game Mega Type III", 124, Mapper124_Init         )
	INES_BOARD( "FDS LH32",                 125, Mapper125_Init         )
	INES_BOARD( "PowerJoy 84-in-1 PJ-008",  126, Mapper126_Init         )
    INES_BOARD( "Double Dragon II (Pirate)", 127, Mapper127_Init        )
    INES_BOARD( "1994 Super HiK 4-in-1",    128, Mapper128_Init         )
/*    INES_BOARD( "",                            129, Mapper129_Init ) */
/*    INES_BOARD( "",                            130, Mapper130_Init ) */
/*    INES_BOARD( "",                            131, Mapper131_Init ) */
	INES_BOARD( "TXC/UNL-22211",            132, Mapper132_Init         )
	INES_BOARD( "SA72008",                  133, Mapper133_Init         )
	INES_BOARD( "MMC3 BMC PIRATE",          134, Mapper134_Init         )
/*    INES_BOARD( "",                            135, Mapper135_Init ) */ /* Duplicate of 135 */
	INES_BOARD( "Sachen 3011",              136, Mapper136_Init         )
	INES_BOARD( "S8259D",                   137, Mapper137_Init         )
	INES_BOARD( "S8259B",                   138, Mapper138_Init         )
	INES_BOARD( "S8259C",                   139, Mapper139_Init         )
	INES_BOARD( "JALECO JF-11/14",          140, Mapper140_Init         )
	INES_BOARD( "S8259A",                   141, Mapper141_Init         )
	INES_BOARD( "UNLKS7032",                142, Mapper142_Init         )
	INES_BOARD( "TCA01",                    143, Mapper143_Init         )
	INES_BOARD( "AGCI 50282",               144, Mapper144_Init         )
	INES_BOARD( "SA72007",                  145, Mapper145_Init         )
/*	INES_BOARD( "",                         146, SA0161M_Init           ) */ /* moved to mapper 79 */
	INES_BOARD( "Sachen 3018 board",        147, Mapper147_Init         )
	INES_BOARD( "SA0037",                   148, Mapper148_Init         )
	INES_BOARD( "SA0036",                   149, Mapper149_Init         )
	INES_BOARD( "SA-015/SA-630",            150, Mapper150_Init         )
	INES_BOARD( "Vs. Unisystem (Konami)",   151, Mapper151_Init         ) /* legacy support. all fixed roms should be using mapper 75 */
	INES_BOARD( "",                         152, Mapper152_Init         )
	INES_BOARD( "BANDAI SRAM",              153, Mapper153_Init         ) /* Bandai board 16 with SRAM instead of EEPROM */
	INES_BOARD( "",                         154, Mapper154_Init         )
	INES_BOARD( "",                         155, Mapper155_Init         )
	INES_BOARD( "",                         156, Mapper156_Init         )
	INES_BOARD( "BANDAI BARCODE",           157, Mapper157_Init         )
	INES_BOARD( "TENGEN 800037",            158, Mapper064_Init         )
	INES_BOARD( "BANDAI 24C01",             159, Mapper159_Init         ) /* Different type of EEPROM on the  bandai board */
/*	INES_BOARD( "SA009",                    160, Mapper160_Init         ) */
/*    INES_BOARD( "",                            161, Mapper161_Init ) */
	INES_BOARD( "",                         162, Mapper162_Init         )
	INES_BOARD( "",                         163, Mapper163_Init         )
	INES_BOARD( "",                         164, Mapper164_Init         )
	INES_BOARD( "",                         165, Mapper165_Init         )
	INES_BOARD( "SUBOR Rev. A",             166, Mapper166_Init         )
	INES_BOARD( "SUBOR Rev. B",             167, Mapper167_Init         )
	INES_BOARD( "",                         168, Mapper168_Init         )
/*    INES_BOARD( "",                            169, Mapper169_Init ) */
	INES_BOARD( "",                         170, Mapper170_Init         )
	INES_BOARD( "Kaiser 7058",              171, Mapper171_Init         )
	INES_BOARD( "Super Mega P-4070",        172, Mapper172_Init         )
	INES_BOARD( "Idea-Tek ET.xx",           173, Mapper173_Init         )
    INES_BOARD( "NTDec 5-in-1",             174, Mapper174_Init         )
	INES_BOARD( "",                         175, Mapper175_Init         )
	INES_BOARD( "BMCFK23C",                 176, Mapper176_Init         )
	INES_BOARD( "Hénggé Diànzǐ",            177, Mapper177_Init         )
	INES_BOARD( "FS305/NJ0430",             178, Mapper178_Init         )
/*    INES_BOARD( "",                            179, Mapper179_Init ) */
	INES_BOARD( "",                         180, Mapper180_Init         )
/*	INES_BOARD( "",                         181, Mapper181_Init         ) */ /* fceux' exclusive mapper to handle Seicross V2, now moved to Mapper 185,sub 4 */
/*    INES_BOARD( "",                            182, Mapper182_Init ) */    /* Deprecated, dupe of Mapper 114 */
	INES_BOARD( "",                         183, Mapper183_Init         )
	INES_BOARD( "",                         184, Mapper184_Init         )
	INES_BOARD( "CNROM+CopyProtection",     185, Mapper185_Init         )
	INES_BOARD( "",                         186, Mapper186_Init         )
	INES_BOARD( "",                         187, Mapper187_Init         )
	INES_BOARD( "",                         188, Mapper188_Init         )
	INES_BOARD( "",                         189, Mapper189_Init         )
	INES_BOARD( "",                         190, Mapper190_Init         )
	INES_BOARD( "",                         191, Mapper191_Init         )
	INES_BOARD( "TW MMC3+VRAM Rev. B",      192, Mapper192_Init         )
	INES_BOARD( "NTDEC TC-112",             193, Mapper193_Init         ) /* War in the Gulf */
	INES_BOARD( "TW MMC3+VRAM Rev. C",      194, Mapper194_Init         )
	INES_BOARD( "TW MMC3+VRAM Rev. D",      195, Mapper195_Init         )
	INES_BOARD( "",                         196, Mapper196_Init         )
	INES_BOARD( "",                         197, Mapper197_Init         )
	INES_BOARD( "TW MMC3+VRAM Rev. E",      198, Mapper198_Init         )
	INES_BOARD( "",                         199, Mapper199_Init         )
	INES_BOARD( "",                         200, Mapper200_Init         )
	INES_BOARD( "21-in-1",                  201, Mapper201_Init         )
	INES_BOARD( "",                         202, Mapper202_Init         )
	INES_BOARD( "",                         203, Mapper203_Init         )
	INES_BOARD( "",                         204, Mapper204_Init         )
	INES_BOARD( "BMC 15-in-1/3-in-1",       205, Mapper205_Init         )
	INES_BOARD( "NAMCOT 108 Rev. C",        206, Mapper206_Init         ) /* Deprecated, Used to be "DEIROM" whatever it means, but actually simple version of MMC3 */
	INES_BOARD( "TAITO X1-005 Rev. B",      207, Mapper207_Init         )
	INES_BOARD( "",                         208, Mapper208_Init         )
	INES_BOARD( "HUMMER/JY BOARD",          209, Mapper209_Init         )
	INES_BOARD( "",                         210, Mapper210_Init         )
	INES_BOARD( "HUMMER/JY BOARD",          211, Mapper211_Init         )
	INES_BOARD( "",                         212, Mapper212_Init         )
	INES_BOARD( "",                         213, Mapper058_Init         ) /* in mapper 58 */
	INES_BOARD( "",                         214, Mapper214_Init         )
	INES_BOARD( "UNL-8237",                 215, Mapper215_Init         )
	INES_BOARD( "Bonza",                    216, Mapper216_Init         )
	INES_BOARD( "",                         217, Mapper217_Init         ) /* Redefined to a new Discrete BMC mapper */
	INES_BOARD( "Magic Floor",              218, Mapper218_Init         )
	INES_BOARD( "A9746",                    219, Mapper219_Init         )
/*	INES_BOARD( "Debug Mapper",             220, Mapper220_Init         ) */
	INES_BOARD( "UNLN625092",               221, Mapper221_Init         )
	INES_BOARD( "",                         222, Mapper222_Init         )
/*    INES_BOARD( "",                            223, Mapper223_Init ) */
	INES_BOARD( "KT-008",                   224, MINDKIDS_Init          ) /* The KT-008 board contains the MINDKIDS chipset */
	INES_BOARD( "",                         225, Mapper225_Init         )
	INES_BOARD( "BMC 22+20-in-1",           226, Mapper226_Init         )
	INES_BOARD( "",                         227, Mapper227_Init         )
	INES_BOARD( "",                         228, Mapper228_Init         )
	INES_BOARD( "",                         229, Mapper229_Init         )
	INES_BOARD( "BMC Contra+22-in-1",       230, Mapper230_Init         )
	INES_BOARD( "20-in-1",                  231, Mapper231_Init         )
	INES_BOARD( "BMC QUATTRO",              232, Mapper232_Init         )
	INES_BOARD( "BMC 22+20-in-1 RST",       233, Mapper233_Init         )
	INES_BOARD( "BMC MAXI",                 234, Mapper234_Init         )
	INES_BOARD( "Golden Game",              235, Mapper235_Init         )
	INES_BOARD( "Realtec 8031/8155/8099/8106", 236, Mapper236_Init      )
	INES_BOARD( "Teletubbies / Y2K",        237, Mapper237_Init         )
	INES_BOARD( "UNL6035052",               238, Mapper238_Init         )
/*    INES_BOARD( "",                            239, Mapper239_Init ) */
	INES_BOARD( "",                         240, Mapper240_Init         )
	INES_BOARD( "BxROM+WRAM",               241, Mapper241_Init         )
	INES_BOARD( "43272",                    242, Mapper242_Init         )
	INES_BOARD( "SA-020A",                  243, Mapper150_Init         )
	INES_BOARD( "DECATHLON",                244, Mapper244_Init         )
	INES_BOARD( "",                         245, Mapper245_Init         )
	INES_BOARD( "FONG SHEN BANG",           246, Mapper246_Init         )
/*    INES_BOARD( "",                            247, Mapper247_Init ) */
/*    INES_BOARD( "",                            248, Mapper248_Init ) */
	INES_BOARD( "",                         249, Mapper249_Init         )
	INES_BOARD( "",                         250, Mapper250_Init         )
/*    INES_BOARD( "",                            251, Mapper251_Init ) */ /* No good dumps for this mapper, use UNIF version */
	INES_BOARD( "SAN GUO ZHI PIRATE",       252, Mapper252_Init         )
	INES_BOARD( "DRAGON BALL PIRATE",       253, Mapper252_Init         )
	INES_BOARD( "",                         254, Mapper254_Init         )
	INES_BOARD( "",                         255, Mapper255_Init         ) /* Duplicate of M225? */

	/* NES 2.0 MAPPERS */

	INES_BOARD( "OneBus",                   256, Mapper256_Init         )
	INES_BOARD( "158B",                     258, Mapper215_Init         )
	INES_BOARD( "F-15",                     259, Mapper259_Init         )
	INES_BOARD( "HPxx / HP2018-A",          260, Mapper260_Init         )
	INES_BOARD( "810544-C-A1",              261, Mapper261_Init         )
	INES_BOARD( "SHERO",                    262, Mapper262_Init         )
	INES_BOARD( "KOF97",                    263, Mapper263_Init         )
	INES_BOARD( "YOKO",                     264, Mapper264_Init         )
	INES_BOARD( "T-262",                    265, Mapper265_Init         )
	INES_BOARD( "CITYFIGHT",                266, Mapper266_Init         )
	INES_BOARD( "8-in-1 JY-119",            267, Mapper267_Init         )
	INES_BOARD( "COOLBOY/MINDKIDS",         268, Mapper268_Init         ) /* Submapper distinguishes between COOLBOY and MINDKIDS */
	INES_BOARD( "Games Xplosion 121-in-1",  269, Mapper269_Init         )
	INES_BOARD( "MGC-026",                  271, Mapper271_Init         )
	INES_BOARD( "Akumajō Special: Boku Dracula-kun", 272, Mapper272_Init )
	INES_BOARD( "80013-B",                  274, Mapper274_Init         )
	INES_BOARD( "",                         277, Mapper277_Init         )
	INES_BOARD( "YY860417C",                281, Mapper281_Init         )
	INES_BOARD( "860224C",                  282, Mapper282_Init         )
	INES_BOARD( "GS-2004/GS-2013",          283, Mapper283_Init         )
	INES_BOARD( "A65AS",                    285, Mapper285_Init         )
	INES_BOARD( "BS-5",                     286, Mapper286_Init         )
	INES_BOARD( "411120-C, 811120-C",       287, Mapper287_Init         )
	INES_BOARD( "GKCX1",                    288, Mapper288_Init         )
	INES_BOARD( "60311C",                   289, Mapper289_Init         )
	INES_BOARD( "NTD-03",                   290, Mapper290_Init         )
	INES_BOARD( "Kasheng 2-in-1 ",          291, Mapper291_Init         )
	INES_BOARD( "BMW8544",                  292, Mapper292_Init         )
	INES_BOARD( "NewStar 12-in-1/7-in-1",   293, Mapper293_Init         )
	INES_BOARD( "63-1601 ",                 294, Mapper294_Init         )
	INES_BOARD( "YY860216C",                295, Mapper295_Init         )
	INES_BOARD( "TXC 01-22110-000",         297, Mapper297_Init         )
	INES_BOARD( "TF1201",                   298, Mapper298_Init         )
	INES_BOARD( "11160",                    299, Mapper299_Init         )
	INES_BOARD( "190in1",                   300, Mapper300_Init         )
	INES_BOARD( "8157",                     301, Mapper301_Init         )
	INES_BOARD( "KS7057",                   302, Mapper302_Init         )
	INES_BOARD( "KS7017",                   303, Mapper303_Init         )
	INES_BOARD( "SMB2J",                    304, Mapper304_Init         )
	INES_BOARD( "KS7031",                   305, Mapper305_Init         )
	INES_BOARD( "KS7016",                   306, Mapper306_Init         )
	INES_BOARD( "KS7037",                   307, Mapper307_Init         )
	INES_BOARD( "TH2131-1",                 308, Mapper308_Init         )
	INES_BOARD( "LH51",                     309, Mapper309_Init         )
	INES_BOARD( "K-1053",                   310, Mapper310_Init         )
	INES_BOARD( "KS7013B",                  312, Mapper312_Init         )
	INES_BOARD( "RESET-TXROM",              313, Mapper313_Init         )
	INES_BOARD( "64in1NoRepeat",            314, Mapper314_Init         )
	INES_BOARD( "830134C",                  315, Mapper315_Init         )
	INES_BOARD( "HP898F",                   319, Mapper319_Init         )
	INES_BOARD( "830425C-4391T",            320, Mapper320_Init         )
	INES_BOARD( "K-3033",                   322, Mapper322_Init         )
	INES_BOARD( "FARID_SLROM_8-IN-1",       323, Mapper323_Init         )
	INES_BOARD( "FARID_UNROM_8-IN-1",       324, Mapper324_Init         )
	INES_BOARD( "MALISB",                   325, Mapper325_Init         )
	INES_BOARD( "Contra/Gryzor",            326, Mapper326_Init         )
	INES_BOARD( "10-24-C-A1",               327, Mapper327_Init         )
	INES_BOARD( "RT-01",                    328, Mapper328_Init         )
	INES_BOARD( "EDU2000",                  329, Mapper329_Init         )
	INES_BOARD( "Sangokushi II: Haō no Tairiku", 330, Mapper330_Init    )
	INES_BOARD( "12-IN-1",                  331, Mapper331_Init         )
	INES_BOARD( "WS",                       332, Mapper332_Init         )
	INES_BOARD( "NEWSTAR-GRM070-8IN1",      333, Mapper333_Init         )
	INES_BOARD( "821202C",                  334, Mapper334_Init         )
	INES_BOARD( "CTC-09",                   335, Mapper335_Init         )
	INES_BOARD( "K-3046",                   336, Mapper336_Init         )
	INES_BOARD( "CTC-12IN1",                337, Mapper337_Init         )
	INES_BOARD( "SA005-A",                  338, Mapper338_Init         )
	INES_BOARD( "K-3006",                   339, Mapper339_Init         )
	INES_BOARD( "K-3036",                   340, Mapper340_Init         )
	INES_BOARD( "TJ-03",                    341, Mapper341_Init         )
	INES_BOARD( "COOLGIRL",                 342, COOLGIRL_Init          )
	INES_BOARD( "RESETNROM-XIN1",           343, Mapper343_Init         )
	INES_BOARD( "GN-26",                    344, Mapper344_Init         )
	INES_BOARD( "L6IN1",                    345, Mapper345_Init         )
	INES_BOARD( "KS7012",                   346, Mapper346_Init         )
	INES_BOARD( "KS7030",                   347, Mapper347_Init         )
	INES_BOARD( "830118C",                  348, Mapper348_Init         )
	INES_BOARD( "G-146",                    349, Mapper349_Init         )
	INES_BOARD( "891227",                   350, Mapper350_Init         )
	INES_BOARD( "Techline XB",              351, Mapper351_Init         )
	INES_BOARD( "KS106C",                   352, Mapper352_Init         )
	INES_BOARD( "Super Mario Family",       353, Mapper353_Init         )
	INES_BOARD( "FAM250/810139C/810331C/SCHI-24", 354, Mapper354_Init   )
	INES_BOARD( "3D-BLOCK",                 355, Mapper355_Init        )
	INES_BOARD( "7-in-1 Rockman (JY-208)",  356, Mapper356_Init         )
	INES_BOARD( "Bit Corp 4-in-1",          357, Mapper357_Init         )
	INES_BOARD( "YY860606C",                358, Mapper358_Init         )
	INES_BOARD( "SB-5013/GCL8050/841242C",  359, Mapper359_Init         )
	INES_BOARD( "Bitcorp 31-in-1",          360, Mapper360_Init         )
	INES_BOARD( "YY841101C (OK-411)",       361, Mapper361_Init         )
	INES_BOARD( "830506C",                  362, Mapper362_Init         )
	INES_BOARD( "JY830832C",                364, Mapper364_Init         )
	INES_BOARD( "GN-45",                    366, Mapper366_Init         )
	INES_BOARD( "Yung-08",                  368, Mapper368_Init         )
	INES_BOARD( "N49C-300",                 369, Mapper369_Init         )
	INES_BOARD( "Golden Mario Party II - Around the World 6-in-1", 370, Mapper370_Init )
	INES_BOARD( "MMC3 PIRATE SFC-12",       372, Mapper372_Init         )
	INES_BOARD( "95/96 Super HiK 4-in-1",   374, Mapper374_Init         )
	INES_BOARD( "135-in-1",                 375, Mapper375_Init         )
	INES_BOARD( "YY841155C",                376, Mapper376_Init         )
	INES_BOARD( "JY-111/JY-112",            377, Mapper377_Init         )
	INES_BOARD( "42 to 80,000 (970630C)",   380, Mapper380_Init         )
	INES_BOARD( "KN-42",                    381, Mapper381_Init         )
	INES_BOARD( "830928C",                  382, Mapper382_Init         )
	INES_BOARD( "YY840708C",                383, Mapper383_Init         )
	INES_BOARD( "NTDEC 2779",               385, Mapper385_Init         )
	INES_BOARD( "YY860729C",                386, Mapper386_Init         )
	INES_BOARD( "YY850735C",                387, Mapper387_Init         )
	INES_BOARD( "YY850835C",                388, Mapper388_Init         )
	INES_BOARD( "Caltron 9-in-1",           389, Mapper389_Init         )
	INES_BOARD( "Realtec 8031",             390, Mapper390_Init         )
	INES_BOARD( "BS-110",                   391, Mapper391_Init         )
	INES_BOARD( "820720C",                  393, Mapper393_Init         )
	INES_BOARD( "HSK007",                   394, Mapper394_Init         )
	INES_BOARD( "Realtec 8210",             395, Mapper395_Init         )
	INES_BOARD( "YY850437C",                396, Mapper396_Init         )
	INES_BOARD( "YY850439C",                397, Mapper397_Init         )
	INES_BOARD( "YY840820C",                398, Mapper398_Init         )
	INES_BOARD( "Star Versus",              399, Mapper399_Init         )
	INES_BOARD( "8-BIT XMAS",               400, Mapper400_Init         )
	INES_BOARD( "BMC Super 19-in-1 (VIP19)",401, Mapper401_Init         )
	INES_BOARD( "J-2282",                   402, Mapper402_Init         )
	INES_BOARD( "89433",                    403, Mapper403_Init         )
	INES_BOARD( "JY012005",                 404, Mapper404_Init         )
	INES_BOARD( "Haradius Zero",            406, Mapper406_Init         )
	INES_BOARD( "retroUSB DPCMcart",        409, Mapper409_Init         )
	INES_BOARD( "JY-302",                   410, Mapper410_Init         )
	INES_BOARD( "A88S-1",                   411, Mapper411_Init         )
	INES_BOARD( "Intellivision 10-in-1 PnP 2nd Ed.", 412, Mapper412_Init )
	INES_BOARD( "Super Russian Roulette",   413, Mapper413_Init         )
	INES_BOARD( "9999999-in-1",             414, Mapper414_Init         )
	INES_BOARD( "0353",                     415, Mapper415_Init         )
	INES_BOARD( "4-in-1/N-32",              416, Mapper416_Init         )
	INES_BOARD( "",                         417, Mapper417_Init         )
	INES_BOARD( "820106-C/821007C/LH42",    418, Mapper418_Init         )
	INES_BOARD( "A971210",                  420, Mapper420_Init         )
	INES_BOARD( "SC871115C",                421, Mapper421_Init         )
	INES_BOARD( "BS-400R/BS-4040",          422, Mapper422_Init         )
	INES_BOARD( "AB-G1L/WELL-NO-DG450",     428, Mapper428_Init         )
	INES_BOARD( "LIKO BBG-235-8-1B",        429, Mapper429_Init         )
	INES_BOARD( "831031C/T-308",            430, Mapper430_Init         )
	INES_BOARD( "Realtek GN-91B",           431, Mapper431_Init         )
	INES_BOARD( "Realtec 8090",             432, Mapper432_Init         )
	INES_BOARD( "NC-20MB",                  433, Mapper433_Init         )
	INES_BOARD( "S-009",                    434, Mapper434_Init         )
	INES_BOARD( "F-1002",                   435, Mapper435_Init         )
	INES_BOARD( "820401/T-217",             436, Mapper436_Init         )
	INES_BOARD( "NTDEC TH2348",             437, Mapper437_Init         )
	INES_BOARD( "K-3071",                   438, Mapper438_Init         )
	INES_BOARD( "YS2309",                   439, Mapper439_Init         )
	INES_BOARD( "841026C/850335C ",         441, Mapper441_Init         )
	INES_BOARD( "NC-3000M",                 443, Mapper443_Init         )
	INES_BOARD( "NC-7000M/NC-8000M",        444, Mapper444_Init         )
	INES_BOARD( "DG574B",                   445, Mapper445_Init         )
	INES_BOARD( "SMD172B_FPGA",             446, Mapper446_Init         )
	INES_BOARD( "KL-06 multicart",          447, Mapper447_Init         )
	INES_BOARD( "830768C",                  448, Mapper448_Init         )
	INES_BOARD( "Super Games King",         449, Mapper449_Init         )
	INES_BOARD( "YY841157C",                450, Mapper450_Init         )
	INES_BOARD( "Haratyler HP/MP",          451, Mapper451_Init         )
	INES_BOARD( "DS-9-27",                  452, Mapper452_Init         )
	INES_BOARD( "Realtec 8042",             453, Mapper453_Init         )
	INES_BOARD( "100-in-1",                 454, Mapper454_Init         )
	INES_BOARD( "N625836",                  455, Mapper455_Init         )
	INES_BOARD( "K6C3001A",                 456, Mapper456_Init         )
	INES_BOARD( "810431C",                  457, Mapper457_Init         )
	INES_BOARD( "",                         458, Mapper458_Init         )
	INES_BOARD( "8-in-1",                   459, Mapper459_Init         )
	INES_BOARD( "FC-29-40/K-3101",        	460, Mapper460_Init         )
	INES_BOARD( "0324",                 	461, Mapper461_Init         )
	INES_BOARD( "YH810X1",                 	463, Mapper463_Init         )
	INES_BOARD( "NTDEC 9012",            	464, Mapper464_Init         )
	INES_BOARD( "ET-120",                 	465, Mapper465_Init         )
	INES_BOARD( "Keybyte Computer",        	466, Mapper466_Init         )
	INES_BOARD( "47-2",                 	467, Mapper467_Init         )
	INES_BOARD( "Impact Soft IM1",         	471, Mapper471_Init         )
	INES_BOARD( "Yhc-000",                  500, Mapper500_Init         )
	INES_BOARD( "Yhc-001",                  501, Mapper501_Init         )
	INES_BOARD( "Yhc-002",                  502, Mapper502_Init         )
	INES_BOARD( "",                         512, Mapper512_Init         )
	INES_BOARD( "SA-9602B",                 513, Mapper513_Init         )
	INES_BOARD( "Subor Karaoke",            514, Mapper514_Init         )
	INES_BOARD( "Brilliant Com Cocoma Pack", 516, Mapper516_Init        )
	INES_BOARD( "Kkachi-wa Nolae Chingu",   517, Mapper517_Init         ) /* Korean Karaoke */
	INES_BOARD( "DANCE2000",                518, Mapper518_Init         )
	INES_BOARD( "EH8813A",                  519, Mapper519_Init         )
	INES_BOARD( "Datach DBZ/Yu Yu Hakusho", 520, Mapper520_Init         )
	INES_BOARD( "DREAMTECH01",              521, Mapper521_Init         )
	INES_BOARD( "LH10",                     522, Mapper522_Init         )
	INES_BOARD( "Jncota KT-???",            523, Mapper523_Init         )
	INES_BOARD( "900218",                   524, Mapper524_Init         )
	INES_BOARD( "KS7021A",                  525, Mapper525_Init         )
	INES_BOARD( "BJ-56",                    526, Mapper526_Init         )
	INES_BOARD( "AX-40G",                   527, Mapper527_Init         )
	INES_BOARD( "831128C",                  528, Mapper528_Init         )
	INES_BOARD( "YY0807/J-2148/T-230",      529, Mapper529_Init         )
	INES_BOARD( "AX5705",                   530, Mapper530_Init         )
	INES_BOARD( "Sachen 3014",              533, Mapper533_Init         )
	INES_BOARD( "NJ064",                    534, Mapper534_Init         )
	INES_BOARD( "LH53",                     535, Mapper535_Init         )
	INES_BOARD( "60-1064-16L (FDS)",        538, Mapper538_Init         )
	INES_BOARD( "Kid Ikarus (FDS)",         539, Mapper539_Init         )
	INES_BOARD( "82112C",                   540, Mapper540_Init         )
	INES_BOARD( "LittleCom 160-in-1",       541, Mapper541_Init         )
	INES_BOARD( "5-in-1 (CH-501)",          543, Mapper543_Init         )
	INES_BOARD( "Waixing FS306",            544, Mapper544_Init         )
	INES_BOARD( "KONAMI-QTAI",              547, Mapper547_Init         )
	INES_BOARD( "CTC-15",                   548, Mapper548_Init         )
	INES_BOARD( "KS-701B (Kaiser FDS)",     549, Mapper549_Init         )
	INES_BOARD( "",                         551, Mapper178_Init         )
	INES_BOARD( "TAITO X1-017",             552, Mapper552_Init         )
	INES_BOARD( "SACHEN 3013",              553, Mapper553_Init         )
	INES_BOARD( "KS-7010",                  554, Mapper554_Init         )
	INES_BOARD( "JY-215",                   556, Mapper556_Init         )
	INES_BOARD( "JY820845C",                550, Mapper550_Init         )
	INES_BOARD( "NES-EVENT2",               555, Mapper555_Init         )
	INES_BOARD( "",                         557, Mapper557_Init         )
	INES_BOARD( "YC-03-09",                 558, Mapper558_Init         )
	INES_BOARD( "Subor 0102",               559, Mapper559_Init         )
INES_BOARD_END()

#define NES_HEADER_SIZE 16
#define NES_TRAINER_SIZE 512

int iNESLoad(const char *name, FCEUFILE *fp) {
	iNES_HEADER header;

	struct md5_context md5;
	uint64 partialmd5 = 0;

	char *mappername  = NULL;
	uint32 mappertest = 0;

	uint64 filesize = FCEU_fgetsize(fp) - NES_HEADER_SIZE; /* size of file excluding header */
	uint64 romSize  = 0; /* size of PRG + CHR rom */

	int x;

	FCEU_fseek(fp, 0, SEEK_SET);
	if (FCEU_fread(&header, 1, 16, fp) != 16)
		return 0;

	if (memcmp(&header, "NES\x1a", 4)) {
		FCEU_PrintError("Not an iNES file!\n");
		return 0;
	}

	memset(&iNESCart, 0, sizeof(iNESCart));
	memset(&vsuni_system, 0, sizeof(vsuni_system));

	if (!memcmp((char *)(&header) + 0x7, "DiskDude", 8)) {
		memset((char *)(&header) + 0x7, 0, 0x9);
	} else if (!memcmp((char *)(&header) + 0x7, "demiforce", 9)) {
		memset((char *)(&header) + 0x7, 0, 0x9);
	} else if (!memcmp((char *)(&header) + 0xA, "Ni03", 4)) {
		memset((char *)(&header) + 0x7, 0, 0x9);
	}

	iNESCart.iNES2      = (header.ROM_type2 & 0x0C) == 0x08;
	ROM.prg.size        = header.ROM_size;
	ROM.chr.size        = header.VROM_size;
	iNESCart.PRGRomSize = ROM.prg.size * 0x4000;
	iNESCart.CHRRomSize = ROM.chr.size * 0x2000;
	iNESCart.mapper     = (header.ROM_type2 & 0xF0) | (header.ROM_type >> 4);
	if (header.ROM_type & 8) {
		iNESCart.mirror = MI_4;
	} else if (header.ROM_type & 1) {
		iNESCart.mirror = MI_V;
	} else {
		iNESCart.mirror = MI_H;
	}
	/* special mirroring case for mapper 30/218 */
	iNESCart.mirror2bits = ((header.ROM_type & 0x08) ? 2 : 0) | (header.ROM_type & 1);
	iNESCart.battery     = (header.ROM_type & 0x02) != 0;
	iNESCart.trainer     = (header.ROM_type & 0x04) != 0;
	iNESCart.ConsoleType = header.ROM_type2 & 0x03;
	if (iNESCart.iNES2) {
		uint8 value;

		iNESCart.mapper |= ((header.ROM_type3 & 0x0F) << 8);
		iNESCart.submapper = (header.ROM_type3 >> 4);

		value = (header.upper_PRG_CHR_size >> 0) & 0x0F;
		if (value < 0x0F) {
			ROM.prg.size       |= (value << 8);
			iNESCart.PRGRomSize = ROM.prg.size * 0x4000;
		} else {
			iNESCart.PRGRomSize = pow(2, header.ROM_size >> 2) * ((header.ROM_size & 0x03) * 2 + 1);
			ROM.prg.size        = (iNESCart.PRGRomSize / 0x4000) + ((iNESCart.PRGRomSize % 0x4000) ? 1 : 0);
		}

		value = (header.upper_PRG_CHR_size >> 4) & 0x0F;
		if (value < 0x0F) {
			ROM.chr.size       |= (value << 8);
			iNESCart.CHRRomSize = ROM.chr.size * 0x2000;
		} else {
			iNESCart.CHRRomSize = pow(2, header.VROM_size >> 2) * ((header.VROM_size & 0x03) * 2 + 1);
			ROM.chr.size        = (iNESCart.CHRRomSize / 0x2000) + ((iNESCart.CHRRomSize % 0x2000) ? 1 : 0);
		}

		value = (header.PRGRAM_size >> 0) & 0x0F;
		if (value) {
			iNESCart.PRGRamSize = 64 << value;
		}

		value = (header.PRGRAM_size >> 4) & 0x0F;
		if (value) {
			iNESCart.PRGRamSaveSize = 64 << value;
		}

		value = (header.CHRRAM_size >> 0) & 0x0F;
		if (value) {
			iNESCart.CHRRamSize = 64 << value;
		}

		value = (header.CHRRAM_size >> 4) & 0x0F;
		if (value) {
			iNESCart.CHRRamSaveSize = 64 << value;
		}
		if ((header.PRGRAM_size & 0xF0) && !(header.ROM_type & 0x02)) {
			 /* set battery bit in header when upper nibble of PRG RAM is non zero */
			header.ROM_type |= 0x02;
			iNESCart.battery = true;
		}
		if (header.Region == 3) {
			iNESCart.region = DENDY;
		} else if (header.Region == 1) {
			iNESCart.region = NES_PAL;
		} else {
			/* 0: Nes Ntsc, 1: Nes Pal */
			iNESCart.region = NES_NTSC;
		}
		if (iNESCart.ConsoleType == 3) {
			iNESCart.ConsoleType = header.VS_hardware & 0x0F;
		} else if (iNESCart.ConsoleType == 1) {
			iNESCart.VS_PPUTypes = header.VS_hardware & 0x0F;
			iNESCart.VS_HWType   = header.VS_hardware >> 4;
		}
		iNESCart.MiscRoms = header.MiscRoms & 0x03;
		iNESCart.InputTypes = header.ExpDevice;
	} else {
		iNESCart.region         = header.upper_PRG_CHR_size & 1;
		iNESCart.submapper      = 0;
		iNESCart.PRGRamSize     = 0;
		iNESCart.PRGRamSaveSize = 0;
		iNESCart.CHRRamSize     = 0;
		iNESCart.CHRRamSaveSize = 0;
		iNESCart.VS_PPUTypes    = 0;
		iNESCart.VS_HWType      = 0;
		iNESCart.MiscRoms       = 0;
		iNESCart.InputTypes     = 0;
		if (!ROM.prg.size) {
			ROM.prg.size = 256;
			iNESCart.PRGRomSize = ROM.prg.size * 0x4000;
		}
	}

	/* Set Vs. System flag if need */
	switch (iNESCart.ConsoleType) {
	case 0: /* standard NES/Dendy */
	case 2: /* Playchoice 10 */
		GameInfo->type = GIT_CART;
		break;
	case 1:
		GameInfo->type = GIT_VSUNI;
		break;
	default:
		FCEU_PrintError(" Game type 0x%02x is not supported. Trying as standard NES.\n", iNESCart.ConsoleType);
		GameInfo->type = GIT_CART;
		break;
	}

	/* Set Vs. System PPU type if need */
	if (GameInfo->type == GIT_VSUNI) {
		switch (iNESCart.VS_PPUTypes) {
		case 0x0: vsuni_system.ppu = PPU_RC2C03; break; /* RP2C03B */
		case 0x1: vsuni_system.ppu = PPU_RC2C03; break; /* RP2C03G */
		case 0x2: vsuni_system.ppu = PPU_RP2C04_0001; break;
		case 0x3: vsuni_system.ppu = PPU_RP2C04_0002; break;
		case 0x4: vsuni_system.ppu = PPU_RP2C04_0003; break;
		case 0x5: vsuni_system.ppu = PPU_RP2C04_0004; break;
		case 0x6: vsuni_system.ppu = PPU_RC2C03; break; /* RC2C03B */
		case 0x7: vsuni_system.ppu = PPU_RC2C03; break; /* RC2C03C */
		case 0x8: vsuni_system.ppu = PPU_RC2C05_01; break;
		case 0x9: vsuni_system.ppu = PPU_RC2C05_02; break;
		case 0xA: vsuni_system.ppu = PPU_RC2C05_03; break;
		case 0xB: vsuni_system.ppu = PPU_RC2C05_04; break;
		case 0xC: vsuni_system.ppu = PPU_RC2C05_05; break;
		default:
			FCEU_PrintError(" Unknown VS System PPU type: 0x%02x\n.", iNESCart.VS_PPUTypes);
			break;
		}

		switch (iNESCart.VS_HWType) {
		case 0x0: vsuni_system.type = VS_TYPE_NORMAL; break;
		case 0x1: vsuni_system.type = VS_TYPE_RBI; break;
		case 0x2: vsuni_system.type = VS_TYPE_TKO; break;
		case 0x3: vsuni_system.type = VS_TYPE_XEVIOUS; break;
		case 0x4: vsuni_system.type = VS_TYPE_ICECLIMBER; break;
		}

		switch (iNESCart.InputTypes) {
		case 0x04: /* 1P connected to $4016 */
			break;
		case 0x05: /* 1P connected to $4017 */
			vsuni_system.ioption = IOPTION_SWAPDIRAB;
			break;
		case 0x07: /* Famicom Zapper */
			vsuni_system.ioption = IOPTION_GUN;
			break;
		}

		if (vsuni_system.ioption & IOPTION_GUN) {
			GameInfo->input[0] = SI_ZAPPER;
			GameInfo->input[1] = SI_NONE;
		} else {
			GameInfo->input[0] = SI_GAMEPAD;
			GameInfo->input[1] = SI_GAMEPAD;
		}
	}

	/* Trainer */
	if (iNESCart.trainer) {
		ROM.trainer.size = NES_TRAINER_SIZE;
		ROM.trainer.data = (uint8 *)FCEU_gmalloc(ROM.trainer.size);
		FCEU_fread(ROM.trainer.data, ROM.trainer.size, 1, fp);
		filesize -= ROM.trainer.size;
	}

	romSize = iNESCart.PRGRomSize + iNESCart.CHRRomSize;
	if (romSize > filesize) {
		FCEU_PrintError( " File length is too short to contain all data reported from header by %llu\n", romSize - filesize);
	} else if (romSize < filesize) {
		if (!iNESCart.MiscRoms) {
			/* in some rare cases where CHR page count is malformed and no , just recompute CHR data from remaining length */
			size_t newCHRSize = filesize - iNESCart.PRGRomSize;
			FCEU_PrintError(" File contains %llu bytes of unused data.\n", filesize - romSize);
			FCEU_PrintError(" CHRROM size is is adjusted for this unused data.\n");
			iNESCart.CHRRomSize = newCHRSize;
		}
	}

	if (!(ROM.prg.data = (uint8 *)FCEU_malloc(uppow2(iNESCart.PRGRomSize)))) {
		Cleanup();
		return 0;
	}
	memset(ROM.prg.data, 0xFF, uppow2(iNESCart.PRGRomSize));
	FCEU_fread(ROM.prg.data, 1, iNESCart.PRGRomSize, fp);

	if (iNESCart.CHRRomSize) {
		if (!(ROM.chr.data = (uint8 *)FCEU_malloc(uppow2(iNESCart.CHRRomSize)))) {
			Cleanup();
			return 0;
		}
		memset(ROM.chr.data, 0xFF, uppow2(iNESCart.CHRRomSize));
		FCEU_fread(ROM.chr.data, 1, iNESCart.CHRRomSize, fp);
	}

	if (iNESCart.MiscRoms) {
		ROM.misc.size = filesize - romSize;
		if (!(ROM.misc.data = (uint8 *)FCEU_malloc(ROM.misc.size))) {
			Cleanup();
			return 0;
		}
		memset(ROM.misc.data, 0xFF, ROM.misc.size);
		FCEU_fread(ROM.misc.data, 1, ROM.misc.size, fp);
	}

	iNESCart.PRGCRC32 = CalcCRC32(0, ROM.prg.data, iNESCart.PRGRomSize);
	iNESCart.CHRCRC32 = CalcCRC32(0, ROM.chr.data, iNESCart.CHRRomSize);
	iNESCart.CRC32    = CalcCRC32(iNESCart.PRGCRC32, ROM.chr.data, iNESCart.CHRRomSize);

	md5_starts(&md5);
	md5_update(&md5, ROM.prg.data, iNESCart.PRGRomSize);
	if (iNESCart.CHRRomSize) {
		md5_update(&md5, ROM.chr.data, iNESCart.CHRRomSize);
	}
	md5_finish(&md5, iNESCart.MD5);
	memcpy(&GameInfo->MD5, &iNESCart.MD5, sizeof(iNESCart.MD5));
	for (x = 0; x < 8; x++) {
		partialmd5 |= (uint64)iNESCart.MD5[7 - x] << (x * 8);
	}

	mappername = "Not Listed";
	for (mappertest = 0; mappertest < (sizeof bmap / sizeof bmap[0]) - 1; mappertest++) {
		if (bmap[mappertest].number == iNESCart.mapper) {
			mappername = (char *)bmap[mappertest].name;
			break;
		}
	}

	if (!iNESCart.iNES2) {
		if (strstr(name, "(E)") || strstr(name, "(e)") || strstr(name, "(Europe)") || strstr(name, "(PAL)") ||
		    strstr(name, "(F)") || strstr(name, "(f)") || strstr(name, "(G)") || strstr(name, "(g)") ||
		    strstr(name, "(I)") || strstr(name, "(i)") || strstr(name, "(S)") || strstr(name, "(s)") ||
		    strstr(name, "(France)") || strstr(name, "(Germany)") || strstr(name, "(Italy)") ||
		    strstr(name, "(Spain)") || strstr(name, "(Sweden)") || strstr(name, "(Sw)") ||
		    strstr(name, "(Australia)") || strstr(name, "(A)") || strstr(name, "(a)")) {
			iNESCart.region = NES_PAL;
		}
	}

	if (iNESCart.iNES2) FCEU_printf(" NES 2.0 extended iNES.\n");
	FCEU_printf(" PRG-ROM CRC32: 0x%08X\n", iNESCart.PRGCRC32);
	if (iNESCart.PRGCRC32 != iNESCart.CRC32) {
		FCEU_printf(" PRG+CHR CRC32: 0x%08X\n", iNESCart.CRC32);
	}
	FCEU_printf(" PRG+CHR MD5:   0x%s\n", md5_asciistr(iNESCart.MD5));
	FCEU_printf(" PRG ROM:       %-4d x 16 KiB = %-5d KiB\n", ROM.prg.size, iNESCart.PRGRomSize >> 10);
	FCEU_printf(" CHR ROM:       %-4d x  8 KiB = %-5d KiB\n", ROM.chr.size, iNESCart.CHRRomSize >> 10);
	FCEU_printf(" Mapper #:      %-6d\n", iNESCart.mapper);
	FCEU_printf(" Mapper name:   %s\n", mappername);
	FCEU_printf(" Mirroring:     %s\n", mirroring_str[iNESCart.mirror]);
	FCEU_printf(" Trainer:       %s\n", iNESCart.trainer ? "Yes" : "No");
	FCEU_printf(" Battery:       %s\n", iNESCart.battery ? "Yes" : "No");
	FCEU_printf(" System:        %s\n", tv_region_str[iNESCart.region]);
	FCEU_printf(" Console:       %s\n", console_types_str[iNESCart.ConsoleType & 0x0F].name);
	if (iNESCart.iNES2) {
		size_t PRGRAM = iNESCart.PRGRamSize + iNESCart.PRGRamSaveSize;
		size_t CHRRAM = iNESCart.CHRRamSize + iNESCart.CHRRamSaveSize;
		if (iNESCart.InputTypes) {
			FCEU_printf(" Input:         %s\n", input_type_str[iNESCart.InputTypes].name);
		}
		FCEU_printf(" Submapper:    %2d\n", iNESCart.submapper);
		if (PRGRAM || CHRRAM) {
			if (iNESCart.battery) {
				FCEU_printf(" PRG RAM:       %-3d KiB ( %2d KiB battery-backed)\n", PRGRAM / 1024, iNESCart.PRGRamSaveSize / 1024);
				FCEU_printf(" CHR RAM:       %-3d KiB ( %2d KiB battery-backed)\n", CHRRAM / 1024, iNESCart.CHRRamSaveSize / 1024);
			} else {
				FCEU_printf(" PRG RAM:       %-3d KiB\n", PRGRAM / 1024);
				FCEU_printf(" CHR RAM:       %-3d KiB\n", CHRRAM / 1024);
			}
		}
		if (ROM.misc.size) {
			FCEU_printf(" MISC-ROM:      %-6d KiB\n", ROM.misc.size >> 10);
		}
	}
	FCEU_printf(" File:          %s\n", path_remove_extension((char *)path_basename(name)));

	if (iNESCart.VS_HWType >= 5) {
		FCEUD_DispMessage(RETRO_LOG_ERROR, 2000, "Vs. Dual System is unsupported.\n");
		FCEU_PrintError( " Vs. Dual System is unsupported.\n");
	}
	if (iNESCart.ConsoleType > 0x02) {
		FCEU_PrintError(" Unsupported type (%02x, \"%s\").\n", iNESCart.ConsoleType, console_types_str[iNESCart.ConsoleType & 0x0F].name);
	}

	ResetCartMapping();
	ResetExState(0, 0);

	SetupCartPRGMapping(0, ROM.prg.data, uppow2(iNESCart.PRGRomSize), 0);

	SetInput(&iNESCart);
	if (iNESCart.iNES2) SetInputNes20(iNESCart.InputTypes);
	CheckHInfo(&iNESCart, partialmd5);
	FCEU_VSUniCheck(partialmd5, &iNESCart.mapper, &iNESCart.mirror);

	/* Must remain here because above functions might change value of
	 * ROM.chr.size and free(ROM.chr.data).
	 */
	if (iNESCart.CHRRomSize) {
		SetupCartCHRMapping(0, ROM.chr.data, uppow2(iNESCart.CHRRomSize), 0);
	}

	setmirror(iNESCart.mirror);

	if (!iNES_Init(iNESCart.mapper)) {
		FCEU_PrintError(" iNES mapper #%d is not supported.\n", iNESCart.mapper);
		Cleanup();
		return 0;
	}

	/* 0: RP2C02 ("NTSC NES")
	 * 1: RP2C07 ("Licensed PAL NES")
	 * 2: Multiple-region
	 * 3: UMC 6527P ("Dendy") */
	if (iNESCart.region == 3)
		isDendy = TRUE;

	FCEUI_SetVidSystem((iNESCart.region == 1) ? 1 : 0);

	GameInterface = iNESGI;

	return 1;
}

static int iNES_Init(int num) {
	BMAPPINGLocal *tmp = bmap;

	CHRRAMSize = -1;

	if (GameInfo->type == GIT_VSUNI)
		AddExState(FCEUVSUNI_STATEINFO, ~0, 0, 0);

	while (tmp->init) {
		if (num == tmp->number) {
			UNIFchrrama = 0; /* need here for compatibility with UNIF mapper code */
			if (!ROM.chr.size) {
				if (iNESCart.iNES2) {
					CHRRAMSize = iNESCart.CHRRamSize + iNESCart.CHRRamSaveSize;
					/* in NES 2.0 headered, its possible to have CHR ROM/RAM at zero */
					/* if (CHRRAMSize == 0)
						CHRRAMSize = iNESCart.CHRRamSize = 8 * 8192; */
				} else {
					switch (num) { /* FIXME, mapper or game data base with the board parameters and ROM/RAM sizes */
					case 13:
						CHRRAMSize = 16 * 1024;
						break;
					case 6:
					case 28:
					case 29:
					case 30:
					case 45:
					case 96:
					case 111:
						CHRRAMSize = 32 * 1024;
						break;
					case 176:
						CHRRAMSize = 128 * 1024;
						break;
					default:
						CHRRAMSize = 8 * 1024;
						break;
					}
					iNESCart.CHRRamSize = CHRRAMSize;
				}
				if (CHRRAMSize > 0) { /* TODO: CHR-RAM are sometimes handled in mappers e.g. MMC1 using submapper 1/2/4
					                     and CHR-RAM can be zero here */
					if ((ROM.chr.data = (uint8 *)malloc(CHRRAMSize)) == NULL) {
						return 0;
					}
					FCEU_MemoryRand(ROM.chr.data, CHRRAMSize);
					UNIFchrrama = ROM.chr.data;
					SetupCartCHRMapping(0, ROM.chr.data, CHRRAMSize, 1);
					AddExState(ROM.chr.data, CHRRAMSize, 0, "CHRR");
				}
			}
			tmp->init(&iNESCart);
			return 1;
		}
		tmp++;
	}
	return 0;
}
