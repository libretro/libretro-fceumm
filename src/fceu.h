#ifndef _FCEUH
#define _FCEUH

#include "fceu-types.h"

/* Overclocking-related */
extern unsigned overclock_enabled;
extern unsigned overclocked;
extern unsigned skip_7bit_overclocking;
extern unsigned DMC_7bit;
extern unsigned totalscanlines;
extern unsigned normal_scanlines;
extern unsigned extrascanlines;
extern unsigned vblankscanlines;

/* Region selection */
extern unsigned dendy;

/* Audio mods*/
extern unsigned swapDuty; /* Swap bits 6 & 7 of $4000/$4004 to mimic bug
                           * found on some famiclones/Dendy models.
                           */

void ResetGameLoaded(void);

#define DECLFR(x) uint8_t FP_FASTAPASS(1) x(uint32_t A)
#define DECLFW(x) void FP_FASTAPASS(2) x(uint32_t A, uint8_t V)

void FCEU_MemoryRand(uint8_t *ptr, uint32_t size);
void FCEU_MemoryRand_Reseed(uint32_t seed);
void FASTAPASS(3) SetReadHandler(int32_t start, int32_t end, readfunc func);
void FASTAPASS(3) SetWriteHandler(int32_t start, int32_t end, writefunc func);
writefunc FASTAPASS(1) GetWriteHandler(int32_t a);
readfunc FASTAPASS(1) GetReadHandler(int32_t a);

int AllocGenieRW(void);
void FlushGenieRW(void);

void FCEU_ResetVidSys(void);

void ResetMapping(void);
void ResetNES(void);
void PowerNES(void);


extern uint64_t timestampbase;
extern uint32_t MMC5HackVROMMask;
extern uint8_t *MMC5HackExNTARAMPtr;
extern int MMC5Hack, PEC586Hack;
extern uint8_t *MMC5HackVROMPTR;
extern uint8_t MMC5HackCHRMode;
extern uint8_t MMC5HackSPMode;
extern uint8_t MMC50x5130;
extern uint8_t MMC5HackSPScroll;
extern uint8_t MMC5HackSPPage;

extern uint8_t RAM[0x800];

extern readfunc ARead[0x10000];
extern writefunc BWrite[0x10000];

extern void (*GameInterface)(int h);
extern void (*GameStateRestore)(int version);

#define GI_RESETM2  1
#define GI_POWER  2
#define GI_CLOSE  3

#include "git.h"
extern FCEUGI *GameInfo;

extern uint8_t PAL;

#include "driver.h"

typedef struct {
	int PAL;
	int SoundVolume;
	int TriangleVolume;
	int SquareVolume[2];
	int NoiseVolume;
	int PCMVolume;
	int GameGenie;

	/* Current first and last rendered scanlines. */
	int FirstSLine;
	int LastSLine;

	/* Driver code(user)-specified first and last rendered scanlines.
	 * Usr*SLine[0] is for NTSC, Usr*SLine[1] is for PAL.
	 */
	int UsrFirstSLine[2];
	int UsrLastSLine[2];
	uint32_t SndRate;
	int soundq;
	int lowpass;
} FCEUS;

extern FCEUS FSettings;

void FCEU_PrintError(const char *format, ...);
void FCEU_printf(const char *format, ...);

void SetNESDeemph(uint8_t d, int force);
void DrawTextTrans(uint8_t *dest, uint32_t width, uint8_t *textmsg, uint8_t fgcolor);
void FCEU_PutImage(void);

extern uint8_t Exit;
extern uint8_t default_palette_selected;
extern uint8_t vsdip;

#define JOY_A        0x01
#define JOY_B        0x02
#define JOY_SELECT   0x04
#define JOY_START    0x08
#define JOY_UP       0x10
#define JOY_DOWN     0x20
#define JOY_LEFT     0x40
#define JOY_RIGHT    0x80

#endif
