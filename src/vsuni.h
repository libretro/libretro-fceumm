#ifndef _FCEU_VSUNI_H
#define _FCEU_VSUNI_H

enum VSGAMEID {
	VS_BASEBALL = 1,
	VS_BATTLECITY,
	VS_CASTLEVANIA,
	VS_CLUCLULAND,
	VS_DRMARIO,
	VS_DUCKHUNT,
	VS_EXITEBIKE,
	VS_FREEDOMFORCE,
	VS_GOONIES,
	VS_GRADIUS,
	VS_GUMSHOE,
	VS_HOGANSALLEY,
	VS_ICECLIMBER,
	VS_JAJAMARU,
	VS_LADIESGOLF,
	VS_MACHRIDER,
	VS_MIGHTYBOMBJACK,
	VS_PINBALL,
	VS_PLATOON,
	VS_RBIBASEBALL,
	VS_SLALOM,
	VS_SMB,
	VS_SOCCER,
	VS_STARLUSTER,
	VS_STROKEANDMATCHGOLF,
	VS_SUPERSKYKID,
	VS_SUPERXEVIOUS,
	VS_TETRIS,
	VS_TKOBOXING,
	VS_TOPGUN,

	VS_GAMEID_COUNT = VS_TOPGUN
};

enum IOPTION {
	IOPTION_GUN       = 0x01,
	IOPTION_SWAPDIRAB = 0x02,
	IOPTION_PREDIP    = 0x10
};

typedef struct VSUNISYSTEM {
	int gameid;		/* Currently used for VS game id for per-game dipswitch */
	int ppu;
	int type;
	int ioption;

	int coinon[2];
	int service;

	int vsdip;

	int rc2c05_enable;
	int rc2c05_A2002;
} VSUNISYSTEM;

extern VSUNISYSTEM vsuni_system;

void FCEU_VSUniPower(void);
void FCEU_VSUniCheck(uint64 md5partial, int *, int *);
void FCEU_VSUniDraw(uint8 *target);

void FCEU_VSUniToggleDIP(int);	/* For movies and netplay */
void FCEU_VSUniCoin(int slot);
void FCEU_VSUniService(void);
void FCEU_VSUniSwap(uint8 *j0, uint8 *j1);

#endif
