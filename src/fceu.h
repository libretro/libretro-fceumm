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

void FCEU_MemoryRand(uint8 *ptr, uint32 size);
void SetReadHandler(int32 start, int32 end, readfunc func);
void SetWriteHandler(int32 start, int32 end, writefunc func);
writefunc GetWriteHandler(int32 a);
readfunc GetReadHandler(int32 a);

int AllocGenieRW(void);
void FlushGenieRW(void);

void FCEU_ResetVidSys(void);

void ResetMapping(void);
void ResetNES(void);
void PowerNES(void);


extern uint64 timestampbase;
extern uint32 MMC5HackVROMMask;
extern uint8 *MMC5HackExNTARAMPtr;
extern int MMC5Hack, PEC586Hack;
extern uint8 *MMC5HackVROMPTR;
extern uint8 MMC5HackCHRMode;
extern uint8 MMC5HackSPMode;
extern uint8 MMC50x5130;
extern uint8 MMC5HackSPScroll;
extern uint8 MMC5HackSPPage;

extern uint8 RAM[0x800];

extern readfunc ARead[0x10000];
extern writefunc BWrite[0x10000];

extern void (*GameInterface)(int h);
extern void (*GameStateRestore)(int version);

#define GI_RESETM2  1
#define GI_POWER  2
#define GI_CLOSE  3

#include "git.h"
extern FCEUGI *GameInfo;

extern uint8 PAL;

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
	uint32 SndRate;
	int soundq;
	int lowpass;
} FCEUS;

extern FCEUS FSettings;

void FCEU_PrintError(char *format, ...);
void FCEU_printf(char *format, ...);

void SetNESDeemph(uint8 d, int force);

extern uint8 pale;
extern uint8 vsdip;

#define JOY_A        0x01
#define JOY_B        0x02
#define JOY_SELECT   0x04
#define JOY_START    0x08
#define JOY_UP       0x10
#define JOY_DOWN     0x20
#define JOY_LEFT     0x40
#define JOY_RIGHT    0x80

#endif
