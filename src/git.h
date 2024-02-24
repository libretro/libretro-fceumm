#ifndef __FCEU_GIT
#define __FCEU_GIT

#define FTYPE_ERROR   -1
#define FTYPE_UNKNOWN 0
#define FTYPE_INES    1
#define FTYPE_INES20  2
#define FTYPE_UNIF    3
#define FTYPE_FDS     4
#define FTYPE_RAWFDS  5
#define FTYPE_NSF     6
#define FTYPE_NSFE    7
#define FTYPE_SPLIT   8
#define FTYPE_DOCTOR  9

/* Mmm...git. Almost as funny as "gimp". */
enum GITYPES {
	GIT_CART = 0,  /* Cart. */
	GIT_VSUNI = 1, /* VS Unisystem. */
	GIT_FDS = 2,   /* Famicom Disk System. */
	GIT_NSF = 3    /* NES Sound Format. */
};

enum GIV {
	GIV_NTSC = 0, /* NTSC emulation. */
	GIV_PAL = 1,  /* PAL emulation. */
	GIV_USER = 2  /* What was set by FCEUI_SetVidSys(). */
};

enum GIPPU {
	PPU_USER        = 0,
	PPU_RP2C04_0001 = 1,
	PPU_RP2C04_0002 = 2,
	PPU_RP2C04_0003 = 3,
	PPU_RP2C04_0004 = 4,
	PPU_RC2C03    = 5, /* somewhat similar to RP2C03x */
	PPU_RC2C05_01 = 6,
	PPU_RC2C05_02 = 7,
	PPU_RC2C05_03 = 8,
	PPU_RC2C05_04 = 9,
	PPU_RC2C05_05 = 10
};

enum VSTYPES {
	VS_TYPE_NORMAL     = 0,
	VS_TYPE_RBI        = 1, /* RBI Baseball protection */
	VS_TYPE_TKO        = 2, /* TKO Boxing protection */
	VS_TYPE_XEVIOUS    = 3, /* Super Xevious protection */
	VS_TYPE_ICECLIMBER = 4  /* Ice Climber protection (unimplemented) */
};

enum SIS {
	SIS_NONE        = 0,
	SIS_DATACH      = 1,
	SIS_NWC         = 2,
	SIS_VSUNISYSTEM = 3,
	SIS_NSF         = 4
};

enum SINES {
	SI_UNSET          = -1,
	SI_NONE           = 0,
	SI_GAMEPAD        = 1,
	SI_ZAPPER         = 2,
	SI_POWERPADA      = 3,
	SI_POWERPADB      = 4,
	SI_ARKANOID       = 5,
	SI_MOUSE          = 6,
	SI_LCDCOMP_ZAPPER = 7,
	SI_SNES_MOUSE     = 8,
	SI_SNES_GAMEPAD   = 9,
	SI_VIRTUALBOY     = 10
};

enum SIFC {
	SIFC_UNSET      = -1,
	SIFC_NONE       = 0,
	SIFC_ARKANOID   = 1,
	SIFC_SHADOW     = 2,
	SIFC_4PLAYER    = 3,
	SIFC_FKB        = 4,
	SIFC_SUBORKB    = 5,
	SIFC_PEC586KB   = 6,
	SIFC_HYPERSHOT  = 7,
	SIFC_MAHJONG    = 8,
	SIFC_PARTYTAP   = 9,
	SIFC_FTRAINERA  = 10,
	SIFC_FTRAINERB  = 11,
	SIFC_OEKAKIDS   = 12,
	SIFC_BWORLD     = 13,
	SIFC_TOPRIDER   = 14,
	SIFC_FAMINETSYS = 15,
	SIFC_HORI4PLAYER = 16,
	SIFC_EXCITINGBOXING = 17,

	SIFC_COUNT = SIFC_EXCITINGBOXING
};

typedef struct {
	uint8 *name; /* Game name, UTF8 encoding */

	int type;     /* GIT_* */
	int vidsys;   /* Current emulated video system; GIV_* */
	int input[2]; /* Desired input for emulated input ports 1 and 2; -1
	              for unknown desired input. */
	int inputfc;  /* Desired Famicom expansion port device. -1 for unknown
	              desired input. */
	int cspecial; /* Special cart expansion: DIP switches, barcode
	              reader, etc. */
	uint8 MD5[16];
	int soundrate; /* For Ogg Vorbis expansion sound wacky support.  0 for default. */
	int soundchan; /* Number of sound channels. */
} FCEUGI;

#endif
