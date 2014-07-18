#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include "libretro.h"

#include "../../fceu.h"
#include "../../fceu-endian.h"
#include "../../input.h"
#include "../../state.h"
#include "../../ppu.h"
#include "../../cart.h"
#include "../../x6502.h"
#include "../../git.h"
#include "../../palette.h"
#include "../../sound.h"
#include "../../file.h"
#include "../../cheat.h"
#include "../../ines.h"
#include "../../unif.h"
#include "../../fds.h"

#include <string.h>
#include "memstream.h"



static retro_video_refresh_t video_cb = NULL;
static retro_input_poll_t poll_cb = NULL;
static retro_input_state_t input_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_environment_t environ_cb = NULL;
static bool use_overscan;

/* emulator-specific variables */

int FCEUnetplay;
#ifdef PSP
#include "pspgu.h"
static __attribute__((aligned(16))) uint16_t retro_palette[256];
#else
static uint16_t retro_palette[256];
#endif

/* Some timing-related variables. */
static int maxconbskip = 9;			/* Maximum consecutive blit skips. */
static int ffbskip = 9;				/* Blit skips per blit when FF-ing */

static int soundo = 1;

static volatile int nofocus = 0;

static int32 *sound = 0;
static uint32 JSReturn[2];
static uint32 current_palette = 0;

int PPUViewScanline=0;
int PPUViewer=0;

/* extern forward decls.*/
extern FCEUGI *GameInfo;
extern uint8 *XBuf;
extern CartInfo iNESCart;
extern CartInfo UNIFCart;

/* emulator-specific callback functions */

void UpdatePPUView(int refreshchr) { }

const char * GetKeyboard(void)
{
   return "";
}

int FCEUD_SendData(void *data, uint32 len)
{
   return 1;
}

#if defined (PSP)
#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 11
#define RED_EXPAND 3
#define GREEN_EXPAND 2
#define BLUE_EXPAND 3
#elif defined (FRONTEND_SUPPORTS_RGB565)
#define RED_SHIFT 11
#define GREEN_SHIFT 5
#define BLUE_SHIFT 0
#define RED_EXPAND 3
#define GREEN_EXPAND 2
#define BLUE_EXPAND 3
#define RED_MASK 0xF800
#define GREEN_MASK 0x7e0
#define BLUE_MASK 0x1f
#else
#define RED_SHIFT 10
#define GREEN_SHIFT 5
#define BLUE_SHIFT 0
#define RED_EXPAND 3
#define GREEN_EXPAND 3
#define BLUE_EXPAND 3
#endif

void FCEUD_SetPalette(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
   r >>= RED_EXPAND;
   g >>= GREEN_EXPAND;
   b >>= BLUE_EXPAND;
   retro_palette[index] = (r << RED_SHIFT) | (g << GREEN_SHIFT) | (b << BLUE_SHIFT);
}

void FCEUD_GetPalette(unsigned char i, unsigned char *r, unsigned char *g, unsigned char *b)
{
}


bool FCEUD_ShouldDrawInputAids (void)
{
   return 1;
}

static struct retro_log_callback log_cb;

static void default_logger(enum retro_log_level level, const char *fmt, ...) {}

void FCEUD_PrintError(char *c)
{
   log_cb.log(RETRO_LOG_WARN, "%s", c);
}

void FCEUD_Message(char *s)
{
   log_cb.log(RETRO_LOG_INFO, "%s", s);
}

void FCEUD_NetworkClose(void)
{ }

void FCEUD_SoundToggle (void)
{
   FCEUI_SetSoundVolume(100);
}

void FCEUD_VideoChanged (void)
{ }

FILE *FCEUD_UTF8fopen(const char *n, const char *m)
{
   return fopen(n, m);
}

#define MAX_PATH 1024

/*palette for FCEU*/
#define MAXPAL 13

struct st_palettes {
	char name[32];
	char desc[32];
	unsigned int data[64];
};


struct st_palettes palettes[] = {
   { "asqrealc", "AspiringSquire's Real palette",
      { 0x6c6c6c, 0x00268e, 0x0000a8, 0x400094,
	      0x700070, 0x780040, 0x700000, 0x621600,
	      0x442400, 0x343400, 0x005000, 0x004444,
	      0x004060, 0x000000, 0x101010, 0x101010,
	      0xbababa, 0x205cdc, 0x3838ff, 0x8020f0,
	      0xc000c0, 0xd01474, 0xd02020, 0xac4014,
	      0x7c5400, 0x586400, 0x008800, 0x007468,
	      0x00749c, 0x202020, 0x101010, 0x101010,
	      0xffffff, 0x4ca0ff, 0x8888ff, 0xc06cff,
	      0xff50ff, 0xff64b8, 0xff7878, 0xff9638,
	      0xdbab00, 0xa2ca20, 0x4adc4a, 0x2ccca4,
	      0x1cc2ea, 0x585858, 0x101010, 0x101010,
	      0xffffff, 0xb0d4ff, 0xc4c4ff, 0xe8b8ff,
	      0xffb0ff, 0xffb8e8, 0xffc4c4, 0xffd4a8,
	      0xffe890, 0xf0f4a4, 0xc0ffc0, 0xacf4f0,
	      0xa0e8ff, 0xc2c2c2, 0x202020, 0x101010 }
	},
   { "loopy", "Loopy's palette",
	   { 0x757575, 0x271b8f, 0x0000ab, 0x47009f,
		   0x8f0077, 0xab0013, 0xa70000, 0x7f0b00,
		   0x432f00, 0x004700, 0x005100, 0x003f17,
		   0x1b3f5f, 0x000000, 0x000000, 0x000000,
		   0xbcbcbc, 0x0073ef, 0x233bef, 0x8300f3,
		   0xbf00bf, 0xe7005b, 0xdb2b00, 0xcb4f0f,
		   0x8b7300, 0x009700, 0x00ab00, 0x00933b,
		   0x00838b, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0x3fbfff, 0x5f97ff, 0xa78bfd,
		   0xf77bff, 0xff77b7, 0xff7763, 0xff9b3b,
		   0xf3bf3f, 0x83d313, 0x4fdf4b, 0x58f898,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0xabe7ff, 0xc7d7ff, 0xd7cbff,
		   0xffc7ff, 0xffc7db, 0xffbfb3, 0xffdbab,
		   0xffe7a3, 0xe3ffa3, 0xabf3bf, 0xb3ffcf,
		   0x9ffff3, 0x000000, 0x000000, 0x000000 }
   },
   { "quor", "Quor's palette",
	   { 0x3f3f3f, 0x001f3f, 0x00003f, 0x1f003f,
		   0x3f003f, 0x3f0020, 0x3f0000, 0x3f2000,
		   0x3f3f00, 0x203f00, 0x003f00, 0x003f20,
		   0x003f3f, 0x000000, 0x000000, 0x000000,
		   0x7f7f7f, 0x405f7f, 0x40407f, 0x5f407f,
		   0x7f407f, 0x7f4060, 0x7f4040, 0x7f6040,
		   0x7f7f40, 0x607f40, 0x407f40, 0x407f60,
		   0x407f7f, 0x000000, 0x000000, 0x000000,
		   0xbfbfbf, 0x809fbf, 0x8080bf, 0x9f80bf,
		   0xbf80bf, 0xbf80a0, 0xbf8080, 0xbfa080,
		   0xbfbf80, 0xa0bf80, 0x80bf80, 0x80bfa0,
		   0x80bfbf, 0x000000, 0x000000, 0x000000,
		   0xffffff, 0xc0dfff, 0xc0c0ff, 0xdfc0ff,
		   0xffc0ff, 0xffc0e0, 0xffc0c0, 0xffe0c0,
		   0xffffc0, 0xe0ffc0, 0xc0ffc0, 0xc0ffe0,
		   0xc0ffff, 0x000000, 0x000000, 0x000000 }
   },
   { "chris", "Chris Covell's palette",
	   { 0x808080, 0x003DA6, 0x0012B0, 0x440096,
		   0xA1005E, 0xC70028, 0xBA0600, 0x8C1700,
		   0x5C2F00, 0x104500, 0x054A00, 0x00472E,
		   0x004166, 0x000000, 0x050505, 0x050505,
		   0xC7C7C7, 0x0077FF, 0x2155FF, 0x8237FA,
		   0xEB2FB5, 0xFF2950, 0xFF2200, 0xD63200,
		   0xC46200, 0x358000, 0x058F00, 0x008A55,
		   0x0099CC, 0x212121, 0x090909, 0x090909,
		   0xFFFFFF, 0x0FD7FF, 0x69A2FF, 0xD480FF,
		   0xFF45F3, 0xFF618B, 0xFF8833, 0xFF9C12,
		   0xFABC20, 0x9FE30E, 0x2BF035, 0x0CF0A4,
		   0x05FBFF, 0x5E5E5E, 0x0D0D0D, 0x0D0D0D,
		   0xFFFFFF, 0xA6FCFF, 0xB3ECFF, 0xDAABEB,
		   0xFFA8F9, 0xFFABB3, 0xFFD2B0, 0xFFEFA6,
		   0xFFF79C, 0xD7E895, 0xA6EDAF, 0xA2F2DA,
		   0x99FFFC, 0xDDDDDD, 0x111111, 0x111111 }
   },
   { "matt", "Matthew Conte's palette",
	   { 0x808080, 0x0000bb, 0x3700bf, 0x8400a6,
		   0xbb006a, 0xb7001e, 0xb30000, 0x912600,
		   0x7b2b00, 0x003e00, 0x00480d, 0x003c22,
		   0x002f66, 0x000000, 0x050505, 0x050505,
		   0xc8c8c8, 0x0059ff, 0x443cff, 0xb733cc,
		   0xff33aa, 0xff375e, 0xff371a, 0xd54b00,
		   0xc46200, 0x3c7b00, 0x1e8415, 0x009566,
		   0x0084c4, 0x111111, 0x090909, 0x090909,
		   0xffffff, 0x0095ff, 0x6f84ff, 0xd56fff,
		   0xff77cc, 0xff6f99, 0xff7b59, 0xff915f,
		   0xffa233, 0xa6bf00, 0x51d96a, 0x4dd5ae,
		   0x00d9ff, 0x666666, 0x0d0d0d, 0x0d0d0d,
		   0xffffff, 0x84bfff, 0xbbbbff, 0xd0bbff,
		   0xffbfea, 0xffbfcc, 0xffc4b7, 0xffccae,
		   0xffd9a2, 0xcce199, 0xaeeeb7, 0xaaf7ee,
		   0xb3eeff, 0xdddddd, 0x111111, 0x111111 }
   },
   { "pasofami", "PasoFami/99 palette",
	   { 0x7f7f7f, 0x0000ff, 0x0000bf, 0x472bbf,
		   0x970087, 0xab0023, 0xab1300, 0x8b1700,
		   0x533000, 0x007800, 0x006b00, 0x005b00,
		   0x004358, 0x000000, 0x000000, 0x000000,
		   0xbfbfbf, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0xdb00cd, 0xe7005b, 0xf83800, 0xe75f13,
		   0xaf7f00, 0x00b800, 0x00ab00, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x000000,
		   0xf8f8f8, 0x3fbfff, 0x6b88ff, 0x9878f8,
		   0xf878f8, 0xf85898, 0xf87858, 0xffa347,
		   0xf8b800, 0xb8f818, 0x5bdb57, 0x58f898,
		   0x00ebdb, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xd8b8f8,
		   0xf8b8f8, 0xfba7c3, 0xf0d0b0, 0xffe3ab,
		   0xfbdb7b, 0xd8f878, 0xb8f8b8, 0xb8f8d8,
		   0x00ffff, 0xf8d8f8, 0x000000, 0x000000 }
   },
   { "crashman", "CrashMan's palette",
	   { 0x585858, 0x001173, 0x000062, 0x472bbf,
		   0x970087, 0x910009, 0x6f1100, 0x4c1008,
		   0x371e00, 0x002f00, 0x005500, 0x004d15,
		   0x002840, 0x000000, 0x000000, 0x000000,
		   0xa0a0a0, 0x004499, 0x2c2cc8, 0x590daa,
		   0xae006a, 0xb00040, 0xb83418, 0x983010,
		   0x704000, 0x308000, 0x207808, 0x007b33,
		   0x1c6888, 0x000000, 0x000000, 0x000000,
		   0xf8f8f8, 0x267be1, 0x5870f0, 0x9878f8,
		   0xff73c8, 0xf060a8, 0xd07b37, 0xe09040,
		   0xf8b300, 0x8cbc00, 0x40a858, 0x58f898,
		   0x00b7bf, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xd8b8f8,
		   0xe6a6ff, 0xf29dc4, 0xf0c0b0, 0xfce4b0,
		   0xe0e01e, 0xd8f878, 0xc0e890, 0x95f7c8,
		   0x98e0e8, 0xf8d8f8, 0x000000, 0x000000 }
   },
   { "mess", "MESS palette",
	   { 0x747474, 0x24188c, 0x0000a8, 0x44009c,
		   0x8c0074, 0xa80010, 0xa40000, 0x7c0800,
		   0x402c00, 0x004400, 0x005000, 0x003c14,
		   0x183c5c, 0x000000, 0x000000, 0x000000,
		   0xbcbcbc, 0x0070ec, 0x2038ec, 0x8000f0,
		   0xbc00bc, 0xe40058, 0xd82800, 0xc84c0c,
		   0x887000, 0x009400, 0x00a800, 0x009038,
		   0x008088, 0x000000, 0x000000, 0x000000,
		   0xfcfcfc, 0x3cbcfc, 0x5c94fc, 0x4088fc,
		   0xf478fc, 0xfc74b4, 0xfc7460, 0xfc9838,
		   0xf0bc3c, 0x80d010, 0x4cdc48, 0x58f898,
		   0x00e8d8, 0x000000, 0x000000, 0x000000,
		   0xfcfcfc, 0xa8e4fc, 0xc4d4fc, 0xd4c8fc,
		   0xfcc4fc, 0xfcc4d8, 0xfcbcb0, 0xfcd8a8,
		   0xfce4a0, 0xe0fca0, 0xa8f0bc, 0xb0fccc,
		   0x9cfcf0, 0x000000, 0x000000, 0x000000 }
   },
   { "zaphod-cv", "Zaphod's VS Castlevania palette",
	   { 0x7f7f7f, 0xffa347, 0x0000bf, 0x472bbf,
		   0x970087, 0xf85898, 0xab1300, 0xf8b8f8,
		   0xbf0000, 0x007800, 0x006b00, 0x005b00,
		   0xffffff, 0x9878f8, 0x000000, 0x000000,
		   0xbfbfbf, 0x0078f8, 0xab1300, 0x6b47ff,
		   0x00ae00, 0xe7005b, 0xf83800, 0x7777ff,
		   0xaf7f00, 0x00b800, 0x00ab00, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x472bbf,
		   0xf8f8f8, 0xffe3ab, 0xf87858, 0x9878f8,
		   0x0078f8, 0xf85898, 0xbfbfbf, 0xffa347,
		   0xc800c8, 0xb8f818, 0x7f7f7f, 0x007800,
		   0x00ebdb, 0x000000, 0x000000, 0xffffff,
		   0xffffff, 0xa7e7ff, 0x5bdb57, 0xe75f13,
		   0x004358, 0x0000ff, 0xe7005b, 0x00b800,
		   0xfbdb7b, 0xd8f878, 0x8b1700, 0xffe3ab,
		   0x00ffff, 0xab0023, 0x000000, 0x000000 }
   },
   { "zaphod-smb", "Zaphod's VS SMB palette",
	   { 0x626a00, 0x0000ff, 0x006a77, 0x472bbf,
		   0x970087, 0xab0023, 0xab1300, 0xb74800,
		   0xa2a2a2, 0x007800, 0x006b00, 0x005b00,
		   0xffd599, 0xffff00, 0x009900, 0x000000,
		   0xff66ff, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0xe75f13,
		   0xaf7f00, 0x00b800, 0x5173ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x91ff88, 0x000088,
		   0xf8f8f8, 0x3fbfff, 0x6b0000, 0x4855f8,
		   0xf878f8, 0xf85898, 0x595958, 0xff009d,
		   0x002f2f, 0xb8f818, 0x5bdb57, 0x58f898,
		   0x00ebdb, 0x787878, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0x590400, 0xbb0000,
		   0xf8b8f8, 0xfba7c3, 0xffffff, 0x00e3e1,
		   0xfbdb7b, 0xffae00, 0xb8f8b8, 0xb8f8d8,
		   0x00ff00, 0xf8d8f8, 0xffaaaa, 0x004000 }
   },
   { "vs-drmar", "VS Dr. Mario palette",
	   { 0x5f97ff, 0x000000, 0x000000, 0x47009f,
		   0x00ab00, 0xffffff, 0xabe7ff, 0x000000,
		   0x000000, 0x000000, 0x000000, 0x000000,
		   0xe7005b, 0x000000, 0x000000, 0x000000,
		   0x5f97ff, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x8b7300, 0xcb4f0f, 0x000000,
		   0xbcbcbc, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x000000, 0x000000, 0x000000,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0x000000, 0xff9b3b, 0x000000, 0x000000,
		   0x83d313, 0x000000, 0x3fbfff, 0x000000,
		   0x0073ef, 0x000000, 0x000000, 0x000000,
		   0x00ebdb, 0x000000, 0x000000, 0x000000,
		   0x000000, 0x000000, 0xf3bf3f, 0x000000,
		   0x005100, 0x000000, 0xc7d7ff, 0xffdbab,
		   0x000000, 0x000000, 0x000000, 0x000000 }
   },
   { "vs-cv", "VS Castlevania palette",
	   { 0xaf7f00, 0xffa347, 0x008b8b, 0x472bbf,
		   0x970087, 0xf85898, 0xab1300, 0xf8b8f8,
		   0xf83800, 0x007800, 0x006b00, 0x005b00,
		   0xffffff, 0x9878f8, 0x00ab00, 0x000000,
		   0xbfbfbf, 0x0078f8, 0xab1300, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0x6b88ff,
		   0xaf7f00, 0x00b800, 0x6b88ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x472bbf,
		   0xf8f8f8, 0xffe3ab, 0xf87858, 0x9878f8,
		   0x0078f8, 0xf85898, 0xbfbfbf, 0xffa347,
		   0x004358, 0xb8f818, 0x7f7f7f, 0x007800,
		   0x00ebdb, 0x000000, 0x000000, 0xffffff,
		   0xffffff, 0xa7e7ff, 0x5bdb57, 0x6b88ff,
		   0x004358, 0x0000ff, 0xe7005b, 0x00b800,
		   0xfbdb7b, 0xffa347, 0x8b1700, 0xffe3ab,
		   0xb8f818, 0xab0023, 0x000000, 0x007800 }
   },
   { "vs-smb", "VS SMB/VS Ice Climber palette",
	   { 0xaf7f00, 0x0000ff, 0x008b8b, 0x472bbf,
		   0x970087, 0xab0023, 0x0000ff, 0xe75f13,
		   0xbfbfbf, 0x007800, 0x5bdb57, 0x005b00,
		   0xf0d0b0, 0xffe3ab, 0x00ab00, 0x000000,
		   0xbfbfbf, 0x0078f8, 0x0058f8, 0x6b47ff,
		   0x000000, 0xe7005b, 0xf83800, 0xf87858,
		   0xaf7f00, 0x00b800, 0x6b88ff, 0x00ab47,
		   0x008b8b, 0x000000, 0x000000, 0x3fbfff,
		   0xf8f8f8, 0x006b00, 0x8b1700, 0x9878f8,
		   0x6b47ff, 0xf85898, 0x7f7f7f, 0xe7005b,
		   0x004358, 0xb8f818, 0x0078f8, 0x58f898,
		   0x00ebdb, 0xfbdb7b, 0x000000, 0x000000,
		   0xffffff, 0xa7e7ff, 0xb8b8f8, 0xf83800,
		   0xf8b8f8, 0xfba7c3, 0xffffff, 0x00ffff,
		   0xfbdb7b, 0xffa347, 0xb8f8b8, 0xb8f8d8,
		   0xb8f818, 0xf8d8f8, 0x000000, 0x007800 }
   }
};

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{ }

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_cb = cb;
}

void retro_set_controller_port_device(unsigned a, unsigned b)
{}


void retro_set_environment(retro_environment_t cb)
{
   static const struct retro_variable vars[] = {
      { "nes_palette", "Color Palette; asqrealc|loopy|quor|chris|matt|pasofami|crashman|mess|zaphod-cv|zaphod-smb|vs-drmar|vs-cv|vs-smb" },
      { NULL, NULL },
   };

   environ_cb = cb;
   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->need_fullpath = true;
   info->valid_extensions = "fds|nes|unif";
   info->library_version = "(SVN)";
   info->library_name = "FCEUmm";
   info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   unsigned width = use_overscan ? 256 : (256 - 16);
   unsigned height = use_overscan ? 240 : (240 - 16);
   info->geometry.base_width = width;
   info->geometry.base_height = height;
   info->geometry.max_width = width;
   info->geometry.max_height = height;
   info->geometry.aspect_ratio = 4.0 / 3.0;
   info->timing.sample_rate = 32050.0;
   if (FSettings.PAL)
      info->timing.fps = 838977920.0/16777215.0;
   else
      info->timing.fps = 1008307711.0/16777215.0;
   info->timing.sample_rate = 32040.5;
}

static void check_system_specs(void)
{
   // TODO - when we get it running at fullspeed on PSP, set to 4
   unsigned level = 5;
   environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_init(void)
{
   enum retro_pixel_format rgb565;
   log_cb.log=default_logger;
   environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);
#ifdef FRONTEND_SUPPORTS_RGB565
   rgb565 = RETRO_PIXEL_FORMAT_RGB565;
   if(environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565))
      log_cb.log(RETRO_LOG_INFO, "Frontend supports RGB565 - will use that instead of XRGB1555.\n");
#endif
   PowerNES();
   check_system_specs();
}

static void emulator_set_input(void)
{
   FCEUI_SetInput(0, SI_GAMEPAD, &JSReturn[0], 0);
   FCEUI_SetInput(1, SI_GAMEPAD, &JSReturn[0], 0);
}

static void emulator_set_custom_palette (void)
{
   uint8 i,r,g,b;

   if (current_palette == 0)
   {
      FCEU_ResetPalette();	/* Do palette reset*/
      return;
   }

   /* Setup this palette*/

   for ( i = 0; i < 64; i++ )
   {
      r = palettes[current_palette-1].data[i] >> 16;
      g = ( palettes[current_palette-1].data[i] & 0xff00 ) >> 8;
      b = ( palettes[current_palette-1].data[i] & 0xff );
      FCEUD_SetPalette( i, r, g, b);
      FCEUD_SetPalette( i+64, r, g, b);
      FCEUD_SetPalette( i+128, r, g, b);
      FCEUD_SetPalette( i+192, r, g, b);
   }
}

static bool fceu_init(const char * full_path)
{
   char* dir=NULL;
   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      FCEUI_SetBaseDirectory(dir);

   FCEUI_Initialize();

   FCEUI_SetSoundVolume(256);
   FCEUI_Sound(32050);

   GameInfo = (FCEUGI*)FCEUI_LoadGame(full_path);
   if (!GameInfo)
      return false;

   emulator_set_input();
   emulator_set_custom_palette();

   FCEUD_SoundToggle();
   return true;
}

void retro_deinit (void)
{
   FCEUI_CloseGame();
   FCEUI_Sound(0);
   FCEUI_Kill();
}

void retro_reset(void)
{
   ResetNES();
}


typedef struct
{
   unsigned retro;
   unsigned nes;
} keymap;

static const keymap bindmap[] = {
   { RETRO_DEVICE_ID_JOYPAD_A, JOY_A },
   { RETRO_DEVICE_ID_JOYPAD_B, JOY_B },
   { RETRO_DEVICE_ID_JOYPAD_SELECT, JOY_SELECT },
   { RETRO_DEVICE_ID_JOYPAD_START, JOY_START },
   { RETRO_DEVICE_ID_JOYPAD_UP, JOY_UP },
   { RETRO_DEVICE_ID_JOYPAD_DOWN, JOY_DOWN },
   { RETRO_DEVICE_ID_JOYPAD_LEFT, JOY_LEFT },
   { RETRO_DEVICE_ID_JOYPAD_RIGHT, JOY_RIGHT },
};

static void check_variables(void)
{
   struct retro_variable var = {0};

   var.key = "nes_palette";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned orig_value = current_palette;

      if (strcmp(var.value, "asqrealc") == 0)
         current_palette = 0;
      else if (strcmp(var.value, "loopy") == 0)
         current_palette = 1;
      else if (strcmp(var.value, "quor") == 0)
         current_palette = 2;
      else if (strcmp(var.value, "chris") == 0)
         current_palette = 3;
      else if (strcmp(var.value, "matt") == 0)
         current_palette = 4;
      else if (strcmp(var.value, "matt") == 0)
         current_palette = 5;
      else if (strcmp(var.value, "pasofami") == 0)
         current_palette = 6;
      else if (strcmp(var.value, "crashman") == 0)
         current_palette = 7;
      else if (strcmp(var.value, "mess") == 0)
         current_palette = 8;
      else if (strcmp(var.value, "zaphod-cv") == 0)
         current_palette = 9;
      else if (strcmp(var.value, "zaphod-smb") == 0)
         current_palette = 10;
      else if (strcmp(var.value, "vs-drmar") == 0)
         current_palette = 11;
      else if (strcmp(var.value, "vs-cv") == 0)
         current_palette = 12;
      else if (strcmp(var.value, "vs-smb") == 0)
         current_palette = 13;

      if (current_palette != orig_value)
         emulator_set_custom_palette();
   }
}

static void FCEUD_UpdateInput(void)
{
   unsigned i;
   unsigned char pad[2];

   pad[0] = 0;
   pad[1] = 0;

   poll_cb();

   for ( i = 0; i < 8; i++)
      pad[0] |= input_cb(0, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro) ? bindmap[i].nes : 0;

   for ( i = 0; i < 8; i++)
      pad[1] |= input_cb(1, RETRO_DEVICE_JOYPAD, 0, bindmap[i].retro) ? bindmap[i].nes : 0;

   JSReturn[0] = pad[0] | (pad[1] << 8);
}

void FCEUD_WriteSoundData(int32 *Buffer, int Count)
{
   int y;
   for (y = 0; y < Count; y++)
      Buffer[y] = (Buffer[y] << 16) | (Buffer[y] & 0xffff);

   audio_batch_cb((const int16_t*)Buffer, Count);
}

void FCEUD_Update(uint8 *XBuf, int32 *Buffer, int Count)
{
}


void retro_run(void)
{
   unsigned width, height, pitch, x, y;
#ifndef PSP
   static uint16_t video_out[256 * 240];
#endif
   uint8_t *gfx;
   int32 ssize;
   bool updated;

   ssize = 0;
   updated = false;

   FCEUI_Emulate(&gfx, &sound, &ssize, 0);   

   if (ssize)
      FCEUD_WriteSoundData(sound, ssize);
#ifdef PSP
   static unsigned int __attribute__((aligned(16))) d_list[32];
   void* const texture_vram_p = (void*) (0x44200000 - (256 * 256)); // max VRAM address - frame size

   sceKernelDcacheWritebackRange(retro_palette,256 * 2);
   sceKernelDcacheWritebackRange(XBuf, 256*240 );

   sceGuStart(GU_DIRECT, d_list);

   // sceGuCopyImage doesnt seem to work correctly with GU_PSM_T8
   // so we use GU_PSM_4444 ( 2 Bytes per pixel ) instead
   // with half the values for pitch / width / x offset
   if (use_overscan)
      sceGuCopyImage(GU_PSM_4444, 0, 0, 128, 240, 128, XBuf, 0, 0, 128, texture_vram_p);
   else
      sceGuCopyImage(GU_PSM_4444, 4, 4, 120, 224, 128, XBuf, 0, 0, 128, texture_vram_p);

   sceGuTexSync();
   sceGuTexImage(0, 256, 256, 256, texture_vram_p);
   sceGuTexMode(GU_PSM_T8, 0, 0, GU_FALSE);
   sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
   sceGuDisable(GU_BLEND);
   sceGuClutMode(GU_PSM_5650, 0, 0xFF, 0);
   sceGuClutLoad(32, retro_palette);

   sceGuFinish();
#endif

   if (use_overscan)
   {
      width = 256;
      height = 240;
      pitch = 256;
#ifndef PSP
      pitch = 512;
      gfx = XBuf;
      for (y = 0; y < height; y++)
         for ( x = 0; x < width; x++, gfx++)
            video_out[y * width + x] = retro_palette[*gfx];
#endif
   }
   else
   {
      width = 256 - 16;
      height = 240 - 16;
      pitch = 256;
#ifndef PSP
      pitch = 512 - 32;
      gfx = XBuf + 8 + 256 * 8;
      for (y = 0; y < height; y++, gfx += 16)
         for ( x = 0; x < width; x++, gfx++)
            video_out[y * width + x] = retro_palette[*gfx];
#endif
   }
   
#ifdef PSP
   video_cb(texture_vram_p, width, height, pitch);
#else
   video_cb(video_out, width, height, pitch);
#endif
	FCEUD_UpdateInput();

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
}

static unsigned serialize_size = 0;

size_t retro_serialize_size(void)
{
   if (serialize_size == 0)
   {
      /* Something arbitrarily big.*/
      uint8_t *buffer = (uint8_t*)malloc(1000000);
      memstream_set_buffer(buffer, 1000000);

      FCEUSS_Save("");
      serialize_size = memstream_get_last_size();
      free(buffer);
   }

   return serialize_size;
}

bool retro_serialize(void *data, size_t size)
{
   if (size != retro_serialize_size())
      return false;

   memstream_set_buffer((uint8_t*)data, size);
   FCEUSS_Save("");
   return true;
}

bool retro_unserialize(const void * data, size_t size)
{
   if (size != retro_serialize_size())
      return false;

   memstream_set_buffer((uint8_t*)data, size);
   FCEUSS_Load("");
   return true;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned a, bool b, const char* c)
{ }

bool retro_load_game(const struct retro_game_info *game)
{
   if (!fceu_init(game->path))
      return false;
   check_variables();

   if (!environ_cb(RETRO_ENVIRONMENT_GET_OVERSCAN, &use_overscan))
      use_overscan = true;

   return true;
}

bool retro_load_game_special(
  unsigned game_type,
  const struct retro_game_info *info, size_t num_info
)
{
   return false;
}

void retro_unload_game(void)
{
   FCEUI_CloseGame();
}

unsigned retro_get_region(void)
{
   return FSettings.PAL ? RETRO_REGION_PAL : RETRO_REGION_NTSC;
}

void *retro_get_memory_data(unsigned id)
{
   if (id != RETRO_MEMORY_SAVE_RAM)
      return NULL;

   if (iNESCart.battery)
	   return iNESCart.SaveGame[0];
   if (UNIFCart.battery)
      return UNIFCart.SaveGame[0];

   return 0;
}

size_t retro_get_memory_size(unsigned id)
{
   if (id != RETRO_MEMORY_SAVE_RAM)
      return 0;

   if (iNESCart.battery)
      return iNESCart.SaveGameLen[0];
   if (UNIFCart.battery)
      return UNIFCart.SaveGameLen[0];

   return 0;
}
