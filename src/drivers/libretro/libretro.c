#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include <libretro.h>
#include <streams/memory_stream.h>
#include <libretro_dipswitch.h>
#include <libretro_core_options.h>

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
#include "../../vsuni.h"
#include "../../video.h"

#ifdef PSP
#include "pspgu.h"
#endif

#if defined(RENDER_GSKIT_PS2)
#include "libretro-common/include/libretro_gskit_ps2.h"
#endif

#define MAX_PLAYERS 4 /* max supported players */
#define MAX_PORTS 2   /* max controller ports,
                       * port 0 for player 1/3, port 1 for player 2/4 */

#define RETRO_DEVICE_AUTO        RETRO_DEVICE_JOYPAD
#define RETRO_DEVICE_GAMEPAD     RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 1)
#define RETRO_DEVICE_ZAPPER      RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE,  0)
#define RETRO_DEVICE_ARKANOID    RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE,  1)

#define RETRO_DEVICE_FC_ARKANOID RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE,  2)
#define RETRO_DEVICE_FC_OEKAKIDS RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE,  3)
#define RETRO_DEVICE_FC_SHADOW   RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_MOUSE,  4)
#define RETRO_DEVICE_FC_4PLAYERS RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 2)
#define RETRO_DEVICE_FC_AUTO     RETRO_DEVICE_JOYPAD

#define NES_WIDTH   256
#define NES_HEIGHT  240
#define NES_8_7_PAR  ((width * (8.0 / 7.0)) / height)
#define NES_4_3      ((width / (height * (256.0 / 240.0))) * 4.0 / 3.0)

#if defined(_3DS)
void* linearMemAlign(size_t size, size_t alignment);
void linearFree(void* mem);
#endif

#if defined(RENDER_GSKIT_PS2)
RETRO_HW_RENDER_INTEFACE_GSKIT_PS2 *ps2 = NULL;
#endif

extern void FCEU_ZapperSetTolerance(int t);

static retro_video_refresh_t video_cb = NULL;
static retro_input_poll_t poll_cb = NULL;
static retro_input_state_t input_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
retro_environment_t environ_cb = NULL;
#ifdef PSP
static bool crop_overscan;
#else
static bool crop_overscan_h;
static bool crop_overscan_v;
#endif

static bool use_raw_palette;
static bool use_par;

/*
 * Flags to keep track of whether turbo
 * buttons toggled on or off.
 *
 * There are two values in array
 * for Turbo A and Turbo B for
 * each player
 */

#define MAX_BUTTONS 8
#define TURBO_BUTTONS 2
unsigned char turbo_button_toggle[MAX_PLAYERS][TURBO_BUTTONS] = { {0} };

typedef struct
{
   unsigned retro;
   unsigned nes;
} keymap;

static const keymap turbomap[] = {
   { RETRO_DEVICE_ID_JOYPAD_X, JOY_A },
   { RETRO_DEVICE_ID_JOYPAD_Y, JOY_B },
};

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

typedef struct {
   bool enable_4player;                /* four-score / 4-player adapter used */
   bool up_down_allowed;               /* disabled simultaneous up+down and left+right dpad combinations */

   /* turbo related */
   uint32_t turbo_enabler[MAX_PLAYERS];
   uint32_t turbo_delay;

   uint32_t type[MAX_PLAYERS + 1];     /* 4-players + famicom expansion */

   /* input data */
   uint32_t JSReturn;                  /* player input data, 1 byte per player (1-4) */
   uint32_t MouseData[MAX_PORTS][3];   /* nes mouse data */
   uint32_t FamicomData[3];            /* Famicom expansion port data */
} NES_INPUT_T;

static NES_INPUT_T nes_input = { 0 };
enum RetroZapperInputModes{RetroLightgun, RetroMouse, RetroPointer};
static enum RetroZapperInputModes zappermode = RetroLightgun;

static bool libretro_supports_bitmasks = false;

/* emulator-specific variables */

const size_t PPU_BIT = 1ULL << 31ULL;

extern uint8 NTARAM[0x800], PALRAM[0x20], SPRAM[0x100], PPU[4];

/* overclock the console by adding dummy scanlines to PPU loop
 * disables DMC DMA and WaveHi filling for these dummies
 * doesn't work with new PPU */
unsigned overclock_enabled = -1;
unsigned overclocked = 0;
unsigned skip_7bit_overclocking = 1; /* 7-bit samples have priority over overclocking */
unsigned totalscanlines = 0;
unsigned normal_scanlines = 240;
unsigned extrascanlines = 0;
unsigned vblankscanlines = 0;
unsigned dendy = 0;

static unsigned systemRegion = 0;
static unsigned opt_region = 0;
static unsigned opt_showAdvSoundOptions = 0;
static unsigned opt_showAdvSystemOptions = 0;

int FCEUnetplay;

#if defined(PSP) || defined(PS2)
static __attribute__((aligned(16))) uint16_t retro_palette[256];
#else
static uint16_t retro_palette[256];
#endif
#if defined(RENDER_GSKIT_PS2)
static uint8_t* fceu_video_out;
#else
static uint16_t* fceu_video_out;
#endif

/* Some timing-related variables. */
static unsigned sndsamplerate;
static unsigned sndquality;
static unsigned sndvolume;
unsigned swapDuty;

static int32_t *sound = 0;
static uint32_t Dummy = 0;
static uint32_t current_palette = 0;
static unsigned serialize_size;

int PPUViewScanline=0;
int PPUViewer=0;

/* extern forward decls.*/
extern FCEUGI *GameInfo;
extern uint8 *XBuf;
extern CartInfo iNESCart;
extern CartInfo UNIFCart;
extern int show_crosshair;
extern int option_ramstate;

/* emulator-specific callback functions */

void UpdatePPUView(int refreshchr) { }

const char * GetKeyboard(void)
{
   return "";
}

#define BUILD_PIXEL_RGB565(R,G,B) (((int) ((R)&0x1f) << RED_SHIFT) | ((int) ((G)&0x3f) << GREEN_SHIFT) | ((int) ((B)&0x1f) << BLUE_SHIFT))

#if defined (PSP)
#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 11
#define RED_EXPAND 3
#define GREEN_EXPAND 2
#define BLUE_EXPAND 3
#elif defined (FRONTEND_SUPPORTS_ABGR1555)
#define RED_SHIFT 0
#define GREEN_SHIFT 5
#define BLUE_SHIFT 10
#define RED_EXPAND 3
#define GREEN_EXPAND 3
#define BLUE_EXPAND 3
#define RED_MASK 0x1F
#define GREEN_MASK 0x3E0
#define BLUE_MASK 0x7C00
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

void FCEUD_SetPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
   unsigned char index_to_write = index;
#if defined(RENDER_GSKIT_PS2)
   /* Index correction for PS2 GS */
   int modi = index & 63;
   if ((modi >= 8 && modi < 16) || (modi >= 40 && modi < 48)) {
      index_to_write += 8;
   } else if ((modi >= 16 && modi < 24) || (modi >= 48 && modi < 56)) {
         index_to_write -= 8;
   }
#endif

#ifdef FRONTEND_SUPPORTS_RGB565
   retro_palette[index_to_write] = BUILD_PIXEL_RGB565(r >> RED_EXPAND, g >> GREEN_EXPAND, b >> BLUE_EXPAND);
#else
   retro_palette[index_to_write] =
      ((r >> RED_EXPAND) << RED_SHIFT) | ((g >> GREEN_EXPAND) << GREEN_SHIFT) | ((b >> BLUE_EXPAND) << BLUE_SHIFT);
#endif
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

void FCEUD_DispMessage(char *m)
{  struct retro_message msg;
   msg.msg = m;
   msg.frames = 180;
   environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, &msg);
}

void FCEUD_NetworkClose(void)
{ }

void FCEUD_SoundToggle (void)
{
   FCEUI_SetSoundVolume(sndvolume);
}

FILE *FCEUD_UTF8fopen(const char *n, const char *m)
{
   if (n)
      return fopen(n, m);
   else
      return NULL;
}

/*palette for FCEU*/
#define PAL_TOTAL   16 /* total no. of palettes in palettes[] */
#define PAL_DEFAULT (PAL_TOTAL + 1)
#define PAL_RAW     (PAL_TOTAL + 2)
#define PAL_CUSTOM  (PAL_TOTAL + 3)

static int external_palette_exist = 0;
extern int ipalette;

/* table for currently loaded palette */
static uint8_t base_palette[192];

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
   { "nintendo-vc", "Virtual Console palette",
      { 0x494949, 0x00006a, 0x090063, 0x290059,
         0x42004a, 0x490000, 0x420000, 0x291100,
         0x182700, 0x003010, 0x003000, 0x002910,
         0x012043, 0x000000, 0x000000, 0x000000,
         0x747174, 0x003084, 0x3101ac, 0x4b0194,
         0x64007b, 0x6b0039, 0x6b2101, 0x5a2f00,
         0x424900, 0x185901, 0x105901, 0x015932,
         0x01495a, 0x101010, 0x000000, 0x000000,
         0xadadad, 0x4a71b6, 0x6458d5, 0x8450e6,
         0xa451ad, 0xad4984, 0xb5624a, 0x947132,
         0x7b722a, 0x5a8601, 0x388e31, 0x318e5a,
         0x398e8d, 0x383838, 0x000000, 0x000000,
         0xb6b6b6, 0x8c9db5, 0x8d8eae, 0x9c8ebc,
         0xa687bc, 0xad8d9d, 0xae968c, 0x9c8f7c,
         0x9c9e72, 0x94a67c, 0x84a77b, 0x7c9d84,
         0x73968d, 0xdedede, 0x000000, 0x000000 }
   },
   { "rgb", "Nintendo RGB PPU palette",
      { 0x6D6D6D, 0x002492, 0x0000DB, 0x6D49DB,
         0x92006D, 0xB6006D, 0xB62400, 0x924900,
         0x6D4900, 0x244900, 0x006D24, 0x009200,
         0x004949, 0x000000, 0x000000, 0x000000,
         0xB6B6B6, 0x006DDB, 0x0049FF, 0x9200FF,
         0xB600FF, 0xFF0092, 0xFF0000, 0xDB6D00,
         0x926D00, 0x249200, 0x009200, 0x00B66D,
         0x009292, 0x242424, 0x000000, 0x000000,
         0xFFFFFF, 0x6DB6FF, 0x9292FF, 0xDB6DFF,
         0xFF00FF, 0xFF6DFF, 0xFF9200, 0xFFB600,
         0xDBDB00, 0x6DDB00, 0x00FF00, 0x49FFDB,
         0x00FFFF, 0x494949, 0x000000, 0x000000,
         0xFFFFFF, 0xB6DBFF, 0xDBB6FF, 0xFFB6FF,
         0xFF92FF, 0xFFB6B6, 0xFFDB92, 0xFFFF49,
         0xFFFF6D, 0xB6FF49, 0x92FF6D, 0x49FFDB,
         0x92DBFF, 0x929292, 0x000000, 0x000000 }
   },
   { "yuv-v3", "FBX's YUV-V3 palette",
      { 0x666666, 0x002A88, 0x1412A7, 0x3B00A4,
         0x5C007E, 0x6E0040, 0x6C0700, 0x561D00,
         0x333500, 0x0C4800, 0x005200, 0x004C18,
         0x003E5B, 0x000000, 0x000000, 0x000000,
         0xADADAD, 0x155FD9, 0x4240FF, 0x7527FE,
         0xA01ACC, 0xB71E7B, 0xB53120, 0x994E00,
         0x6B6D00, 0x388700, 0x0D9300, 0x008C47,
         0x007AA0, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x64B0FF, 0x9290FF, 0xC676FF,
         0xF26AFF, 0xFF6ECC, 0xFF8170, 0xEA9E22,
         0xBCBE00, 0x88D800, 0x5CE430, 0x45E082,
         0x48CDDE, 0x4F4F4F, 0x000000, 0x000000,
         0xFFFFFF, 0xC0DFFF, 0xD3D2FF, 0xE8C8FF,
         0xFAC2FF, 0xFFC4EA, 0xFFCCC5, 0xF7D8A5,
         0xE4E594, 0xCFEF96, 0xBDF4AB, 0xB3F3CC,
         0xB5EBF2, 0xB8B8B8, 0x000000, 0x000000 }
   },
   { "unsaturated-final", "FBX's Unsaturated-Final palette",
      { 0x676767, 0x001F8E, 0x23069E, 0x40008E,
         0x600067, 0x67001C, 0x5B1000, 0x432500,
         0x313400, 0x074800, 0x004F00, 0x004622,
         0x003A61, 0x000000, 0x000000, 0x000000,
         0xB3B3B3, 0x205ADF, 0x5138FB, 0x7A27EE,
         0xA520C2, 0xB0226B, 0xAD3702, 0x8D5600,
         0x6E7000, 0x2E8A00, 0x069200, 0x008A47,
         0x037B9B, 0x101010, 0x000000, 0x000000,
         0xFFFFFF, 0x62AEFF, 0x918BFF, 0xBC78FF,
         0xE96EFF, 0xFC6CCD, 0xFA8267, 0xE29B26,
         0xC0B901, 0x84D200, 0x58DE38, 0x46D97D,
         0x49CED2, 0x494949, 0x000000, 0x000000,
         0xFFFFFF, 0xC1E3FF, 0xD5D4FF, 0xE7CCFF,
         0xFBC9FF, 0xFFC7F0, 0xFFD0C5, 0xF8DAAA,
         0xEBE69A, 0xD1F19A, 0xBEF7AF, 0xB6F4CD,
         0xB7F0EF, 0xB2B2B2, 0x000000, 0x000000 }
   },
   { "sony-cxa2025as-us", "Sony CXA2025AS US palette",
      { 0x585858, 0x00238C, 0x00139B, 0x2D0585,
         0x5D0052, 0x7A0017, 0x7A0800, 0x5F1800,
         0x352A00, 0x093900, 0x003F00, 0x003C22,
         0x00325D, 0x000000, 0x000000, 0x000000,
         0xA1A1A1, 0x0053EE, 0x153CFE, 0x6028E4,
         0xA91D98, 0xD41E41, 0xD22C00, 0xAA4400,
         0x6C5E00, 0x2D7300, 0x007D06, 0x007852,
         0x0069A9, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x1FA5FE, 0x5E89FE, 0xB572FE,
         0xFE65F6, 0xFE6790, 0xFE773C, 0xFE9308,
         0xC4B200, 0x79CA10, 0x3AD54A, 0x11D1A4,
         0x06BFFE, 0x424242, 0x000000, 0x000000,
         0xFFFFFF, 0xA0D9FE, 0xBDCCFE, 0xE1C2FE,
         0xFEBCFB, 0xFEBDD0, 0xFEC5A9, 0xFED18E,
         0xE9DE86, 0xC7E992, 0xA8EEB0, 0x95ECD9,
         0x91E4FE, 0xACACAC, 0x000000, 0x000000 }
   },
   { "pal", "PAL palette",
      { 0x808080, 0x0000BA, 0x3700BF, 0x8400A6,
         0xBB006A, 0xB7001E, 0xB30000, 0x912600,
         0x7B2B00, 0x003E00, 0x00480D, 0x003C22,
         0x002F66, 0x000000, 0x050505, 0x050505,
         0xC8C8C8, 0x0059FF, 0x443CFF, 0xB733CC,
         0xFE33AA, 0xFE375E, 0xFE371A, 0xD54B00,
         0xC46200, 0x3C7B00, 0x1D8415, 0x009566,
         0x0084C4, 0x111111, 0x090909, 0x090909,
         0xFEFEFE, 0x0095FF, 0x6F84FF, 0xD56FFF,
         0xFE77CC, 0xFE6F99, 0xFE7B59, 0xFE915F,
         0xFEA233, 0xA6BF00, 0x51D96A, 0x4DD5AE,
         0x00D9FF, 0x666666, 0x0D0D0D, 0x0D0D0D,
         0xFEFEFE, 0x84BFFF, 0xBBBBFF, 0xD0BBFF,
         0xFEBFEA, 0xFEBFCC, 0xFEC4B7, 0xFECCAE,
         0xFED9A2, 0xCCE199, 0xAEEEB7, 0xAAF8EE,
         0xB3EEFF, 0xDDDDDD, 0x111111, 0x111111 }
   },
   { "bmf-final2", "BMF's Final 2 palette",
      { 0x525252, 0x000080, 0x08008A, 0x2C007E,
         0x4A004E, 0x500006, 0x440000, 0x260800,
         0x0A2000, 0x002E00, 0x003200, 0x00260A,
         0x001C48, 0x000000, 0x000000, 0x000000,
         0xA4A4A4, 0x0038CE, 0x3416EC, 0x5E04DC,
         0x8C00B0, 0x9A004C, 0x901800, 0x703600,
         0x4C5400, 0x0E6C00, 0x007400, 0x006C2C,
         0x005E84, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x4C9CFF, 0x7C78FF, 0xA664FF,
         0xDA5AFF, 0xF054C0, 0xF06A56, 0xD68610,
         0xBAA400, 0x76C000, 0x46CC1A, 0x2EC866,
         0x34C2BE, 0x3A3A3A, 0x000000, 0x000000,
         0xFFFFFF, 0xB6DAFF, 0xC8CAFF, 0xDAC2FF,
         0xF0BEFF, 0xFCBCEE, 0xFAC2C0, 0xF2CCA2,
         0xE6DA92, 0xCCE68E, 0xB8EEA2, 0xAEEABE,
         0xAEE8E2, 0xB0B0B0, 0x000000, 0x000000 }
   },
   { "bmf-final3", "BMF's Final 3 palette",
      { 0x686868, 0x001299, 0x1A08AA, 0x51029A,
         0x7E0069, 0x8E001C, 0x7E0301, 0x511800,
         0x1F3700, 0x014E00, 0x005A00, 0x00501C,
         0x004061, 0x000000, 0x000000, 0x000000,
         0xB9B9B9, 0x0C5CD7, 0x5035F0, 0x8919E0,
         0xBB0CB3, 0xCE0C61, 0xC02B0E, 0x954D01,
         0x616F00, 0x1F8B00, 0x01980C, 0x00934B,
         0x00819B, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x63B4FF, 0x9B91FF, 0xD377FF,
         0xEF6AFF, 0xF968C0, 0xF97D6C, 0xED9B2D,
         0xBDBD16, 0x7CDA1C, 0x4BE847, 0x35E591,
         0x3FD9DD, 0x606060, 0x000000, 0x000000,
         0xFFFFFF, 0xACE7FF, 0xD5CDFF, 0xEDBAFF,
         0xF8B0FF, 0xFEB0EC, 0xFDBDB5, 0xF9D28E,
         0xE8EB7C, 0xBBF382, 0x99F7A2, 0x8AF5D0,
         0x92F4F1, 0xBEBEBE, 0x000000, 0x000000 }
   },
   { "smooth-fbx", "FBX's Smooth palette",
      { 0x6A6D6A, 0x001380, 0x1E008A, 0x39007A,
         0x550056, 0x5A0018, 0x4F1000, 0x3D1C00,
         0x253200, 0x003D00, 0x004000, 0x003924,
         0x002E55, 0x000000, 0x000000, 0x000000,
         0xB9BCB9, 0x1850C7, 0x4B30E3, 0x7322D6,
         0x951FA9, 0x9D285C, 0x983700, 0x7F4C00,
         0x5E6400, 0x227700, 0x027E02, 0x007645,
         0x006E8A, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x68A6FF, 0x8C9CFF, 0xB586FF,
         0xD975FD, 0xE377B9, 0xE58D68, 0xD49D29,
         0xB3AF0C, 0x7BC211, 0x55CA47, 0x46CB81,
         0x47C1C5, 0x4A4D4A, 0x000000, 0x000000,
         0xFFFFFF, 0xCCEAFF, 0xDDDEFF, 0xECDAFF,
         0xF8D7FE, 0xFCD6F5, 0xFDDBCF, 0xF9E7B5,
         0xF1F0AA, 0xDAFAA9, 0xC9FFBC, 0xC3FBD7,
         0xC4F6F6, 0xBEC1BE, 0x000000, 0x000000 }
   },
   { "composite-direct-fbx", "FBX's Composite Direct palette",
      { 0x656565, 0x00127D, 0x18008E, 0x360082,
         0x56005D, 0x5A0018, 0x4F0500, 0x381900,
         0x1D3100, 0x003D00, 0x004100, 0x003B17,
         0x002E55, 0x000000, 0x000000, 0x000000,
         0xAFAFAF, 0x194EC8, 0x472FE3, 0x6B1FD7,
         0x931BAE, 0x9E1A5E, 0x993200, 0x7B4B00,
         0x5B6700, 0x267A00, 0x008200, 0x007A3E,
         0x006E8A, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x64A9FF, 0x8E89FF, 0xB676FF,
         0xE06FFF, 0xEF6CC4, 0xF0806A, 0xD8982C,
         0xB9B40A, 0x83CB0C, 0x5BD63F, 0x4AD17E,
         0x4DC7CB, 0x4C4C4C, 0x000000, 0x000000,
         0xFFFFFF, 0xC7E5FF, 0xD9D9FF, 0xE9D1FF,
         0xF9CEFF, 0xFFCCF1, 0xFFD4CB, 0xF8DFB1,
         0xEDEAA4, 0xD6F4A4, 0xC5F8B8, 0xBEF6D3,
         0xBFF1F1, 0xB9B9B9, 0x000000, 0x000000 }
   },
   { "pvm-style-d93-fbx", "FBX's PVM Style D93 palette",
      { 0x696B63, 0x001774, 0x1E0087, 0x340073,
         0x560057, 0x5E0013, 0x531A00, 0x3B2400,
         0x243000, 0x063A00, 0x003F00, 0x003B1E,
         0x00334E, 0x000000, 0x000000, 0x000000,
         0xB9BBB3, 0x1453B9, 0x4D2CDA, 0x671EDE,
         0x98189C, 0x9D2344, 0xA03E00, 0x8D5500,
         0x656D00, 0x2C7900, 0x008100, 0x007D42,
         0x00788A, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x69A8FF, 0x9691FF, 0xB28AFA,
         0xEA7DFA, 0xF37BC7, 0xF28E59, 0xE6AD27,
         0xD7C805, 0x90DF07, 0x64E53C, 0x45E27D,
         0x48D5D9, 0x4E5048, 0x000000, 0x000000,
         0xFFFFFF, 0xD2EAFF, 0xE2E2FF, 0xE9D8FF,
         0xF5D2FF, 0xF8D9EA, 0xFADEB9, 0xF9E89B,
         0xF3F28C, 0xD3FA91, 0xB8FCA8, 0xAEFACA,
         0xCAF3F3, 0xBEC0B8, 0x000000, 0x000000 }
   },
   { "ntsc-hardware-fbx", "FBX's NTSC Hardware palette",
      { 0x6A6D6A, 0x001380, 0x1E008A, 0x39007A,
         0x550056, 0x5A0018, 0x4F1000, 0x382100,
         0x213300, 0x003D00, 0x004000, 0x003924,
         0x002E55, 0x000000, 0x000000, 0x000000,
         0xB9BCB9, 0x1850C7, 0x4B30E3, 0x7322D6,
         0x951FA9, 0x9D285C, 0x963C00, 0x7A5100,
         0x5B6700, 0x227700, 0x027E02, 0x007645,
         0x006E8A, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x68A6FF, 0x9299FF, 0xB085FF,
         0xD975FD, 0xE377B9, 0xE58D68, 0xCFA22C,
         0xB3AF0C, 0x7BC211, 0x55CA47, 0x46CB81,
         0x47C1C5, 0x4A4D4A, 0x000000, 0x000000,
         0xFFFFFF, 0xCCEAFF, 0xDDDEFF, 0xECDAFF,
         0xF8D7FE, 0xFCD6F5, 0xFDDBCF, 0xF9E7B5,
         0xF1F0AA, 0xDAFAA9, 0xC9FFBC, 0xC3FBD7,
         0xC4F6F6, 0xBEC1BE, 0x000000, 0x000000 }
   },
   { "nes-classic-fbx-fs", "FBX's NES-Classic FS palette",
      { 0x60615F, 0x000083, 0x1D0195, 0x340875,
         0x51055E, 0x56000F, 0x4C0700, 0x372308,
         0x203A0B, 0x0F4B0E, 0x194C16, 0x02421E,
         0x023154, 0x000000, 0x000000, 0x000000,
         0xA9AAA8, 0x104BBF, 0x4712D8, 0x6300CA,
         0x8800A9, 0x930B46, 0x8A2D04, 0x6F5206,
         0x5C7114, 0x1B8D12, 0x199509, 0x178448,
         0x206B8E, 0x000000, 0x000000, 0x000000,
         0xFBFBFB, 0x6699F8, 0x8974F9, 0xAB58F8,
         0xD557EF, 0xDE5FA9, 0xDC7F59, 0xC7A224,
         0xA7BE03, 0x75D703, 0x60E34F, 0x3CD68D,
         0x56C9CC, 0x414240, 0x000000, 0x000000,
         0xFBFBFB, 0xBED4FA, 0xC9C7F9, 0xD7BEFA,
         0xE8B8F9, 0xF5BAE5, 0xF3CAC2, 0xDFCDA7,
         0xD9E09C, 0xC9EB9E, 0xC0EDB8, 0xB5F4C7,
         0xB9EAE9, 0xABABAB, 0x000000, 0x000000 }
   },
   { "nescap", "RGBSource's NESCAP palette",
      { 0x646365, 0x001580, 0x1D0090, 0x380082,
         0x56005D, 0x5A001A, 0x4F0900, 0x381B00,
         0x1E3100, 0x003D00, 0x004100, 0x003A1B,
         0x002F55, 0x000000, 0x000000, 0x000000,
         0xAFADAF, 0x164BCA, 0x472AE7, 0x6B1BDB,
         0x9617B0, 0x9F185B, 0x963001, 0x7B4800,
         0x5A6600, 0x237800, 0x017F00, 0x00783D,
         0x006C8C, 0x000000, 0x000000, 0x000000,
         0xFFFFFF, 0x60A6FF, 0x8F84FF, 0xB473FF,
         0xE26CFF, 0xF268C3, 0xEF7E61, 0xD89527,
         0xBAB307, 0x81C807, 0x57D43D, 0x47CF7E,
         0x4BC5CD, 0x4C4B4D, 0x000000, 0x000000,
         0xFFFFFF, 0xC2E0FF, 0xD5D2FF, 0xE3CBFF,
         0xF7C8FF, 0xFEC6EE, 0xFECEC6, 0xF6D7AE,
         0xE9E49F, 0xD3ED9D, 0xC0F2B2, 0xB9F1CC,
         0xBAEDED, 0xBAB9BB, 0x000000, 0x000000 }
   },
   { "wavebeam", "nakedarthur's Wavebeam palette",
      { 0X6B6B6B, 0X001B88, 0X21009A, 0X40008C,
         0X600067, 0X64001E, 0X590800, 0X481600,
         0X283600, 0X004500, 0X004908, 0X00421D,
         0X003659, 0X000000, 0X000000, 0X000000,
         0XB4B4B4, 0X1555D3, 0X4337EF, 0X7425DF,
         0X9C19B9, 0XAC0F64, 0XAA2C00, 0X8A4B00,
         0X666B00, 0X218300, 0X008A00, 0X008144,
         0X007691, 0X000000, 0X000000, 0X000000,
         0XFFFFFF, 0X63B2FF, 0X7C9CFF, 0XC07DFE,
         0XE977FF, 0XF572CD, 0XF4886B, 0XDDA029,
         0XBDBD0A, 0X89D20E, 0X5CDE3E, 0X4BD886,
         0X4DCFD2, 0X525252, 0X000000, 0X000000,
         0XFFFFFF, 0XBCDFFF, 0XD2D2FF, 0XE1C8FF,
         0XEFC7FF, 0XFFC3E1, 0XFFCAC6, 0XF2DAAD,
         0XEBE3A0, 0XD2EDA2, 0XBCF4B4, 0XB5F1CE,
         0XB6ECF1, 0XBFBFBF, 0X000000, 0X000000 }
   }
};

#ifdef HAVE_NTSC_FILTER
/* ntsc */
#include "nes_ntsc.h"
#define NTSC_NONE       0
#define NTSC_COMPOSITE  1
#define NTSC_SVIDEO     2
#define NTSC_RGB        3
#define NTSC_MONOCHROME 4

#define NES_NTSC_WIDTH  (((NES_NTSC_OUT_WIDTH(256) + 3) >> 2) << 2)

static unsigned use_ntsc = 0;
static unsigned burst_phase;
static nes_ntsc_t nes_ntsc;
static nes_ntsc_setup_t ntsc_setup;
static uint16_t *ntsc_video_out = NULL; /* for ntsc blit buffer */

static void NTSCFilter_Cleanup(void)
{
   if (ntsc_video_out)
      free(ntsc_video_out);
   ntsc_video_out = NULL;
}

static void NTSCFilter_Init(void)
{
   memset(&nes_ntsc, 0, sizeof(nes_ntsc));
   memset(&ntsc_setup, 0, sizeof(ntsc_setup));
   ntsc_video_out = (uint16_t *)malloc(NES_NTSC_WIDTH * NES_HEIGHT * sizeof(uint16_t));
}

static void NTSCFilter_Setup(void)
{
   if (ntsc_video_out == NULL)
      NTSCFilter_Init();

   switch (use_ntsc) {
   case NTSC_COMPOSITE:
      ntsc_setup = nes_ntsc_composite;
      break;
   case NTSC_SVIDEO:
      ntsc_setup = nes_ntsc_svideo;
      break;
   case NTSC_RGB:
      ntsc_setup = nes_ntsc_rgb;
      break;
   case NTSC_MONOCHROME:
      ntsc_setup = nes_ntsc_monochrome;
      break;
   default:
      break;
   }

   ntsc_setup.merge_fields = 0;
   if ((GameInfo->type != GIT_VSUNI) && (current_palette == PAL_DEFAULT || current_palette == PAL_RAW))
      /* use ntsc default palette instead of internal default palette for that "identity" effect */
      ntsc_setup.base_palette = NULL;
   else
      /* use internal palette, this includes palette presets, external palette and custom palettes
          * for VS. System games */
      ntsc_setup.base_palette = (unsigned char const *)palo;

   nes_ntsc_init(&nes_ntsc, &ntsc_setup);
}
#endif /* HAVE_NTSC_FILTER */

static void ResetPalette(void);
static void retro_set_custom_palette(void);

static void ResetPalette(void)
{
   retro_set_custom_palette();
#ifdef HAVE_NTSC_FILTER
   NTSCFilter_Setup();
#endif
}

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

static void update_nes_controllers(unsigned port, unsigned device)
{
   nes_input.type[port] = device;

   if (port < 4)
   {
      switch (device)
      {
      case RETRO_DEVICE_NONE:
         FCEUI_SetInput(port, SI_NONE, &Dummy, 0);
         FCEU_printf(" Player %u: None Connected\n", port + 1);
         break;
      case RETRO_DEVICE_ZAPPER:
         FCEUI_SetInput(port, SI_ZAPPER, nes_input.MouseData[port], 1);
         FCEU_printf(" Player %u: Zapper\n", port + 1);
         break;
      case RETRO_DEVICE_ARKANOID:
         FCEUI_SetInput(port, SI_ARKANOID, nes_input.MouseData[port], 0);
         FCEU_printf(" Player %u: Arkanoid\n", port + 1);
         break;
      case RETRO_DEVICE_GAMEPAD:
      default:
         nes_input.type[port] = RETRO_DEVICE_GAMEPAD;
         FCEUI_SetInput(port, SI_GAMEPAD, &nes_input.JSReturn, 0);
         FCEU_printf(" Player %u: Gamepad\n", port + 1);
         break;
      }
   }

   if (port == 4)
   {
      switch (device)
      {
      case RETRO_DEVICE_FC_ARKANOID:
         FCEUI_SetInputFC(SIFC_ARKANOID, nes_input.FamicomData, 0);
         FCEU_printf(" Famicom Expansion: Arkanoid\n");
         break;
      case RETRO_DEVICE_FC_SHADOW:
         FCEUI_SetInputFC(SIFC_SHADOW, nes_input.FamicomData, 1);
         FCEU_printf(" Famicom Expansion: (Bandai) Hyper Shot\n");
         break;
      case RETRO_DEVICE_FC_OEKAKIDS:
         FCEUI_SetInputFC(SIFC_OEKAKIDS, nes_input.FamicomData, 1);
         FCEU_printf(" Famicom Expansion: Oeka Kids Tablet\n");
         break;
      case RETRO_DEVICE_FC_4PLAYERS:
         FCEUI_SetInputFC(SIFC_4PLAYER, &nes_input.JSReturn, 0);
         FCEU_printf(" Famicom Expansion: Famicom 4-Player Adapter\n");
         break;
      case RETRO_DEVICE_NONE:
      default:
         FCEUI_SetInputFC(SIFC_NONE, &Dummy, 0);
         FCEU_printf(" Famicom Expansion: None Connected\n");
         break;
      }
   }
}

static unsigned nes_to_libretro(int d)
{
   switch(d)
   {
   case SI_UNSET:
   case SI_GAMEPAD:
      return RETRO_DEVICE_GAMEPAD;
   case SI_NONE:
      return RETRO_DEVICE_NONE;
   case SI_ZAPPER:
      return RETRO_DEVICE_ZAPPER;
   case SI_ARKANOID:
      return RETRO_DEVICE_ARKANOID;
   }

   return (RETRO_DEVICE_GAMEPAD);
}

static unsigned fc_to_libretro(int d)
{
   switch(d)
   {
   case SIFC_UNSET:
   case SIFC_NONE:
      return RETRO_DEVICE_NONE;
   case SIFC_ARKANOID:
      return RETRO_DEVICE_FC_ARKANOID;
   case SIFC_SHADOW:
      return RETRO_DEVICE_FC_SHADOW;
   case SIFC_OEKAKIDS:
      return RETRO_DEVICE_FC_OEKAKIDS;
   case SIFC_4PLAYER:
      return RETRO_DEVICE_FC_4PLAYERS;
   }

   return (RETRO_DEVICE_NONE);
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   if (port < 5)
   {
      if (port < 2) /* player 1-2 */
      {
         if (device != RETRO_DEVICE_AUTO)
            update_nes_controllers(port, device);
         else
            update_nes_controllers(port, nes_to_libretro(GameInfo->input[port]));
      }
      else
      {
         if (port < 4) /* player 3-4 */
         {
            /* This section automatically enables 4players support
             * when player 3 or 4 used */

            nes_input.type[port] = RETRO_DEVICE_NONE;

            if (device == RETRO_DEVICE_AUTO)
            {
               if (nes_input.enable_4player)
                  nes_input.type[port] = RETRO_DEVICE_GAMEPAD;
            }
            else if (device == RETRO_DEVICE_GAMEPAD)
               nes_input.type[port] = RETRO_DEVICE_GAMEPAD;

            FCEU_printf(" Player %u: %s\n", port + 1,
               (nes_input.type[port] == RETRO_DEVICE_NONE) ? "None Connected" : "Gamepad");
         }
         else /* do famicom controllers here */
         {
            if (device != RETRO_DEVICE_FC_AUTO)
               update_nes_controllers(4, device);
            else
               update_nes_controllers(4, fc_to_libretro(GameInfo->inputfc));
         }

         if (nes_input.type[2] == RETRO_DEVICE_GAMEPAD
         || nes_input.type[3] == RETRO_DEVICE_GAMEPAD)
            FCEUI_DisableFourScore(0);
         else
            FCEUI_DisableFourScore(1);

         /* check if famicom 4player adapter is used */
         if (nes_input.type[4] == RETRO_DEVICE_FC_4PLAYERS)
            FCEUI_DisableFourScore(1);
      }
   }
}

static void set_variables(void)
{
   unsigned i = 0, index = 0;

   /* Initialize main core option struct */
   for (i = 0; i < MAX_CORE_OPTIONS; i++)
      option_defs_us[i] = option_defs_empty;

   /* Write common core options to main struct */
   while (option_defs_common[index].key) {
      option_defs_us[index] = option_defs_common[index];
      index++;
   }

   /* Append dipswitch settings to core options if available */
   index += set_dipswitch_variables(index, option_defs_us);
   option_defs_us[index] = option_defs_empty;

   libretro_set_core_options(environ_cb);
}

void retro_set_environment(retro_environment_t cb)
{
   static const struct retro_controller_description pads1[] = {
      { "Auto",    RETRO_DEVICE_AUTO },
      { "Gamepad", RETRO_DEVICE_GAMEPAD },
      { "Zapper",  RETRO_DEVICE_ZAPPER },
      { 0, 0 },
   };

   static const struct retro_controller_description pads2[] = {
      { "Auto",     RETRO_DEVICE_AUTO },
      { "Gamepad",  RETRO_DEVICE_GAMEPAD },
      { "Arkanoid", RETRO_DEVICE_ARKANOID },
      { "Zapper",   RETRO_DEVICE_ZAPPER },
      { 0, 0 },
   };

   static const struct retro_controller_description pads3[] = {
      { "Auto",     RETRO_DEVICE_AUTO },
      { "Gamepad",  RETRO_DEVICE_GAMEPAD },
      { 0, 0 },
   };

   static const struct retro_controller_description pads4[] = {
      { "Auto",     RETRO_DEVICE_AUTO },
      { "Gamepad",  RETRO_DEVICE_GAMEPAD },
      { 0, 0 },
   };

   static const struct retro_controller_description pads5[] = {
      { "Auto",                  RETRO_DEVICE_FC_AUTO },
      { "Arkanoid",              RETRO_DEVICE_FC_ARKANOID },
      { "(Bandai) Hyper Shot",   RETRO_DEVICE_FC_SHADOW },
      { "Oeka Kids Tablet",      RETRO_DEVICE_FC_OEKAKIDS },
      { "4-Player Adapter",      RETRO_DEVICE_FC_4PLAYERS },
      { 0, 0 },
   };

   static const struct retro_controller_info ports[] = {
      { pads1, 3 },
      { pads2, 4 },
      { pads3, 2 },
      { pads4, 2 },
      { pads5, 5 },
      { 0, 0 },
   };

   environ_cb = cb;
   environ_cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->need_fullpath    = false;
   info->valid_extensions = "fds|nes|unf|unif";
#ifdef GIT_VERSION
   info->library_version  = "(SVN)" GIT_VERSION;
#else
   info->library_version  = "(SVN)";
#endif
   info->library_name     = "FCEUmm";
   info->block_extract    = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
#ifdef PSP
   unsigned width  = NES_WIDTH  - (crop_overscan ? 16 : 0);
   unsigned height = NES_HEIGHT - (crop_overscan ? 16 : 0);
#else
   unsigned width  = NES_WIDTH  - (crop_overscan_h ? 16 : 0);
   unsigned height = NES_HEIGHT - (crop_overscan_v ? 16 : 0);
#endif
#ifdef HAVE_NTSC_FILTER
   info->geometry.base_width = (use_ntsc ? NES_NTSC_OUT_WIDTH(width) : width);
   info->geometry.max_width = (use_ntsc ? NES_NTSC_WIDTH : NES_WIDTH);
#else
   info->geometry.base_width = width;
   info->geometry.max_width = NES_WIDTH;
#endif
   info->geometry.base_height = height;
   info->geometry.max_height = NES_HEIGHT;
   info->geometry.aspect_ratio = (float)(use_par ? NES_8_7_PAR : NES_4_3);
   info->timing.sample_rate = (float)sndsamplerate;
   if (FSettings.PAL || dendy)
      info->timing.fps = 838977920.0/16777215.0;
   else
      info->timing.fps = 1008307711.0/16777215.0;
}

static void check_system_specs(void)
{
   /* TODO - when we get it running at fullspeed on PSP, set to 4 */
   unsigned level = 5;
   environ_cb(RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL, &level);
}

void retro_init(void)
{
   bool achievements = true;
   log_cb.log=default_logger;
   environ_cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log_cb);

   environ_cb(RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS, &achievements);

   if (environ_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
      libretro_supports_bitmasks = true;
}

static void retro_set_custom_palette(void)
{
   unsigned i;

   ipalette = 0;
   use_raw_palette = false;

   /* VS UNISystem uses internal palette presets regardless of options */
   if (GameInfo->type == GIT_VSUNI)
      FCEU_ResetPalette();

   /* Reset and choose between default internal or external custom palette */
   else if (current_palette == PAL_DEFAULT || current_palette == PAL_CUSTOM)
   {
      ipalette = external_palette_exist && (current_palette == PAL_CUSTOM);

      /* if ipalette is set to 1, external palette
       * is loaded, else it will load default NES palette.
       * FCEUI_SetPaletteArray() both resets the palette array to
       * internal default palette and then chooses which one to use. */
      FCEUI_SetPaletteArray( NULL );
   }

   /* setup raw palette */
   else if (current_palette == PAL_RAW)
   {
      pal color;
      use_raw_palette = true;
      for (i = 0; i < 64; i++)
      {
         color.r = (((i >> 0) & 0xF) * 255) / 15;
         color.g = (((i >> 4) & 0x3) * 255) / 3;
         color.b = 0;
         FCEUD_SetPalette( i, color.r, color.g, color.b);
      }
   }

   /* setup palette presets */
   else
   {
      unsigned *palette_data = palettes[current_palette].data;
      for ( i = 0; i < 64; i++ )
      {
         unsigned data = palette_data[i];
         base_palette[ i * 3 + 0 ] = ( data >> 16 ) & 0xff; /* red */
         base_palette[ i * 3 + 1 ] = ( data >>  8 ) & 0xff; /* green */
         base_palette[ i * 3 + 2 ] = ( data >>  0 ) & 0xff; /* blue */
      }
      FCEUI_SetPaletteArray( base_palette );
   }
}

/* Set variables for NTSC(1) / PAL(2) / Dendy(3)
 * Dendy has PAL framerate and resolution, but ~NTSC timings,
 * and has 50 dummy scanlines to force 50 fps.
 */
static void FCEUD_RegionOverride(unsigned region)
{
   unsigned pal = 0;
   unsigned d = 0;

   switch (region)
   {
      case 0: /* auto */
         d = (systemRegion >> 1) & 1;
         pal = systemRegion & 1;
         break;
      case 1: /* ntsc */
         FCEU_DispMessage("System: NTSC");
         break;
      case 2: /* pal */
         pal = 1;
         FCEU_DispMessage("System: PAL");
         break;
      case 3: /* dendy */
         d = 1;
         FCEU_DispMessage("System: Dendy");
         break;
   }

   dendy = d;
   FCEUI_SetVidSystem(pal);
   ResetPalette();
}

void retro_deinit (void)
{
   FCEUI_CloseGame();
   FCEUI_Sound(0);
   FCEUI_Kill();
#if defined(_3DS)
   linearFree(fceu_video_out);
#else
   if (fceu_video_out)
      free(fceu_video_out);
   fceu_video_out = NULL;
#endif
#if defined(RENDER_GSKIT_PS2)
   ps2 = NULL;
#endif
   libretro_supports_bitmasks = false;
   DPSW_Cleanup();
#ifdef HAVE_NTSC_FILTER
   NTSCFilter_Cleanup();
#endif
}

void retro_reset(void)
{
   ResetNES();
}

static void set_apu_channels(int chan)
{
   FSettings.SquareVolume[1] = (chan & 1) ? 256 : 0;
   FSettings.SquareVolume[0] = (chan & 2) ? 256 : 0;
   FSettings.TriangleVolume  = (chan & 3) ? 256 : 0;
   FSettings.NoiseVolume     = (chan & 4) ? 256 : 0;
   FSettings.PCMVolume       = (chan & 5) ? 256 : 0;
}

static void check_variables(bool startup)
{
   struct retro_variable var = {0};
   char key[256];
   int i, enable_apu;

   /* 1 = Performs only geometry update: e.g. overscans */
   /* 2 = Performs video/geometry update when needed and timing changes: e.g. region and filter change */
   int audio_video_updated = 0;

   var.key = "fceumm_ramstate";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "random"))
         option_ramstate = 2;
      else if (!strcmp(var.value, "fill $00"))
         option_ramstate = 1;
      else
         option_ramstate = 0;
   }

#ifdef HAVE_NTSC_FILTER
   var.key = "fceumm_ntsc_filter";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned orig_value = use_ntsc;
      if (strcmp(var.value, "disabled") == 0)
         use_ntsc = NTSC_NONE;
      else if (strcmp(var.value, "composite") == 0)
         use_ntsc = NTSC_COMPOSITE;
      else if (strcmp(var.value, "svideo") == 0)
         use_ntsc = NTSC_SVIDEO;
      else if (strcmp(var.value, "rgb") == 0)
         use_ntsc = NTSC_RGB;
      else if (strcmp(var.value, "monochrome") == 0)
         use_ntsc = NTSC_MONOCHROME;
      if (use_ntsc != orig_value)
      {
         ResetPalette();
         audio_video_updated = 2;
      }
   }
#endif /* HAVE_NTSC_FILTER */

   var.key = "fceumm_palette";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned orig_value = current_palette;

      if (!strcmp(var.value, "default"))
         current_palette = PAL_DEFAULT;
      else if (!strcmp(var.value, "raw"))
         current_palette = PAL_RAW;
      else if (!strcmp(var.value, "custom"))
         current_palette = PAL_CUSTOM;
      else if (!strcmp(var.value, "asqrealc"))
         current_palette = 0;
      else if (!strcmp(var.value, "nintendo-vc"))
         current_palette = 1;
      else if (!strcmp(var.value, "rgb"))
         current_palette = 2;
      else if (!strcmp(var.value, "yuv-v3"))
         current_palette = 3;
      else if (!strcmp(var.value, "unsaturated-final"))
         current_palette = 4;
      else if (!strcmp(var.value, "sony-cxa2025as-us"))
         current_palette = 5;
      else if (!strcmp(var.value, "pal"))
         current_palette = 6;
      else if (!strcmp(var.value, "bmf-final2"))
         current_palette = 7;
      else if (!strcmp(var.value, "bmf-final3"))
         current_palette = 8;
      else if (!strcmp(var.value, "smooth-fbx"))
         current_palette = 9;
      else if (!strcmp(var.value, "composite-direct-fbx"))
         current_palette = 10;
      else if (!strcmp(var.value, "pvm-style-d93-fbx"))
         current_palette = 11;
      else if (!strcmp(var.value, "ntsc-hardware-fbx"))
         current_palette = 12;
      else if (!strcmp(var.value, "nes-classic-fbx-fs"))
         current_palette = 13;
      else if (!strcmp(var.value, "nescap"))
         current_palette = 14;
      else if (!strcmp(var.value, "wavebeam"))
         current_palette = 15;

      if (current_palette != orig_value)
      {
         audio_video_updated = 1;
         ResetPalette();
      }  
   }

   var.key = "fceumm_up_down_allowed";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      nes_input.up_down_allowed = (!strcmp(var.value, "enabled")) ? true : false;
   else
      nes_input.up_down_allowed = false;

   var.key = "fceumm_nospritelimit";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      int no_sprite_limit = (!strcmp(var.value, "enabled")) ? 1 : 0;
      FCEUI_DisableSpriteLimitation(no_sprite_limit);
   }

   var.key = "fceumm_overclocking";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool do_reinit = false;

      if (!strcmp(var.value, "disabled")
            && overclock_enabled != 0)
      {
         skip_7bit_overclocking = 1;
         extrascanlines         = 0;
         vblankscanlines        = 0;
         overclock_enabled      = 0;
         do_reinit              = true;
      }
      else if (!strcmp(var.value, "2x-Postrender"))
      {
         skip_7bit_overclocking = 1;
         extrascanlines         = 266;
         vblankscanlines        = 0;
         overclock_enabled      = 1;
         do_reinit              = true;
      }
      else if (!strcmp(var.value, "2x-VBlank"))
      {
         skip_7bit_overclocking = 1;
         extrascanlines         = 0;
         vblankscanlines        = 266;
         overclock_enabled      = 1;
         do_reinit              = true;
      }

      normal_scanlines = dendy ? 290 : 240;
      totalscanlines = normal_scanlines + (overclock_enabled ? extrascanlines : 0);

      if (do_reinit && startup)
      {
         FCEU_KillVirtualVideo();
         FCEU_InitVirtualVideo();
      }
   }

   var.key = "fceumm_zapper_mode";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "mouse")) zappermode = RetroMouse;
      else if (!strcmp(var.value, "touchscreen")) zappermode = RetroPointer;
      else zappermode = RetroLightgun; /*default setting*/
   }

   var.key = "fceumm_zapper_tolerance";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      FCEU_ZapperSetTolerance(atoi(var.value));
   }

   var.key = "fceumm_region";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)

   var.key = "fceumm_show_crosshair";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "enabled")) show_crosshair = 1;
      else if (!strcmp(var.value, "disabled")) show_crosshair = 0;
   }

#ifdef PSP
   var.key = "fceumm_overscan";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "enabled"));
      if (newval != crop_overscan)
      {
         crop_overscan = newval;
         audio_video_updated = 1;
      }
   }

#else
   var.key = "fceumm_overscan_h";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "enabled"));
      if (newval != crop_overscan_h)
      {
         crop_overscan_h = newval;
         audio_video_updated = 1;
      }
   }

   var.key = "fceumm_overscan_v";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "enabled"));
      if (newval != crop_overscan_v)
      {
         crop_overscan_v = newval;
         audio_video_updated = 1;
      }
   }
#endif
   var.key = "fceumm_aspect";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "8:7 PAR"));
      if (newval != use_par)
      {
         use_par = newval;
         audio_video_updated = 1;
      }
   }

   var.key = "fceumm_turbo_enable";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      nes_input.turbo_enabler[0] = 0;
      nes_input.turbo_enabler[1] = 0;

      if (!strcmp(var.value, "Player 1"))
         nes_input.turbo_enabler[0] = 1;
      else if (!strcmp(var.value, "Player 2"))
         nes_input.turbo_enabler[1] = 1;
      else if (!strcmp(var.value, "Both"))
      {
         nes_input.turbo_enabler[0] = 1;
         nes_input.turbo_enabler[1] = 1;
      }
   }

   var.key = "fceumm_turbo_delay";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      nes_input.turbo_delay = atoi(var.value);

   var.key = "fceumm_region";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned oldval = opt_region;
      if (!strcmp(var.value, "Auto"))
         opt_region = 0;
      else if (!strcmp(var.value, "NTSC"))
         opt_region = 1;
      else if (!strcmp(var.value, "PAL"))
         opt_region = 2;
      else if (!strcmp(var.value, "Dendy"))
         opt_region = 3;
      if (opt_region != oldval)
      {
         FCEUD_RegionOverride(opt_region);
         audio_video_updated = 2;
      }
   }

   var.key = "fceumm_sndquality";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned oldval = sndquality;
      if (!strcmp(var.value, "Low"))
         sndquality = 0;
      else if (!strcmp(var.value, "High"))
         sndquality = 1;
      else if (!strcmp(var.value, "Very High"))
         sndquality = 2;
      if (sndquality != oldval)
         FCEUI_SetSoundQuality(sndquality);
   }

   var.key = "fceumm_sndvolume";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      int val = (int)(atof(var.value) * 25.6);
      sndvolume = val;
      FCEUD_SoundToggle();
   }

   if (audio_video_updated && !startup)
   {
      struct retro_system_av_info av_info;
      retro_get_system_av_info(&av_info);
      if (audio_video_updated == 2)  
         environ_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &av_info);
      else
         environ_cb(RETRO_ENVIRONMENT_SET_GEOMETRY, &av_info);
   }

   var.key = "fceumm_swapduty";

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      bool newval = (!strcmp(var.value, "enabled"));
      if (newval != swapDuty)
         swapDuty = newval;
   }

   var.key = key;

   enable_apu = 0xff;

   strcpy(key, "fceumm_apu_x");
   for (i = 0; i < 5; i++)
   {
      key[strlen("fceumm_apu_")] = '1' + i;
      var.value = NULL;
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && !strcmp(var.value, "disabled"))
         enable_apu &= ~(1 << i);
   }
   set_apu_channels(enable_apu);

   update_dipswitch();

   var.key = "fceumm_show_adv_system_options";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned newval = (!strcmp(var.value, "enabled")) ? 1 : 0;
      if ((opt_showAdvSystemOptions != newval) || startup)
      {
         struct retro_core_option_display option_display;
         unsigned i;
         unsigned size;
         char options_list[][25] = {
            "fceumm_overclocking",
            "fceumm_ramstate",
            "fceumm_nospritelimit",
            "fceumm_up_down_allowed",
            "fceumm_show_crosshair"
         };

         opt_showAdvSystemOptions = newval;
         option_display.visible = opt_showAdvSystemOptions;
         size = sizeof(options_list) / sizeof(options_list[0]);
         for (i = 0; i < size; i++)
         {
            option_display.key = options_list[i];
            environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
         }
      }
   }

   var.key = "fceumm_show_adv_sound_options";
   var.value = NULL;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      unsigned newval = (!strcmp(var.value, "enabled")) ? 1 : 0;
      if ((opt_showAdvSoundOptions != newval) || startup)
      {
         struct retro_core_option_display option_display;
         unsigned i;
         unsigned size;
         char options_list[][25] = {
            "fceumm_sndvolume",
            "fceumm_sndquality",
            "fceumm_swapduty",
            "fceumm_apu_1",
            "fceumm_apu_2",
            "fceumm_apu_3",
            "fceumm_apu_4",
            "fceumm_apu_5"
         };

         opt_showAdvSoundOptions = newval;
         option_display.visible  = opt_showAdvSoundOptions;
         size = sizeof(options_list) / sizeof(options_list[0]);
         for (i = 0; i < size; i++)
         {
            option_display.key = options_list[i];
            environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY, &option_display);
         }
      }
   }
}

static int mzx = 0, mzy = 0;

void get_mouse_input(unsigned port, uint32_t *zapdata)
{
   bool adjx = false;
   bool adjy = false;
   int min_width, min_height, max_width, max_height;

#ifdef PSP
   adjx = adjy = crop_overscan ? 1 : 0;
#else
   adjx        = crop_overscan_h ? 1 : 0;
   adjy        = crop_overscan_v ? 1 : 0;
#endif
   max_width   = 256;
   max_height  = 240;
   zapdata[2]  = 0; /* reset click state */

   if (zappermode == RetroMouse) /* mouse device */
   {
      min_width   = (adjx ? 8 : 0) + 1;
      min_height  = (adjy ? 8 : 0) + 1;
      max_width  -= (adjx ? 8 : 0);
      max_height -= (adjy ? 8 : 0);

      /* TODO: Add some sort of mouse sensitivity */
      mzx += input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
      mzy += input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);

      /* Set crosshair within the limits of current screen resolution */
      if (mzx < min_width) mzx = min_width;
      else if (mzx > max_width) mzx = max_width;

      if (mzy < min_height) mzy = min_height;
      else if (mzy > max_height) mzy = max_height;

      zapdata[0] = mzx;
      zapdata[1] = mzy;

      if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT))
         zapdata[2] |= 0x1;
      if (input_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT))
         zapdata[2] |= 0x2;
   }
   else if (zappermode == RetroPointer) {
      int offset_x = (adjx ? 0X8FF : 0);
      int offset_y = (adjy ? 0X999 : 0);

      int _x = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
      int _y = input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

      if (_x == 0 && _y == 0)
      {
         zapdata[0] = 0;
         zapdata[1] = 0;
      }
      else
      {
         zapdata[0] = (_x + (0x7FFF + offset_x)) * max_width  / ((0x7FFF + offset_x) * 2);
         zapdata[1] = (_y + (0x7FFF + offset_y)) * max_height  / ((0x7FFF + offset_y) * 2);
      }

      if (input_cb(port, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED))
         zapdata[2] |= 0x1;
   }
   else /* lightgun device */
   {
      int offset_x = (adjx ? 0X8FF : 0);
      int offset_y = (adjy ? 0X999 : 0);
      int offscreen;
      int offscreen_shot;
      int trigger;

      offscreen = input_cb( port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN );
      offscreen_shot = input_cb( port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_RELOAD );
      trigger = input_cb( port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER );

      if ( offscreen || offscreen_shot )
      {
         zapdata[0] = 0;
         zapdata[1] = 0;
      }
      else
      {
         int _x = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X);
         int _y = input_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y);

         zapdata[0] = (_x + (0x7FFF + offset_x)) * max_width  / ((0x7FFF + offset_x) * 2);
         zapdata[1] = (_y + (0x7FFF + offset_y)) * max_height  / ((0x7FFF + offset_y) * 2);
      }

      if ( trigger || offscreen_shot )
         zapdata[2] |= 0x1;
   }
}

static void FCEUD_UpdateInput(void)
{
   unsigned player, port;

   poll_cb();

   /* Reset input states */
   nes_input.JSReturn = 0;

   /* nes gamepad */
   for (player = 0; player < MAX_PLAYERS; player++)
   {
      int i              = 0;
      uint8_t input_buf  = 0;
      int player_enabled = (nes_input.type[player] == RETRO_DEVICE_GAMEPAD) || (nes_input.type[player] == RETRO_DEVICE_JOYPAD);

      if (player_enabled)
      {
         int16_t ret;

         if (libretro_supports_bitmasks)
         {
            ret = input_cb(player, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_MASK);

            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_A))
               input_buf |= JOY_A;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_B))
               input_buf |= JOY_B;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_SELECT))
               input_buf |= JOY_SELECT;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_START))
               input_buf |= JOY_START;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_UP))
               input_buf |= JOY_UP;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_DOWN))
               input_buf |= JOY_DOWN;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_LEFT))
               input_buf |= JOY_LEFT;
            if (ret & (1 << RETRO_DEVICE_ID_JOYPAD_RIGHT))
               input_buf |= JOY_RIGHT;
         }
         else
         {
            for (i = 0; i < MAX_BUTTONS; i++)
               input_buf |= input_cb(player, RETRO_DEVICE_JOYPAD, 0,
                     bindmap[i].retro) ? bindmap[i].nes : 0;
         }

         /* Turbo A and Turbo B buttons are
          * mapped to Joypad X and Joypad Y
          * in RetroArch joypad.
          *
          * We achieve this by keeping track of
          * the number of times it increments
          * the toggle counter and fire or not fire
          * depending on whether the delay value has
          * been reached.
          */

         if (nes_input.turbo_enabler[player])
         {
            /* Handle Turbo A & B buttons */
            for (i = 0; i < TURBO_BUTTONS; i++)
            {
               if (input_cb(player, RETRO_DEVICE_JOYPAD, 0, turbomap[i].retro))
               {
                  if (!turbo_button_toggle[player][i])
                     input_buf |= turbomap[i].nes;
                  turbo_button_toggle[player][i]++;
                  turbo_button_toggle[player][i] %= nes_input.turbo_delay + 1;
               }
               else
                  /* If the button is not pressed, just reset the toggle */
                  turbo_button_toggle[player][i] = 0;
            }
         }
      }

      if (!nes_input.up_down_allowed)
      {
         if (input_buf & (JOY_UP))
            if (input_buf & (JOY_DOWN))
               input_buf &= ~((JOY_UP ) | (JOY_DOWN));
         if (input_buf & (JOY_LEFT))
            if (input_buf & (JOY_RIGHT))
               input_buf &= ~((JOY_LEFT ) | (JOY_RIGHT));
      }

      nes_input.JSReturn |= (input_buf & 0xff) << (player << 3);
   }

   /* other inputs*/
   for (port = 0; port < MAX_PORTS; port++)
   {
      switch (nes_input.type[port])
      {
         case RETRO_DEVICE_ARKANOID:
         case RETRO_DEVICE_ZAPPER:
            get_mouse_input(port, nes_input.MouseData[port]);
            break;
      }
   }

   /* famicom inputs */
   switch (nes_input.type[4])
   {
      case RETRO_DEVICE_FC_ARKANOID:
      case RETRO_DEVICE_FC_OEKAKIDS:
      case RETRO_DEVICE_FC_SHADOW:
         get_mouse_input(0, nes_input.FamicomData);
         break;
   }

   if (input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2))
      FCEU_VSUniCoin();             /* Insert Coin VS System */

   if (GameInfo->type == GIT_FDS)   /* Famicom Disk System */
   {
      bool curL = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
      bool curR = input_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
      static bool prevL = false, prevR = false;

      if (curL && !prevL)
         FCEU_FDSSelect();          /* Swap FDisk side */
      prevL = curL;

      if (curR && !prevR)
         FCEU_FDSInsert(-1);        /* Insert or eject the disk */
      prevR = curR;
   }
}

void FCEUD_Update(uint8 *XBuf, int32 *Buffer, int Count)
{
}

static void retro_run_blit(uint8_t *gfx)
{
   unsigned x, y;
#ifdef PSP
   static unsigned int __attribute__((aligned(16))) d_list[32];
   void* texture_vram_p = NULL;
#endif
   unsigned incr   = 0;
   unsigned width  = 256;
   unsigned height = 240;
   unsigned pitch  = 512;

#ifdef PSP
   if (crop_overscan)
   {
      width  -= 16;
      height -= 16;
   }
   texture_vram_p = (void*) (0x44200000 - (256 * 256)); /* max VRAM address - frame size */

   sceKernelDcacheWritebackRange(retro_palette,256 * 2);
   sceKernelDcacheWritebackRange(XBuf, 256*240 );

   sceGuStart(GU_DIRECT, d_list);

   /* sceGuCopyImage doesnt seem to work correctly with GU_PSM_T8
    * so we use GU_PSM_4444 ( 2 Bytes per pixel ) instead
    * with half the values for pitch / width / x offset
    */
   if (crop_overscan)
      sceGuCopyImage(GU_PSM_4444, 4, 4, 120, 224, 128, XBuf, 0, 0, 128, texture_vram_p);
   else
      sceGuCopyImage(GU_PSM_4444, 0, 0, 128, 240, 128, XBuf, 0, 0, 128, texture_vram_p);

   sceGuTexSync();
   sceGuTexImage(0, 256, 256, 256, texture_vram_p);
   sceGuTexMode(GU_PSM_T8, 0, 0, GU_FALSE);
   sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
   sceGuDisable(GU_BLEND);
   sceGuClutMode(GU_PSM_5650, 0, 0xFF, 0);
   sceGuClutLoad(32, retro_palette);

   sceGuFinish();

   video_cb(texture_vram_p, width, height, 256);
#elif defined(RENDER_GSKIT_PS2)
   uint32_t *buf = (uint32_t *)RETRO_HW_FRAME_BUFFER_VALID;

   if (!ps2) {
      if (!environ_cb(RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE, (void **)&ps2) || !ps2) {
         printf("Failed to get HW rendering interface!\n");
         return;
      }

      if (ps2->interface_version != RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION) {
         printf("HW render interface mismatch, expected %u, got %u!\n",
                  RETRO_HW_RENDER_INTERFACE_GSKIT_PS2_VERSION, ps2->interface_version);
         return;
      }

      ps2->coreTexture->Width = width;
      ps2->coreTexture->Height = height;
      ps2->coreTexture->PSM = GS_PSM_T8;
      ps2->coreTexture->ClutPSM = GS_PSM_CT16;
      ps2->coreTexture->Filter = GS_FILTER_LINEAR;
      ps2->padding = (struct retro_hw_ps2_insets){ crop_overscan_v ? 8.0f : 0.0f,
                                                   crop_overscan_h ? 8.0f : 0.0f,
                                                   crop_overscan_v ? 8.0f : 0.0f,
                                                   crop_overscan_h ? 8.0f : 0.0f};
   }

   ps2->coreTexture->Clut = (u32*)retro_palette;
   ps2->coreTexture->Mem = (u32*)gfx;

   video_cb(buf, width, height, pitch);
#else
#ifdef HAVE_NTSC_FILTER
   if (use_ntsc)
   {
      burst_phase ^= 1;
      if (ntsc_setup.merge_fields)
         burst_phase = 0;

      nes_ntsc_blit(&nes_ntsc, (NES_NTSC_IN_T const*)gfx, (NES_NTSC_IN_T *)XDBuf,
          NES_WIDTH, burst_phase, NES_WIDTH, NES_HEIGHT,
          ntsc_video_out, NES_NTSC_WIDTH * sizeof(uint16));

      width    = NES_WIDTH - (crop_overscan_h ? 16 : 0);
      width    = NES_NTSC_OUT_WIDTH(width);
      height   = NES_HEIGHT - (crop_overscan_v ? 16 : 0);
      pitch    = width * sizeof(uint16_t);

      {
         int32_t h_offset   = crop_overscan_h ?  NES_NTSC_OUT_WIDTH(8) : 0;
         int32_t v_offset   = crop_overscan_v ? 8 : 0;
         const uint16_t *in = ntsc_video_out + h_offset + NES_NTSC_WIDTH * v_offset;
         uint16_t *out      = fceu_video_out;

         for (y = 0; y < height; y++)
         {
            memcpy(out, in, pitch);
            in += NES_NTSC_WIDTH;
            out += width;
         }
      }
      video_cb(fceu_video_out, width, height, pitch);
   }
   else
#endif /* HAVE_NTSC_FILTER */
   {
      incr   += (crop_overscan_h ? 16 : 0);
      width  -= (crop_overscan_h ? 16 : 0);
      height -= (crop_overscan_v ? 16 : 0);
      pitch  -= (crop_overscan_h ? 32 : 0);
      gfx    += (crop_overscan_v ? ((crop_overscan_h ? 8 : 0) + 256 * 8) : (crop_overscan_h ? 8 : 0));

      if (use_raw_palette)
      {
         uint8_t *deemp = XDBuf + (gfx - XBuf);
         for (y = 0; y < height; y++, gfx += incr, deemp += incr)
            for (x = 0; x < width; x++, gfx++, deemp++)
               fceu_video_out[y * width + x] = retro_palette[*gfx & 0x3F] | (*deemp << 2);
      }
      else
      {
         for (y = 0; y < height; y++, gfx += incr)
            for (x = 0; x < width; x++, gfx++)
               fceu_video_out[y * width + x] = retro_palette[*gfx];
      }
      video_cb(fceu_video_out, width, height, pitch);
   }
#endif
}

void retro_run(void)
{
   uint8_t *gfx;
   int32_t i, ssize = 0;
   bool updated = false;

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables(false);

   FCEUD_UpdateInput();
   FCEUI_Emulate(&gfx, &sound, &ssize, 0);

   for (i = 0; i < ssize; i++)
      sound[i] = (sound[i] << 16) | (sound[i] & 0xffff);

   audio_batch_cb((const int16_t*)sound, ssize);

   retro_run_blit(gfx);
}

size_t retro_serialize_size(void)
{
   if (serialize_size == 0)
   {
      /* Something arbitrarily big.*/
      uint8_t *buffer = (uint8_t*)malloc(1000000);
      memstream_set_buffer(buffer, 1000000);

      FCEUSS_Save_Mem();
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
   FCEUSS_Save_Mem();
   return true;
}

bool retro_unserialize(const void * data, size_t size)
{
   if (size != retro_serialize_size())
      return false;

   memstream_set_buffer((uint8_t*)data, size);
   FCEUSS_Load_Mem();
   return true;
}

static int checkGG(char c)
{
   static const char lets[16] = { 'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y', 'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N' };
   int x;

   for (x = 0; x < 16; x++)
      if (lets[x] == toupper(c))
         return 1;
   return 0;
}

static int GGisvalid(const char *code)
{
   size_t len = strlen(code);
   uint32 i;
   
   if (len != 6 && len != 8)
      return 0;

   for (i = 0; i < len; i++)
      if (!checkGG(code[i]))
         return 0;
   return 1;
}

void retro_cheat_reset(void)
{
   FCEU_ResetCheats();
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   char name[256];
   char temp[256];
   char *codepart;
   uint16 a;
   uint8  v;
   int    c;
   int    type = 1;

   if (code == NULL)
      return;

   sprintf(name, "N/A");
   strcpy(temp, code);
   codepart = strtok(temp, "+,;._ ");

   while (codepart)
   {
      if ((strlen(codepart) == 7) && (codepart[4]==':'))
      {
         /* raw code in xxxx:xx format */
         log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (Raw)\n", codepart);
         codepart[4] = '\0';
         a = strtoul(codepart, NULL, 16);
         v = strtoul(codepart + 5, NULL, 16);
         c = -1;
         /* Zero-page addressing modes don't go through the normal read/write handlers in FCEU, so
          * we must do the old hacky method of RAM cheats. */
         if (a < 0x0100) type = 0;
         FCEUI_AddCheat(name, a, v, c, type);
      }
      else if ((strlen(codepart) == 10) && (codepart[4] == '?') && (codepart[7] == ':'))
      {
         /* raw code in xxxx?xx:xx */
         log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (Raw)\n", codepart);
         codepart[4] = '\0';
         codepart[7] = '\0';
         a = strtoul(codepart, NULL, 16);
         v = strtoul(codepart + 8, NULL, 16);
         c = strtoul(codepart + 5, NULL, 16);
         /* Zero-page addressing modes don't go through the normal read/write handlers in FCEU, so
          * we must do the old hacky method of RAM cheats. */
         if (a < 0x0100) type = 0;
         FCEUI_AddCheat(name, a, v, c, type);
      }
      else if (GGisvalid(codepart) && FCEUI_DecodeGG(codepart, &a, &v, &c))
      {
         FCEUI_AddCheat(name, a, v, c, type);
         log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (GG)\n", codepart);
      }
      else if (FCEUI_DecodePAR(codepart, &a, &v, &c, &type))
      {
         FCEUI_AddCheat(name, a, v, c, type);
         log_cb.log(RETRO_LOG_DEBUG, "Cheat code added: '%s' (PAR)\n", codepart);
      }
      else
         log_cb.log(RETRO_LOG_DEBUG, "Invalid or unknown code: '%s'\n", codepart);
      codepart = strtok(NULL,"+,;._ ");
   }
}

typedef struct cartridge_db
{
   char title[256];
   uint32_t crc;
} cartridge_db_t;

static const struct cartridge_db fourscore_db_list[] =
{
   {
      "Bomberman II (USA)",
      0x1ebb5b42
   },
#if 0
   {
      "Championship Bowling (USA)",
      0xeac38105
   },
#endif
   {
      "Chris Evert & Ivan Lendl in Top Players' Tennis (USA)",
      0xf99e37eb
   },
#if 0
   {
      "Crash 'n' the Boys - Street Challenge (USA)",
      0xc7f0c457
   },
#endif
   {
      "Four Players' Tennis (Europe)",
      0x48b8ee58
   },
   {
      "Danny Sullivan's Indy Heat (Europe)",
      0x27ca0679,
   },
   {
      "Gauntlet II (Europe)",
      0x79f688bc
   },
   {
      "Gauntlet II (USA)",
      0x1b71ccdb
   },
   {
      "Greg Norman's Golf Power (USA)",
      0x1352f1b9
   },
   {
      "Harlem Globetrotters (USA)",
      0x2e6ee98d
   },
   {
      "Ivan 'Ironman' Stewart's Super Off Road (Europe)",
      0x05104517
   },
   {
      "Ivan 'Ironman' Stewart's Super Off Road (USA)",
      0x4b041b6b
   },
   {
      "Kings of the Beach - Professional Beach Volleyball (USA)",
      0xf54b34bd
   },
   {
      "Magic Johnson's Fast Break (USA)",
      0xc6c2edb5
   },
   {
      "M.U.L.E. (USA)",
      0x0939852f
   },
   {
      "Micro Mages",
      0x4e6b9078
   },
   {
      "Monster Truck Rally (USA)",
      0x2f698c4d
   },
   {
      "NES Play Action Football (USA)",
      0xb9b4d9e0
   },
   {
      "Nightmare on Elm Street, A (USA)",
      0xda2cb59a
   },
   {
      "Nintendo World Cup (Europe)",
      0x8da6667d
   },
   {
      "Nintendo World Cup (Europe) (Rev A)",
      0x7c16f819
   },
   {
      "Nintendo World Cup (Europe) (Rev B)",
      0x7f08d0d9
   },
   {
      "Nintendo World Cup (USA)",
      0xa22657fa
   },
   {
      "R.C. Pro-Am II (Europe)",
      0x308da987
   },
   {
      "R.C. Pro-Am II (USA)",
      0x9edd2159
   },
   {
      "Rackets & Rivals (Europe)",
      0x8fa6e92c
   },
   {
      "Roundball - 2-on-2 Challenge (Europe)",
      0xad0394f0
   },
   {
      "Roundball - 2-on-2 Challenge (USA)",
      0x6e4dcfd2
   },
   {
      "Spot - The Video Game (Japan)",
      0x0abdd5ca
   },
   {
      "Spot - The Video Game (USA)",
      0xcfae9dfa
   },
   {
      "Smash T.V. (Europe)",
      0x0b8f8128
   },
   {
      "Smash T.V. (USA)",
      0x6ee94d32
   },
   {
      "Super Jeopardy! (USA)",
      0xcf4487a2
   },
   {
      "Super Spike V'Ball (Europe)",
      0xc05a63b2
   },
   {
      "Super Spike V'Ball (USA)",
      0xe840fd21
   },
   {
      "Super Spike V'Ball + Nintendo World Cup (USA)",
      0x407d6ffd
   },
   {
      "Swords and Serpents (Europe)",
      0xd153caf6
   },
   {
      "Swords and Serpents (France)",
      0x46135141
   },
   {
      "Swords and Serpents (USA)",
      0x3417ec46
   },
   {
      "Battle City (Japan) (4 Players Hack) http://www.romhacking.net/hacks/2142/",
      0x69977c9e
   },
   {
      "Bomberman 3 (Homebrew) http://tempect.de/senil/games.html",
      0x2da5ece0
   },
   {
      "K.Y.F.F. (Homebrew) http://slydogstudios.org/index.php/k-y-f-f/",
      0x90d2e9f0
   },
   {
      "Super PakPak (Homebrew) http://wiki.nesdev.com/w/index.php/Super_PakPak",
      0x1394ded0
   },
   {
      "Super Mario Bros. + Tetris + Nintendo World Cup (Europe)",
      0x73298c87
   },
   {
      "Super Mario Bros. + Tetris + Nintendo World Cup (Europe) (Rev A)",
      0xf46ef39a
   }
};

static const struct cartridge_db famicom_4p_db_list[] =
{
   {
      "Bakutoushi Patton-Kun (Japan) (FDS)",
      0xc39b3bb2
   },
   {
      "Bomber Man II (Japan)",
      0x0c401790
   },
   {
      "Championship Bowling (Japan)",
      0x9992f445
   },
   {
      "Downtown - Nekketsu Koushinkyoku - Soreyuke Daiundoukai (Japan)",
      0x3e470fe0
   },
   {
      "Ike Ike! Nekketsu Hockey-bu - Subette Koronde Dairantou (Japan)",
      0x4f032933
   },
   {
      "Kunio-kun no Nekketsu Soccer League (Japan)",
      0x4b5177e9
   },
   {
      "Moero TwinBee - Cinnamon Hakase o Sukue! (Japan)",
      0x9f03b11f
   },
   {
      "Moero TwinBee - Cinnamon Hakase wo Sukue! (Japan) (FDS)",
      0x13205221
   },
   {
      "Nekketsu Kakutou Densetsu (Japan)",
      0x37e24797
   },
   {
      "Nekketsu Koukou Dodgeball-bu (Japan)",
      0x62c67984
   },
   {
      "Nekketsu! Street Basket - Ganbare Dunk Heroes (Japan)",
      0x88062d9a
   },
   {
      "Super Dodge Ball (USA) (3-4p with Game Genie code GEUOLZZA)",
      0x689971f9
   },
   {
      "Super Dodge Ball (USA) (patched) http://www.romhacking.net/hacks/71/",
      0x4ff17864
   },
   {
      "U.S. Championship V'Ball (Japan)",
      0x213cb3fb
   },
   {
      "U.S. Championship V'Ball (Japan) (Beta)",
      0xd7077d96
   },
   {
      "Wit's (Japan)",
      0xb1b16b8a
   }
};

#ifdef _WIN32
static char slash = '\\';
#else
static char slash = '/';
#endif

bool retro_load_game(const struct retro_game_info *game)
{
   unsigned i, j;
   char* dir=NULL;
   char* sav_dir=NULL;
   size_t fourscore_len = sizeof(fourscore_db_list)   / sizeof(fourscore_db_list[0]);
   size_t famicom_4p_len = sizeof(famicom_4p_db_list) / sizeof(famicom_4p_db_list[0]);
   enum retro_pixel_format rgb565;

   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,     "(VSSystem) Insert Coin" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "(FDS) Disk Side Change" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "(FDS) Insert/Eject Disk" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "D-Pad Left" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "D-Pad Up" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "D-Pad Down" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "D-Pad Right" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT,   "Select" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,    "Start" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Turbo A" },
      { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Turbo B" },

      { 0 },
   };
   size_t desc_base = 64;
   struct retro_memory_descriptor descs[64 + 4];
   struct retro_memory_map        mmaps;

   if (!game)
      return false;

#ifdef FRONTEND_SUPPORTS_RGB565
   rgb565 = RETRO_PIXEL_FORMAT_RGB565;
   if(environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &rgb565))
      log_cb.log(RETRO_LOG_INFO, "Frontend supports RGB565 - will use that instead of XRGB1555.\n");
#endif

   /* initialize some of the default variables */
#ifdef GEKKO
   sndsamplerate = 32000;
#else
   sndsamplerate = 48000;
#endif
   sndquality = 0;
   sndvolume = 150;
   swapDuty = 0;
   dendy = 0;

   /* Wii: initialize this or else last variable is passed through
    * when loading another rom causing save state size change. */
   serialize_size = 0;

   PowerNES();
   check_system_specs();
#if defined(_3DS)
   fceu_video_out = (uint16_t*)linearMemAlign(256 * 240 * sizeof(uint16_t), 128);
#elif !defined(PSP)
#ifdef HAVE_NTSC_FILTER
#define FB_WIDTH NES_NTSC_WIDTH
#define FB_HEIGHT NES_HEIGHT
#else /* !HAVE_NTSC_FILTER */
#define FB_WIDTH NES_WIDTH
#define FB_HEIGHT NES_HEIGHT
#endif
   fceu_video_out = (uint16_t*)malloc(FB_WIDTH * FB_HEIGHT * sizeof(uint16_t));
#endif

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      FCEUI_SetBaseDirectory(dir);
   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &sav_dir) && sav_dir)
      FCEUI_SetSaveDirectory(sav_dir);

   memset(base_palette, 0, sizeof(base_palette));

   FCEUI_Initialize();

   FCEUI_SetSoundVolume(sndvolume);
   FCEUI_Sound(sndsamplerate);

   GameInfo = (FCEUGI*)FCEUI_LoadGame(game->path, (uint8_t*)game->data, game->size);
   if (!GameInfo)
   {
      struct retro_message msg;
      char msg_local[256];

      sprintf(msg_local, "ROM loading failed...");
      msg.msg    = msg_local;
      msg.frames = 360;
      if (environ_cb)
         environ_cb(RETRO_ENVIRONMENT_SET_MESSAGE, (void*)&msg);
      return false;
   }

   for (i = 0; i < MAX_PORTS; i++) {
      FCEUI_SetInput(i, SI_GAMEPAD, &nes_input.JSReturn, 0);
      nes_input.type[i] = RETRO_DEVICE_JOYPAD;
   }

   external_palette_exist = ipalette;
   if (external_palette_exist)
      FCEU_printf(" Loading custom palette: %s%cnes.pal\n", dir, slash);

   /* Save region and dendy mode for region-auto detect */
   systemRegion = (dendy << 1) | (retro_get_region() & 1);

   current_palette = 0;

   ResetPalette();
   FCEUD_SoundToggle();
   set_variables();
   check_variables(true);
   PowerNES();

   FCEUI_DisableFourScore(1);

   for (i = 0; i < fourscore_len; i++)
   {
      if (fourscore_db_list[i].crc == iNESCart.CRC32)
      {
         FCEUI_DisableFourScore(0);
         nes_input.enable_4player = true;
         break;
      }
   }

   for (i = 0; i < famicom_4p_len; i++)
   {
      if (famicom_4p_db_list[i].crc == iNESCart.CRC32)
      {
         GameInfo->inputfc = SIFC_4PLAYER;
         FCEUI_SetInputFC(SIFC_4PLAYER, &nes_input.JSReturn, 0);
         nes_input.enable_4player = true;
         break;
      }
   }

   memset(descs, 0, sizeof(descs));
   i = 0;

   for (j = 0; j < desc_base; j++)
   {
       if (MMapPtrs[j] != NULL)
       {
         descs[i].ptr    = MMapPtrs[j];
         descs[i].start  = j * 1024;
         descs[i].len    = 1024;
         descs[i].select = 0;
         i++;
       }
   }
   /* This doesn't map in 2004--2007 but those aren't really
    * worthwhile to read from on a vblank anyway
    */
   descs[i].flags = 0;
   descs[i].ptr = PPU;
   descs[i].offset = 0;
   descs[i].start = 0x2000;
   descs[i].select = 0;
   descs[i].disconnect = 0;
   descs[i].len = 4;
   descs[i].addrspace="PPUREG";
   i++;
   /* In the future, it would be good to map pattern tables 1 and 2,
    * but these must be remapped often
    */
   /* descs[i] = (struct retro_memory_descriptor){0, ????, 0, 0x0000 | PPU_BIT, PPU_BIT, PPU_BIT, 0x1000, "PAT0"}; */
   /* i++; */
   /* descs[i] = (struct retro_memory_descriptor){0, ????, 0, 0x1000 | PPU_BIT, PPU_BIT, PPU_BIT, 0x1000, "PAT1"}; */
   /* i++; */
   /* Likewise it would be better to use "vnapage" for this but
    * RetroArch API is inconvenient for handles like that, so we'll
    * just blithely assume the client will handle mapping and that
    * we'll ignore those carts that have extra NTARAM.
    */
   descs[i].flags = 0;
   descs[i].ptr = NTARAM;
   descs[i].offset = 0;
   descs[i].start = PPU_BIT | 0x2000;
   descs[i].select = PPU_BIT;
   descs[i].disconnect = PPU_BIT;
   descs[i].len = 0x0800;
   descs[i].addrspace="NTARAM";
   i++;
   descs[i].flags = 0;
   descs[i].ptr = PALRAM;
   descs[i].offset = 0;
   descs[i].start = PPU_BIT | 0x3000;
   descs[i].select = PPU_BIT;
   descs[i].disconnect = PPU_BIT;
   descs[i].len = 0x020;
   descs[i].addrspace="PALRAM";
   i++;
   /* OAM doesn't really live anywhere in address space so I'll put it at 0x4000. */
   descs[i].flags = 0;
   descs[i].ptr = SPRAM;
   descs[i].offset = 0;
   descs[i].start = PPU_BIT | 0x4000;
   descs[i].select = PPU_BIT;
   descs[i].disconnect = PPU_BIT;
   descs[i].len = 0x100;
   descs[i].addrspace="OAM";
   i++;
   mmaps.descriptors = descs;
   mmaps.num_descriptors = i;
   environ_cb(RETRO_ENVIRONMENT_SET_MEMORY_MAPS, &mmaps);

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
#if defined(_3DS)
   if (fceu_video_out)
      linearFree(fceu_video_out);
#else
   if (fceu_video_out)
      free(fceu_video_out);
   fceu_video_out = NULL;
#endif
#if defined(RENDER_GSKIT_PS2)
   ps2 = NULL;
#endif
#ifdef HAVE_NTSC_FILTER
   NTSCFilter_Cleanup();
#endif
}

unsigned retro_get_region(void)
{
   return FSettings.PAL ? RETRO_REGION_PAL : RETRO_REGION_NTSC;
}

void *retro_get_memory_data(unsigned type)
{
   uint8_t* data;

   switch(type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (iNESCart.battery && iNESCart.SaveGame[0] && iNESCart.SaveGameLen[0])
            return iNESCart.SaveGame[0];
         else if (UNIFCart.battery && UNIFCart.SaveGame[0] && UNIFCart.SaveGameLen[0])
            return UNIFCart.SaveGame[0];
         else if (GameInfo->type == GIT_FDS)
            return FDSROM_ptr();
         else
            data = NULL;
         break;
      case RETRO_MEMORY_SYSTEM_RAM:
         data = RAM;
         break;
      default:
         data = NULL;
         break;
   }

   return data;
}

size_t retro_get_memory_size(unsigned type)
{
   unsigned size;

   switch(type)
   {
      case RETRO_MEMORY_SAVE_RAM:
         if (iNESCart.battery && iNESCart.SaveGame[0] && iNESCart.SaveGameLen[0])
            size = iNESCart.SaveGameLen[0];
         else if (UNIFCart.battery && UNIFCart.SaveGame[0] && UNIFCart.SaveGameLen[0])
            size = UNIFCart.SaveGameLen[0];
         else if (GameInfo->type == GIT_FDS)
            size = FDSROM_size();
         else
            size = 0;
         break;
      case RETRO_MEMORY_SYSTEM_RAM:
         size = 0x800;
         break;
      default:
         size = 0;
         break;
   }

   return size;
}
