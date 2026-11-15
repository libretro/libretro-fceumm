/* Palette conversion to the frontend's pixel format rounds to nearest.
 *
 * Writes a custom palette whose entry k is the gray 4k + 3, and a
 * generated NROM image that shows each palette entry in turn as the
 * backdrop, the next entry each frame. Every frame must show its entry's
 * gray narrowed with rounding (exact in XRGB8888).
 *
 * Linux (build the core with `make -f Makefile.libretro WANT_32BPP=0`
 * to test the RGB565 path):
 *   cc -std=c89 -pedantic -Wall -O1 \
 *      -Isrc/drivers/libretro/libretro-common/include \
 *      tests/palette_round.c -o palette_round -ldl
 *   ./palette_round ./fceumm_libretro.so
 * MSYS2 MINGW64:
 *   cc -std=c89 -pedantic -Wall -O1 \
 *      -Isrc/drivers/libretro/libretro-common/include \
 *      tests/palette_round.c -o palette_round.exe
 *   ./palette_round.exe ./fceumm_libretro.dll */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "libretro.h"

#ifdef _WIN32
#include <windows.h>
#define LIB_OPEN(p)    ((void *)LoadLibraryA(p))
#define LIB_SYM(h, s)  ((void *)GetProcAddress((HMODULE)(h), (s)))
#else
#include <dlfcn.h>
#define LIB_OPEN(p)    dlopen((p), RTLD_NOW | RTLD_LOCAL)
#define LIB_SYM(h, s)  dlsym((h), (s))
#endif

#define ROM_NAME  "palette_round_test.nes"
#define PAL_NAME  "nes.pal"
#define FRAMES    200
#define SKIP      8   /* frames before the cycle starts */

static enum retro_pixel_format pixel_format = RETRO_PIXEL_FORMAT_0RGB1555;
static unsigned long shown[FRAMES];
static unsigned frame;

static unsigned gray(unsigned k)
{
   return 4 * k + 3;
}

/* v narrowed to 'bits' bits, rounded to nearest. */
static unsigned long narrow(unsigned v, unsigned bits)
{
   unsigned long max = (1UL << bits) - 1;
   return (v * max + 127) / 255;
}

static unsigned long pixel_of(unsigned v)
{
   switch (pixel_format)
   {
      case RETRO_PIXEL_FORMAT_XRGB8888:
         return ((unsigned long)v << 16) | ((unsigned long)v << 8) | v;
      case RETRO_PIXEL_FORMAT_RGB565:
         return (narrow(v, 5) << 11) | (narrow(v, 6) << 5) | narrow(v, 5);
      default:
         return (narrow(v, 5) << 10) | (narrow(v, 5) << 5) | narrow(v, 5);
   }
}

static void video_cb(const void *data, unsigned w, unsigned h, size_t pitch)
{
   const unsigned char *row;

   if (!data || frame >= FRAMES)
   {
      frame++;
      return;
   }
   row = (const unsigned char *)data + (h / 2) * pitch;
   if (pixel_format == RETRO_PIXEL_FORMAT_XRGB8888)
      shown[frame] = ((const uint32_t *)row)[w / 2] & 0xFFFFFFUL;
   else
      shown[frame] = ((const uint16_t *)row)[w / 2];
   frame++;
}

static void audio_cb(int16_t l, int16_t r) { (void)l; (void)r; }
static size_t audio_batch_cb(const int16_t *d, size_t f) { (void)d; return f; }
static void input_poll_cb(void) { }

static int16_t input_state_cb(unsigned port, unsigned dev, unsigned idx, unsigned id)
{
   (void)port; (void)dev; (void)idx; (void)id;
   return 0;
}

static void log_cb(enum retro_log_level level, const char *fmt, ...)
{
   (void)level; (void)fmt;
}

static bool env_cb(unsigned cmd, void *data)
{
   switch (cmd)
   {
      case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
         pixel_format = *(const enum retro_pixel_format *)data;
         return true;
      case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
      case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
         *(const char **)data = ".";
         return true;
      case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
         ((struct retro_log_callback *)data)->log = log_cb;
         return true;
      case RETRO_ENVIRONMENT_GET_VARIABLE:
      {
         struct retro_variable *var = (struct retro_variable *)data;
         if (!strcmp(var->key, "fceumm_palette"))
         {
            var->value = "custom";
            return true;
         }
         return false;
      }
      default:
         return false;
   }
}

static int write_file(const char *path, const unsigned char *data, size_t size)
{
   FILE *f = fopen(path, "wb");
   if (!f)
      return 0;
   if (fwrite(data, 1, size, f) != size)
   {
      fclose(f);
      return 0;
   }
   return fclose(f) == 0;
}

static int write_palette(void)
{
   unsigned char pal[64 * 3];
   unsigned k;

   for (k = 0; k < 64; k++)
      pal[k * 3] = pal[k * 3 + 1] = pal[k * 3 + 2] = (unsigned char)gray(k);
   return write_file(PAL_NAME, pal, sizeof(pal));
}

/* NROM-128: background on, NMI on, idle loop. Each NMI writes the next
 * palette entry number into the backdrop ($3F00) and resets the scroll.
 * CHR is zero, so the whole picture is the backdrop. */
static int write_rom(void)
{
   static const unsigned char reset[] = {
      0x78,                   /* sei           */
      0xD8,                   /* cld           */
      0xA2, 0xFF,             /* ldx #$FF      */
      0x9A,                   /* txs           */
      0xA9, 0x00,             /* lda #$00      */
      0x85, 0x00,             /* sta $00       */
      0xA9, 0x0A,             /* lda #$0A      */
      0x8D, 0x01, 0x20,       /* sta $2001     */
      0xA9, 0x80,             /* lda #$80      */
      0x8D, 0x00, 0x20,       /* sta $2000     */
      0x4C, 0x00, 0x00        /* jmp idle      */
   };
   static const unsigned char nmi[] = {
      0xA9, 0x3F,             /* lda #$3F      */
      0x8D, 0x06, 0x20,       /* sta $2006     */
      0xA9, 0x00,             /* lda #$00      */
      0x8D, 0x06, 0x20,       /* sta $2006     */
      0xA5, 0x00,             /* lda $00       */
      0x29, 0x3F,             /* and #$3F      */
      0x8D, 0x07, 0x20,       /* sta $2007     */
      0xE6, 0x00,             /* inc $00       */
      0xA9, 0x00,             /* lda #$00      */
      0x8D, 0x06, 0x20,       /* sta $2006     */
      0x8D, 0x06, 0x20,       /* sta $2006     */
      0x8D, 0x05, 0x20,       /* sta $2005     */
      0x8D, 0x05, 0x20,       /* sta $2005     */
      0x40                    /* rti           */
   };
   static unsigned char rom[16 + 0x4000 + 0x2000];
   unsigned idle   = 0xC000 + sizeof(reset) - 3;
   unsigned nmi_at = 0xC000 + sizeof(reset);

   memset(rom, 0, sizeof(rom));
   memcpy(rom, "NES\x1A", 4);
   rom[4] = 1;
   rom[5] = 1;
   memcpy(rom + 16, reset, sizeof(reset));
   rom[16 + sizeof(reset) - 2] = (unsigned char)(idle & 0xFF);
   rom[16 + sizeof(reset) - 1] = (unsigned char)(idle >> 8);
   memcpy(rom + 16 + sizeof(reset), nmi, sizeof(nmi));
   rom[16 + 0x3FFA] = (unsigned char)(nmi_at & 0xFF);
   rom[16 + 0x3FFB] = (unsigned char)(nmi_at >> 8);
   rom[16 + 0x3FFC] = 0x00;
   rom[16 + 0x3FFD] = 0xC0;
   rom[16 + 0x3FFE] = (unsigned char)(idle & 0xFF);
   rom[16 + 0x3FFF] = (unsigned char)(idle >> 8);
   return write_file(ROM_NAME, rom, sizeof(rom));
}

static int load_sym(void *lib, const char *name, void *dst)
{
   void *p = LIB_SYM(lib, name);
   if (!p)
   {
      fprintf(stderr, "missing %s\n", name);
      return 0;
   }
   memcpy(dst, &p, sizeof(p));
   return 1;
}

int main(int argc, char **argv)
{
   void (*set_environment)(retro_environment_t);
   void (*set_video_refresh)(retro_video_refresh_t);
   void (*set_audio_sample)(retro_audio_sample_t);
   void (*set_audio_sample_batch)(retro_audio_sample_batch_t);
   void (*set_input_poll)(retro_input_poll_t);
   void (*set_input_state)(retro_input_state_t);
   void (*init)(void);
   void (*deinit)(void);
   bool (*load_game)(const struct retro_game_info *);
   void (*unload_game)(void);
   void (*run)(void);
   struct retro_game_info gi;
   void *lib;
   unsigned f, phase, best = 0, best_phase = 0, bad = 0;
   int i, fail = 0;

   if (argc < 2)
   {
      fprintf(stderr, "usage: %s core\n", argv[0]);
      return 2;
   }
   lib = LIB_OPEN(argv[1]);
   if (!lib)
   {
      fprintf(stderr, "cannot load %s\n", argv[1]);
      return 1;
   }
   if (!load_sym(lib, "retro_set_environment", &set_environment)
         || !load_sym(lib, "retro_set_video_refresh", &set_video_refresh)
         || !load_sym(lib, "retro_set_audio_sample", &set_audio_sample)
         || !load_sym(lib, "retro_set_audio_sample_batch", &set_audio_sample_batch)
         || !load_sym(lib, "retro_set_input_poll", &set_input_poll)
         || !load_sym(lib, "retro_set_input_state", &set_input_state)
         || !load_sym(lib, "retro_init", &init)
         || !load_sym(lib, "retro_deinit", &deinit)
         || !load_sym(lib, "retro_load_game", &load_game)
         || !load_sym(lib, "retro_unload_game", &unload_game)
         || !load_sym(lib, "retro_run", &run))
      return 1;
   if (!write_palette() || !write_rom())
   {
      fprintf(stderr, "cannot write the test content\n");
      return 1;
   }

   set_environment(env_cb);
   init();
   set_video_refresh(video_cb);
   set_audio_sample(audio_cb);
   set_audio_sample_batch(audio_batch_cb);
   set_input_poll(input_poll_cb);
   set_input_state(input_state_cb);
   memset(&gi, 0, sizeof(gi));
   gi.path = ROM_NAME;
   if (!load_game(&gi))
   {
      fprintf(stderr, "load failed\n");
      fail = 1;
   }
   else
   {
      for (i = 0; i < FRAMES; i++)
         run();
      unload_game();
   }
   deinit();
   remove(ROM_NAME);
   remove(PAL_NAME);
   if (fail)
      return 1;

   /* The cycle's phase is the one most frames agree with; every frame
    * after the start must then show its own entry. */
   for (phase = 0; phase < 64; phase++)
   {
      unsigned n = 0;
      for (f = SKIP; f < FRAMES; f++)
         if (shown[f] == pixel_of(gray((f + phase) & 63)))
            n++;
      if (n > best)
      {
         best       = n;
         best_phase = phase;
      }
   }

   printf("pixel format: %s\n",
         pixel_format == RETRO_PIXEL_FORMAT_XRGB8888 ? "XRGB8888" :
         pixel_format == RETRO_PIXEL_FORMAT_RGB565 ? "RGB565" : "0RGB1555");
   if (best != FRAMES - SKIP)
   {
      for (f = SKIP; f < FRAMES; f++)
      {
         unsigned k = (f + best_phase) & 63;
         if (shown[f] != pixel_of(gray(k)))
         {
            printf("FAIL: gray %3u shown as %06lx, rounds to %06lx\n",
                  gray(k), shown[f], pixel_of(gray(k)));
            if (++bad >= 8)
               break;
         }
      }
      printf("FAIL: %u of %u frames show another value\n",
            FRAMES - SKIP - best, FRAMES - SKIP);
      fail = 1;
   }
   printf("%s\n", fail ? "FAIL" : "PASS");
   return fail;
}
