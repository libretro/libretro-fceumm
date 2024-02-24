#ifndef _FCEU_CART_H
#define _FCEU_CART_H

enum DefaultValues { DEFAULT = -1, NOEXTRA = -1 };
enum ConsoleSystem { NES_NTSC = 0, NES_PAL = 1, MULTI = 2, DENDY = 3 };

typedef struct {
	/* Set by mapper/board code: */
	void (*Power)(void);
	void (*Reset)(void);
	void (*Close)(void);

	uint8 *SaveGame[4];    /* Pointers to memory to save/load. */
	uint32 SaveGameLen[4]; /* How much memory to save/load. */

	/* Set by iNES/UNIF loading code. */
	int format;
	int iNES2;   /* iNES version */
	int mapper; /* mapper used */
	int submapper;
	int mirror;      /* As set in the header or chunk.
	                    * iNES/UNIF specific.  Intended
	                    * to help support games like "Karnov"
	                    * that are not really MMC3 but are
	                    * set to mapper 4.
	                    */
	int mirror2bits; /* a 2bit representation for mirroring.
	                    * For use in nonstandard way like apply one-screen mirroring on
	                    * mapper 30.
	                    */
	int battery;     /* Presence of an actual battery. */
	int trainer;     /* Presense of trainer data */
	int MiscRoms;    /* Presense of misc roms */
	int region;      /* video system timing (NTSC, PAL, Dendy */
	int ConsoleType;
	int InputTypes;
	int VS_PPUTypes;
	int VS_HWType;

	int PRGRomSize;     /* actual prg rom size in bytes */
	int CHRRomSize;     /* actual chr rom size in bytes */
	int PRGRamSize;     /* prg ram size in bytes (volatile) */
	int CHRRamSize;     /* chr ram size in bytes (volatile) */
	int PRGRamSaveSize; /* prg ram size in bytes (non-volatile or battery backed) */
	int CHRRamSaveSize; /* chr ram size in bytes (non-volatile or battery backed) */

	uint8 MD5[16];
	uint32 PRGCRC32;
	uint32 CHRCRC32;
	uint32 CRC32; /* Should be set by the iNES/UNIF loading
	               * code, used by mapper/board code, maybe
	               * other code in the future.
	               */
} CartInfo;

typedef struct mem_t {
	uint8 *data;
	uint32 size;
} mem_t;

typedef struct romData_t {
	mem_t prg; /* size in 16k banks */
	mem_t chr; /* size in 8k banks */
	mem_t trainer;
	mem_t misc;
	mem_t disk;
	mem_t disko;
} romData_t;

extern uint8 *Page[32], *VPage[8], *MMC5SPRVPage[8], *MMC5BGVPage[8];

void ResetCartMapping(void);
void SetupCartPRGMapping(int chip, uint8 *p, uint32 size, uint8 ram);
void SetupCartCHRMapping(int chip, uint8 *p, uint32 size, uint8 ram);
void SetupCartMirroring(int m, int hard, uint8 *extra);

DECLFR(CartBROB);
DECLFR(CartBR);
DECLFW(CartBW);

extern uint8 *PRGptr[32];
extern uint8 *CHRptr[32];

extern uint32 PRGsize[32];
extern uint32 CHRsize[32];

extern uint32 PRGmask2[32];
extern uint32 PRGmask4[32];
extern uint32 PRGmask8[32];
extern uint32 PRGmask16[32];
extern uint32 PRGmask32[32];

extern uint32 CHRmask1[32];
extern uint32 CHRmask2[32];
extern uint32 CHRmask4[32];
extern uint32 CHRmask8[32];

void setprg2(uint16 A, uint16 V);
void setprg4(uint16 A, uint16 V);
void setprg8(uint16 A, uint16 V);
void setprg16(uint16 A, uint16 V);
void setprg32(uint16 A, uint16 V);

void setprg2r(int r, uint16 A, uint16 V);
void setprg4r(int r, uint16 A, uint16 V);
void setprg8r(int r, uint16 A, uint16 V);
void setprg16r(int r, uint16 A, uint16 V);
void setprg32r(int r, uint16 A, uint16 V);

void setchr1r(int r, uint16 A, uint16 V);
void setchr2r(int r, uint16 A, uint16 V);
void setchr4r(int r, uint16 A, uint16 V);
void setchr8r(int r, uint16 V);

void setchr1(uint16 A, uint16 V);
void setchr2(uint16 A, uint16 V);
void setchr4(uint16 A, uint16 V);
void setchr8(uint16 V);

void setmirror(int t);
void setmirrorw(int a, int b, int c, int d);
void setntamem(uint8 *p, int ram, int b);

enum MirroringType {
	MI_H = 0, /* horizontal */
	MI_V = 1, /* vertical */
	MI_0 = 2, /* single-screen 0 */
	MI_1 = 3, /* single-screen 1 */
	MI_4 = 4  /* four-screen */
};

extern CartInfo iNESCart;

extern uint8 *WRAM;
extern uint32 WRAMSIZE;
extern romData_t ROM;

#endif
