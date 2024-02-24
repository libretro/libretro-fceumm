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

#include <string.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "input.h"
#include "palette.h"
#include "vsuni.h"
#include "state.h"
#include "cart.h"

typedef struct {
	char *name;
	int gameid;
	uint64 md5partial;
	int mapper;
	int mirroring;
	int ppu;
	int ioption;
	int predip;
	int type;
} VSUNIENTRY;

VSUNISYSTEM vsuni_system;

static uint8 DIPS = 0;

void FCEUI_VSUniToggleDIPView(void) {
	DIPS = !DIPS;
}

void FCEU_VSUniToggleDIP(int w) {
	vsuni_system.vsdip ^= 1 << w;
}

void FCEUI_VSUniSetDIP(int w, int state) {
	if (((vsuni_system.vsdip >> w) & 1) != state)
		FCEUI_VSUniToggleDIP(w);
}

uint8 FCEUI_VSUniGetDIPs(void) {
	return(vsuni_system.vsdip);
}

static uint8 secdata[2][32] = {
	{
		/* TKO Boxing */
		0xff, 0xbf, 0xb7, 0x97, 0x97, 0x17, 0x57, 0x4f,
		0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90, 0x94, 0x14,
		0x56, 0x4e, 0x6f, 0x6b, 0xeb, 0xa9, 0xb1, 0x90,
		0xd4, 0x5c, 0x3e, 0x26, 0x87, 0x83, 0x13, 0x00
	},
	{
		/* RBI Baseball */
		0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00,
		0x00, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x94, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}
};

static uint8 VSindex;

static DECLFR(VSSecRead) {
	if ((vsuni_system.type == VS_TYPE_TKO) || (vsuni_system.type == VS_TYPE_RBI)) {
		if (A == 0x5e00) {
			VSindex = 0;
		} else if (A == 0x5e01) {
			if (vsuni_system.type == VS_TYPE_TKO) {
				return (secdata[0][(VSindex++) & 0x1F]);
			} else if (vsuni_system.type == VS_TYPE_RBI) {
				return (secdata[1][(VSindex++) & 0x1F]);
			}
		}
	} else if (vsuni_system.type == VS_TYPE_XEVIOUS) {
		if (A == 0x54FF) {
			return (0x5);
		} else if (A == 0x5678) {
			return (VSindex ? 0 : 1);
		} else if (A == 0x578F) {
			return (VSindex ? 0xd1 : 0x89);
		} else if (A == 0x5567) {
			VSindex ^= 1;
			return (VSindex ? 0x37 : 0x3E);
		}
	}
	return cpu.openbus;
}

void FCEU_VSUniCoin(int slot) {
	vsuni_system.coinon[slot] = 6;
}

void FCEU_VSUniService(void) {
	vsuni_system.service = 6;
}

static readfunc OldReadPPU;
static writefunc OldWritePPU[2];

static DECLFR(rc2c05_A2002) {
	return((OldReadPPU(A) & ~0x3F) | vsuni_system.rc2c05_A2002);
}

static DECLFW(rc2c05_B2000_2001) {
	A = (A ^ 1);
	OldWritePPU[A & 1](A, V);
}

void FCEU_VSUniSwap(uint8 *j0, uint8 *j1) {
	uint8 t0 = *j0;
	uint8 t1 = *j1;

	if (vsuni_system.ioption & IOPTION_SWAPDIRAB) {
		/* Swap controllers 1 and 2 expect Select/start buttons */
		*j0 = (t0 & 0x0C) | (t1 & 0xF3);
		*j1 = (t1 & 0x0C) | (t0 & 0xF3);
	}

	t0 = *j0;
	t1 = *j1;

	/* swap select and start */
	*j0 = (t0 & 0xF3) | ((t0 & JOY_START) >> 1) | ((t0 & JOY_SELECT) << 1);
	*j1 = (t1 & 0xF3) | ((t1 & JOY_START) >> 1) | ((t1 & JOY_SELECT) << 1);
}

void FCEU_VSUniPower(void) {
	vsuni_system.coinon[0] = 0;
	vsuni_system.coinon[1] = 0;
	vsuni_system.service = 0;

	VSindex = 0;

	switch (vsuni_system.ppu) {
	case PPU_RP2C04_0001:
	case PPU_RP2C04_0002:
	case PPU_RP2C04_0003:
	case PPU_RP2C04_0004:
		palette_nes_selected = vsuni_system.ppu;
		break;
	case PPU_RC2C05_01:
		vsuni_system.rc2c05_enable = TRUE;
		vsuni_system.rc2c05_A2002 = 0x1B;
		break;
	case PPU_RC2C05_02:
		vsuni_system.rc2c05_enable = TRUE;
		vsuni_system.rc2c05_A2002 = 0x3D;
		break;
	case PPU_RC2C05_03:
		vsuni_system.rc2c05_enable = TRUE;
		vsuni_system.rc2c05_A2002 = 0x1C;
		break;
	case PPU_RC2C05_04:
		vsuni_system.rc2c05_enable = TRUE;
		vsuni_system.rc2c05_A2002 = 0x1B;
		break;
	case PPU_RC2C05_05:
		vsuni_system.rc2c05_enable = TRUE;
		vsuni_system.rc2c05_A2002 = 0x00;
		break;
	default:
		vsuni_system.rc2c05_enable = FALSE;
		vsuni_system.rc2c05_A2002 = 0x00;
		break;
	}

	SetReadHandler(0x5000, 0x5FFF, VSSecRead);
	if (vsuni_system.rc2c05_enable) {
		OldReadPPU = GetReadHandler(0x2002);
		SetReadHandler(0x2002, 0x2002, rc2c05_A2002);

		OldWritePPU[0] = GetWriteHandler(0x2000);
		OldWritePPU[1] = GetWriteHandler(0x2001);
		SetWriteHandler(0x2000, 0x2001, rc2c05_B2000_2001);
	}
}

/* Games that will probably not be supported ever(or for a long time), since they require
dual-system:

 Balloon Fight
 VS Mahjong
 VS Tennis
 Wrecking Crew
*/

/* Games/PPU list.  Information copied from MAME.  ROMs are exchangable, so don't take
this list as "this game must use this PPU".

RP2C04-0001:
- Baseball
- Freedom Force
- Gradius
- Hogan's Alley
- Mach Rider (Japan, Fighting Course)
- Pinball
- Platoon
- Super Xevious

RP2C04-0002:
- Castlevania
- Ladies Golf
- Mach Rider (Endurance Course)
- Raid on Bungeling Bay (Japan)
- Slalom
- Stroke N' Match Golf
- Wrecking Crew

RP2C04-0003:
- Dr. Mario
- Excite Bike
- Goonies
- Soccer
- TKO Boxing

RP2C04-0004:
- Clu Clu Land
- Excite Bike (Japan)
- Ice Climber
- Ice Climber Dual (Japan)
- Super Mario Bros.

PPU_RC2C03:
- Battle City
- Duck Hunt
- Mahjang
- Pinball (Japan)
- Rbi Baseball
- Star Luster
- Stroke and Match Golf (Japan)
- Super Skykid
- Tennis
- Tetris

RC2C05-01:
- Ninja Jajamaru Kun (Japan)

RC2C05-02:
- Mighty Bomb Jack (Japan)

RC2C05-03:
- Gumshoe

RC2C05-04:
- Top Gun
*/

VSUNIENTRY VSUniGames[] =
{
	{ "Baseball",                  VS_BASEBALL,           0x691d4200ea42be45LL,  99, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_NORMAL },
	{ "Battle City",               VS_BATTLECITY,         0x8540949d74c4d0ebLL,  99, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_NORMAL },
	{ "Battle City(Bootleg)",      VS_BATTLECITY,         0x8093cbe7137ac031LL,  99, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_NORMAL },

	{ "Clu Clu Land",              VS_CLUCLULAND,         0x1b8123218f62b1eeLL,  99, MI_4, PPU_RP2C04_0004, IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Dr Mario",                  VS_DRMARIO,            0xe1af09c477dc0081LL,   1, MI_4, PPU_RP2C04_0003, IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Duck Hunt",                 VS_DUCKHUNT,           0x47735d1e5f1205bbLL,  99, MI_4, PPU_RC2C03,      IOPTION_GUN, 0, VS_TYPE_NORMAL },
	{ "Excitebike",                VS_EXITEBIKE,          0x3dcd1401bcafde77LL,  99, MI_4, PPU_RP2C04_0003, 0, 0, VS_TYPE_NORMAL },
	{ "Excitebike (J)",            VS_EXITEBIKE,          0x7ea51c9d007375f0LL,  99, MI_4, PPU_RP2C04_0004, 0, 0, VS_TYPE_NORMAL },
	{ "Excitebike (J)",            VS_EXITEBIKE,          0x7bcccfdd8011ba99LL,  99, MI_4, PPU_RP2C04_0003, 0, 0, VS_TYPE_NORMAL }, /* Excite Bike (EB4-3 E). nes added 2023-8-5 */
	{ "Freedom Force",             VS_FREEDOMFORCE,       0xed96436bd1b5e688LL, 206, MI_4, PPU_RP2C04_0001, IOPTION_GUN, 0, VS_TYPE_NORMAL },	/* Wrong color in game select screen? */
	{ "Stroke and Match Golf",     VS_STROKEANDMATCHGOLF, 0x612325606e82bc66LL,  99, MI_4, PPU_RP2C04_0002, IOPTION_SWAPDIRAB | IOPTION_PREDIP, 0x01, VS_TYPE_NORMAL },

	{ "Goonies",                   VS_GOONIES,            0xb4032d694e1d2733LL,  75, MI_4, PPU_RP2C04_0003, 0, 0, VS_TYPE_NORMAL },
	{ "Gradius",                   VS_GRADIUS,            0x50687ae63bdad976LL,  75, MI_4, PPU_RP2C04_0001, IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Gumshoe",                   VS_GUMSHOE,            0x87161f8ee37758d3LL,  99, MI_4, PPU_RC2C05_03,   IOPTION_GUN, 0, VS_TYPE_NORMAL },
	{ "Gumshoe",                   VS_GUMSHOE,            0xb8500780bf69ce29LL,  99, MI_4, PPU_RC2C05_03,   IOPTION_GUN, 0, VS_TYPE_NORMAL },
	{ "Gumshoe",                   VS_GUMSHOE,            0xa6bf132ba11d0a8cLL,  99, MI_4, PPU_RC2C05_03,   IOPTION_GUN, 0, VS_TYPE_NORMAL }, /* Gumshoe (VS).nes added: 2017-9-5 */
	{ "Gumshoe",                   VS_GUMSHOE,            0x53658a1f6d2df78eLL,  99, MI_4, PPU_RC2C05_03,   IOPTION_GUN, 0, VS_TYPE_NORMAL }, /* Gumshoe (VS).nes added: 2023-8-5 */
	{ "Gumshoe",                   VS_GUMSHOE,            0x15f31725e5cbafd4LL,  99, MI_4, PPU_RC2C05_03,   IOPTION_GUN, 0, VS_TYPE_NORMAL }, /* Gumshoe (VS).nes added: 2023-8-5 */
	{ "Hogan's Alley",             VS_HOGANSALLEY,        0xd78b7f0bb621fb45LL,  99, MI_4, PPU_RP2C04_0001, IOPTION_GUN, 0, VS_TYPE_NORMAL },
	{ "Hogan's Alley",             VS_HOGANSALLEY,        0x7566994e3d5ca6a7LL,  99, MI_4, PPU_RP2C04_0001, IOPTION_GUN, 0, VS_TYPE_NORMAL }, /* Hogans Alley.nes added: 2023-8-5 */
	{ "Ice Climber",               VS_ICECLIMBER,         0xd21e999513435e2aLL,  99, MI_4, PPU_RP2C04_0004, IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Ladies Golf",               VS_LADIESGOLF,         0x781b24be57ef6785LL,  99, MI_4, PPU_RP2C04_0002, IOPTION_SWAPDIRAB | IOPTION_PREDIP, 0x1, VS_TYPE_NORMAL },

	{ "Mach Rider",                VS_MACHRIDER,          0x015672618af06441LL,  99, MI_4, PPU_RP2C04_0002, 0, 0, VS_TYPE_NORMAL },
	{ "Mach Rider (J)",            VS_MACHRIDER,          0xa625afb399811a8aLL,  99, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_NORMAL },
	{ "Mighty Bomb Jack",          VS_MIGHTYBOMBJACK,     0xe6a89f4873fac37bLL,  99, MI_4, PPU_RC2C05_02,   0, 0, VS_TYPE_NORMAL }, /* 2023-8-5 switched to mapper 99 instead of mapper 0 */
	{ "Ninja Jajamaru Kun",        VS_JAJAMARU,           0xb26a2c31474099c0LL,  99, MI_4, PPU_RC2C05_01,   IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Pinball",                   VS_PINBALL,            0xc5f49d3de7f2e9b8LL,  99, MI_4, PPU_RP2C04_0001, IOPTION_PREDIP, 0x01, VS_TYPE_NORMAL },
	{ "Pinball (J)",               VS_PINBALL,            0x66ab1a3828cc901cLL,  99, MI_4, PPU_RC2C03,      IOPTION_PREDIP, 0x1, VS_TYPE_NORMAL },
	{ "Platoon",                   VS_PLATOON,            0x160f237351c19f1fLL,  67, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_NORMAL },
	{ "RBI Baseball",              VS_RBIBASEBALL,        0x6a02d345812938afLL, 206, MI_4, PPU_RP2C04_0001, IOPTION_SWAPDIRAB, 0, VS_TYPE_RBI },
	{ "Soccer",                    VS_SOCCER,             0xd4e7a9058780eda3LL,  99, MI_4, PPU_RP2C04_0003, IOPTION_SWAPDIRAB, 0, VS_TYPE_NORMAL },
	{ "Star Luster",               VS_STARLUSTER,         0x8360e134b316d94cLL,  99, MI_4, PPU_RC2C03,      0, 0, VS_TYPE_NORMAL },
	{ "Stroke and Match Golf (J)", VS_STROKEANDMATCHGOLF, 0x869bb83e02509747LL,  99, MI_4, PPU_RC2C03,      IOPTION_SWAPDIRAB | IOPTION_PREDIP, 0x1, VS_TYPE_NORMAL },
	{ "Super Sky Kid",             VS_SUPERSKYKID,        0x78d04c1dd4ec0101LL, 206, MI_4, PPU_RC2C03,      IOPTION_SWAPDIRAB | IOPTION_PREDIP, 0x20, VS_TYPE_NORMAL },
	{ "Super Xevious",             VS_SUPERXEVIOUS,       0x2d396247cf58f9faLL, 206, MI_4, PPU_RP2C04_0001, 0, 0, VS_TYPE_XEVIOUS },

	{ "Tetris",                    VS_TETRIS,             0x531a5e8eea4ce157LL,  99, MI_4, PPU_RC2C03,      IOPTION_PREDIP, 0x20, VS_TYPE_NORMAL },
	{ "Tetris",                    VS_TETRIS,             0x2afaee01608e55cbLL,  99, MI_4, PPU_RC2C03,      IOPTION_PREDIP, 0x20, VS_TYPE_NORMAL },
	{ "Top Gun",                   VS_TOPGUN,             0xf1dea36e6a7b531dLL,   2, MI_4, PPU_RC2C05_04,   0, 0, VS_TYPE_NORMAL },
	{ "VS Castlevania",            VS_CASTLEVANIA,        0x92fd6909c81305b9LL,   2, MI_4, PPU_RP2C04_0002, 0, 0, VS_TYPE_NORMAL },
	{ "VS Slalom",                 VS_SLALOM,             0x4889b5a50a623215LL,   0, MI_4, PPU_RP2C04_0002, 0, 0, VS_TYPE_NORMAL },
	{ "VS Super Mario Bros",       VS_SMB,                0x39d8cfa788e20b6cLL,  99, MI_4, PPU_RP2C04_0004, 0, 0, VS_TYPE_NORMAL },
	{ "VS Super Mario Bros [a1]",  VS_SMB,                0xfc182e5aefbce14dLL,  99, MI_4, PPU_RP2C04_0004, 0, 0, VS_TYPE_NORMAL },
	{ "VS TKO Boxing",             VS_TKOBOXING,          0x6e1ee06171d8ce3aLL, 206, MI_4, PPU_RP2C04_0003, IOPTION_PREDIP, 0x00, VS_TYPE_TKO },
	{ "VS TKO Boxing [a1]",        VS_TKOBOXING,          0xa1c694ce147bc1edLL, 206, MI_4, PPU_RP2C04_0003, IOPTION_PREDIP, 0x00, VS_TYPE_TKO },
	{ 0 }
};

void FCEU_VSUniCheck(uint64 md5partial, int *MapperNo, int *Mirroring) {
	VSUNIENTRY *vs = VSUniGames;

	while (vs->name) {
		if (md5partial == vs->md5partial) {
			static char *mirroring_str[] = {
				"Horizontal",
				"Vertical",
				"$2000",
				"$2400",
				"\"Four-screen\"",
			};
			int tofix = 0;

			GameInfo->type      = GIT_VSUNI;
			GameInfo->cspecial  = SIS_VSUNISYSTEM;
			GameInfo->inputfc   = SIFC_NONE;
			
			if (*MapperNo != vs->mapper) {
				tofix |= 1;
				*MapperNo           = vs->mapper;
			}
			
			if (*Mirroring != vs->mirroring) {
				tofix |= 2;
				*Mirroring = vs->mirroring;
			}

			vsuni_system.gameid = vs->gameid;
			vsuni_system.ppu    = vs->ppu;
			vsuni_system.type   = vs->type;
			vsuni_system.vsdip  = 0;

			if (!vsuni_system.ioption) {
				vsuni_system.ioption = vs->ioption;
			}

			if (vs->ioption & IOPTION_PREDIP) {
				vsuni_system.vsdip = vs->predip;
			}

			if (vs->ioption & IOPTION_GUN) {
				GameInfo->input[0] = SI_ZAPPER;
				GameInfo->input[1] = SI_NONE;
			} else {
				GameInfo->input[0] = SI_GAMEPAD;
				GameInfo->input[1] = SI_GAMEPAD;
			}

			FCEU_printf("\n");
			FCEU_printf(" System: VS-UniSystem\n");
			FCEU_printf(" Name: %s\n", vs->name);
			FCEU_printf("\n");

			if (tofix) {
				FCEU_PrintError(" Incorrect VS-Unisystem header information!\n");
				if (tofix & 1)
					FCEU_PrintError(" Mapper:    %d\n", vs->mapper);
				if (tofix & 2)
					FCEU_PrintError(" Mirroring: %s\n", mirroring_str[vs->mirroring]);
			}

			return;
		}
		vs++;
	}
}

void FCEU_VSUniDraw(uint8 *target) {
	uint32 *dest;
	int y, x;

	if (!DIPS) return;

	dest = (uint32*)(target + 256 * 12 + 164);
	for (y = 24; y; y--, dest += (256 - 72) >> 2) {
		for (x = 72 >> 2; x; x--, dest++)
			*dest = 0;
	}

	dest = (uint32*)(target + 256 * (12 + 4) + 164 + 6);
	for (y = 16; y; y--, dest += (256 >> 2) - 16)
		for (x = 8; x; x--) {
			*dest = 0x01010101;
			dest += 2;
		}

	dest = (uint32*)(target + 256 * (12 + 4) + 164 + 6);
	for (x = 0; x < 8; x++, dest += 2) {
		uint32 *da = dest + (256 >> 2);

		if (!((vsuni_system.vsdip >> x) & 1))
			da += (256 >> 2) * 10;
		for (y = 4; y; y--, da += 256 >> 2)
			*da = 0;
	}
}


SFORMAT FCEUVSUNI_STATEINFO[] = {
	{ &VSindex, 1, "vsin" },
	{ 0 }
};
