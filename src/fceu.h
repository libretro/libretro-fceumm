#ifndef _FCEUH
#define _FCEUH

#include "fceu-types.h"
#include "file.h"

#define RAM_SIZE 0x800
#define RAM_MASK (RAM_SIZE - 1)

#define NES_WIDTH  256
#define NES_HEIGHT 240
#define NTSC_WIDTH 602

extern int fceuindbg;
void ResetGameLoaded(void);

#define DECLFR(x) uint8 x(uint16 A)
#define DECLFW(x) void x(uint16 A, uint8 V)

void FCEU_MemoryRand(uint8 *ptr, uint32 size);
void SetReadHandler(uint16 start, uint16 end, readfunc func);
void SetWriteHandler(uint16 start, uint16 end, writefunc func);
writefunc GetWriteHandler(uint16 a);
readfunc GetReadHandler(uint16 a);

void FCEU_ResetVidSys(void);

void ResetMapping(void);
void ResetNES(void);
void PowerNES(void);

extern uint64 timestampbase;
extern uint32 MMC5HackVROMMask;
extern uint8 *MMC5HackExNTARAMPtr;
extern uint8 MMC5Hack;
extern uint8 *MMC5HackVROMPTR;
extern uint8 MMC5HackCHRMode;
extern uint8 MMC5HackSPMode;
extern uint8 MMC50x5130;
extern uint8 MMC5HackSPScroll;
extern uint8 MMC5HackSPPage;

extern uint8 PEC586Hack;

extern uint8 QTAIHack;
extern uint8 qtramreg;
extern uint8 QTRAM[0x800];

extern uint8 *RAM;

extern readfunc ARead[0x10000];
extern writefunc BWrite[0x10000];

extern void (*GameInterface)(int h);
extern void (*GameStateRestore)(int version);

#define GI_RESETM2 1
#define GI_POWER   2
#define GI_CLOSE   3

#include "git.h"
extern FCEUGI *GameInfo;

extern uint8 isPAL;
extern uint8 isDendy;

#include "driver.h"

typedef struct {
	int PAL;

	int volume[12]; /* master, nes apu and expansion audio */

	int GameGenie;

	/* Current first and last rendered scanlines. */
	int FirstSLine;
	int LastSLine;

	/* Driver code(user)-specified first and last rendered scanlines.
	 * Usr*SLine[0] is for NTSC, Usr*SLine[1] is for PAL.
	 */
	int UsrFirstSLine[2];
	int UsrLastSLine[2];

	int SndRate;
	int soundq;
	int lowpass;

	int SwapDutyCycles;
	int RamInitState;
	int ShowCrosshair;
	int ReplaceP2StartWithMicrophone;
	int PPUOverclockEnabled;
	int SkipDMC7BitOverclock;
	int ReduceDMCPopping;
} FCEUS;

extern FCEUS FSettings;

void FCEU_PrintError(char *format, ...); /* warning level messages */
void FCEU_PrintDebug(char *format, ...); /* debug level messages */
void FCEU_printf(char *format, ...);     /* normal messages */

void SetNESDeemph(uint8 d, int force);
void DrawTextTrans(uint8 *dest, uint32 width, uint8 *textmsg, uint8 fgcolor);
void FCEU_PutImage(void);
#ifdef FRAMESKIP
void FCEU_PutImageDummy(void);
#endif

#define JOY_A      0x01
#define JOY_B      0x02
#define JOY_SELECT 0x04
#define JOY_START  0x08
#define JOY_UP     0x10
#define JOY_DOWN   0x20
#define JOY_LEFT   0x40
#define JOY_RIGHT  0x80

int UNIFLoad(const char *name, FCEUFILE *fp);
int iNESLoad(const char *name, FCEUFILE *fp);
int FDSLoad(const char *name, FCEUFILE *fp);
int NSFLoad(FCEUFILE *fp);

#endif
