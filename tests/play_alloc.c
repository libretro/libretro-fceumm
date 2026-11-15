/* Allocation-free play and savestate round trip, with a generated
 * NROM image (no external ROMs).
 *
 * Loads the core, runs a short warm-up, then counts every heap call the
 * process makes while it calls retro_serialize_size, and for each frame
 * retro_serialize, retro_unserialize and retro_run, the way run-ahead
 * does. Any allocation fails the test. It then saves a state, runs
 * frames, loads the state and runs them again: both runs must produce
 * the same pictures and audio.
 *
 * Core options are given as key=value arguments after the core path.
 *
 * Linux:
 *   cc -std=c89 -pedantic -Wall -O1 -rdynamic \
 *      -Isrc/drivers/libretro/libretro-common/include \
 *      tests/play_alloc.c -o play_alloc -ldl
 *   ./play_alloc ./fceumm_libretro.so
 *   ./play_alloc ./fceumm_libretro.so fceumm_sndstereodelay=15ms
 * MSYS2 MINGW64:
 *   cc -std=c89 -pedantic -Wall -O1 \
 *      -Isrc/drivers/libretro/libretro-common/include \
 *      tests/play_alloc.c -o play_alloc.exe
 *   ./play_alloc.exe ./fceumm_libretro.dll
 *
 * Counting needs glibc, where the executable's malloc interposes the
 * core's; elsewhere the count is reported as skipped and only the
 * round trip is checked. */
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

#define ROM_NAME     "play_alloc_test.nes"
#define WARM_FRAMES  60
#define PLAY_FRAMES  240
#define TRIP_FRAMES  120
#define MAX_OPTIONS  16

/* ---- heap call counting (glibc) ---- */

static unsigned long heap_calls;
static int heap_counting;

#if defined(__GLIBC__)
#define HAVE_HEAP_COUNT 1
extern void *__libc_malloc(size_t);
extern void *__libc_calloc(size_t, size_t);
extern void *__libc_realloc(void *, size_t);
extern void  __libc_free(void *);

void *malloc(size_t n)
{
   if (heap_counting)
      heap_calls++;
   return __libc_malloc(n);
}

void *calloc(size_t n, size_t s)
{
   if (heap_counting)
      heap_calls++;
   return __libc_calloc(n, s);
}

void *realloc(void *p, size_t n)
{
   if (heap_counting)
      heap_calls++;
   return __libc_realloc(p, n);
}

void free(void *p)
{
   if (heap_counting && p)
      heap_calls++;
   __libc_free(p);
}
#else
#define HAVE_HEAP_COUNT 0
#endif

/* ---- frontend ---- */

static struct retro_variable options[MAX_OPTIONS];
static unsigned num_options;
static unsigned long frame_hash;
static unsigned pixel_bytes = 2;

static void hash_bytes(const unsigned char *p, size_t n)
{
   size_t i;
   for (i = 0; i < n; i++)
      frame_hash = ((frame_hash ^ p[i]) * 16777619UL) & 0xFFFFFFFFUL;
}

static void video_cb(const void *data, unsigned w, unsigned h, size_t pitch)
{
   unsigned y;
   if (!data)
      return;
   for (y = 0; y < h; y++)
      hash_bytes((const unsigned char *)data + y * pitch, (size_t)w * pixel_bytes);
}

static void audio_cb(int16_t l, int16_t r)
{
   hash_bytes((const unsigned char *)&l, sizeof(l));
   hash_bytes((const unsigned char *)&r, sizeof(r));
}

static size_t audio_batch_cb(const int16_t *d, size_t frames)
{
   hash_bytes((const unsigned char *)d, frames * 2 * sizeof(*d));
   return frames;
}

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
   unsigned i;

   switch (cmd)
   {
      case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
         pixel_bytes = (*(const enum retro_pixel_format *)data
               == RETRO_PIXEL_FORMAT_XRGB8888) ? 4 : 2;
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
         for (i = 0; i < num_options; i++)
            if (!strcmp(var->key, options[i].key))
            {
               var->value = options[i].value;
               return true;
            }
         return false;
      }
      default:
         return false;
   }
}

/* ---- generated content ---- */

/* NROM-128: rendering on, a square wave on pulse 1, then an idle loop.
 * Everything else in PRG and CHR is zero. */
static int write_rom(const char *path)
{
   static const unsigned char code[] = {
      0x78,                   /* sei            */
      0xD8,                   /* cld            */
      0xA9, 0x01,             /* lda #$01       */
      0x8D, 0x15, 0x40,       /* sta $4015      */
      0xA9, 0xBF,             /* lda #$BF       */
      0x8D, 0x00, 0x40,       /* sta $4000      */
      0xA9, 0xFD,             /* lda #$FD       */
      0x8D, 0x02, 0x40,       /* sta $4002      */
      0xA9, 0x00,             /* lda #$00       */
      0x8D, 0x03, 0x40,       /* sta $4003      */
      0xA9, 0x1E,             /* lda #$1E       */
      0x8D, 0x01, 0x20,       /* sta $2001      */
      0x4C, 0x00, 0x00        /* jmp idle (self)*/
   };
   static unsigned char rom[16 + 0x4000 + 0x2000];
   unsigned idle = 0xC000 + sizeof(code) - 3;
   FILE *f;

   memset(rom, 0, sizeof(rom));
   memcpy(rom, "NES\x1A", 4);
   rom[4] = 1; /* 16 KiB PRG */
   rom[5] = 1; /* 8 KiB CHR */
   memcpy(rom + 16, code, sizeof(code));
   rom[16 + sizeof(code) - 2] = (unsigned char)(idle & 0xFF);
   rom[16 + sizeof(code) - 1] = (unsigned char)(idle >> 8);
   /* NMI, RESET, IRQ vectors */
   rom[16 + 0x3FFA] = (unsigned char)(idle & 0xFF);
   rom[16 + 0x3FFB] = (unsigned char)(idle >> 8);
   rom[16 + 0x3FFC] = 0x00;
   rom[16 + 0x3FFD] = 0xC0;
   rom[16 + 0x3FFE] = (unsigned char)(idle & 0xFF);
   rom[16 + 0x3FFF] = (unsigned char)(idle >> 8);

   f = fopen(path, "wb");
   if (!f)
      return 0;
   if (fwrite(rom, 1, sizeof(rom), f) != sizeof(rom))
   {
      fclose(f);
      return 0;
   }
   return fclose(f) == 0;
}

/* ---- test ---- */

typedef struct
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
   size_t (*serialize_size)(void);
   bool (*serialize)(void *, size_t);
   bool (*unserialize)(const void *, size_t);
} core_t;

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
   core_t c;
   void *lib;
   struct retro_game_info gi;
   unsigned char *state;
   size_t size;
   unsigned long hash_a, hash_b, calls;
   int i, fail = 0;

   if (argc < 2)
   {
      fprintf(stderr, "usage: %s core [key=value ...]\n", argv[0]);
      return 2;
   }
   for (i = 2; i < argc && num_options < MAX_OPTIONS; i++)
   {
      char *eq = strchr(argv[i], '=');
      if (!eq)
         continue;
      *eq = '\0';
      options[num_options].key   = argv[i];
      options[num_options].value = eq + 1;
      num_options++;
   }

   lib = LIB_OPEN(argv[1]);
   if (!lib)
   {
      fprintf(stderr, "cannot load %s\n", argv[1]);
      return 1;
   }
   if (!load_sym(lib, "retro_set_environment", &c.set_environment)
         || !load_sym(lib, "retro_set_video_refresh", &c.set_video_refresh)
         || !load_sym(lib, "retro_set_audio_sample", &c.set_audio_sample)
         || !load_sym(lib, "retro_set_audio_sample_batch", &c.set_audio_sample_batch)
         || !load_sym(lib, "retro_set_input_poll", &c.set_input_poll)
         || !load_sym(lib, "retro_set_input_state", &c.set_input_state)
         || !load_sym(lib, "retro_init", &c.init)
         || !load_sym(lib, "retro_deinit", &c.deinit)
         || !load_sym(lib, "retro_load_game", &c.load_game)
         || !load_sym(lib, "retro_unload_game", &c.unload_game)
         || !load_sym(lib, "retro_run", &c.run)
         || !load_sym(lib, "retro_serialize_size", &c.serialize_size)
         || !load_sym(lib, "retro_serialize", &c.serialize)
         || !load_sym(lib, "retro_unserialize", &c.unserialize))
      return 1;

   if (!write_rom(ROM_NAME))
   {
      fprintf(stderr, "cannot write %s\n", ROM_NAME);
      return 1;
   }

   c.set_environment(env_cb);
   c.init();
   c.set_video_refresh(video_cb);
   c.set_audio_sample(audio_cb);
   c.set_audio_sample_batch(audio_batch_cb);
   c.set_input_poll(input_poll_cb);
   c.set_input_state(input_state_cb);
   memset(&gi, 0, sizeof(gi));
   gi.path = ROM_NAME;
   if (!c.load_game(&gi))
   {
      fprintf(stderr, "load failed\n");
      remove(ROM_NAME);
      return 1;
   }

   for (i = 0; i < WARM_FRAMES; i++)
      c.run();

   /* The state buffer is the frontend's; it is made before play. */
   heap_counting = 1;
   size = c.serialize_size();
   heap_counting = 0;
   state = (unsigned char *)malloc(size);
   if (!size || !state)
   {
      fprintf(stderr, "FAIL: serialize size %lu\n", (unsigned long)size);
      return 1;
   }

   heap_counting = 1;
   for (i = 0; i < PLAY_FRAMES; i++)
   {
      if (!c.serialize(state, size) || !c.unserialize(state, size))
      {
         heap_counting = 0;
         fprintf(stderr, "FAIL: savestate round trip at frame %d\n", i);
         fail = 1;
         break;
      }
      c.run();
   }
   heap_counting = 0;
   calls = heap_calls;

   if (!HAVE_HEAP_COUNT)
      printf("heap calls during play: not counted on this platform\n");
   else if (calls)
   {
      printf("FAIL: %lu heap calls during play\n", calls);
      fail = 1;
   }
   else
      printf("heap calls during play: 0\n");

   /* Same frames after loading the state they were run from. */
   if (!c.serialize(state, size))
      fail = 1;
   frame_hash = 2166136261UL;
   for (i = 0; i < TRIP_FRAMES; i++)
      c.run();
   hash_a = frame_hash;
   if (!c.unserialize(state, size))
      fail = 1;
   frame_hash = 2166136261UL;
   for (i = 0; i < TRIP_FRAMES; i++)
      c.run();
   hash_b = frame_hash;
   if (hash_a != hash_b)
   {
      printf("FAIL: frames after loading differ (%08lx vs %08lx)\n", hash_a, hash_b);
      fail = 1;
   }
   else
      printf("savestate round trip: same %d frames\n", TRIP_FRAMES);

   c.unload_game();
   c.deinit();
   free(state);
   remove(ROM_NAME);
   printf("%s\n", fail ? "FAIL" : "PASS");
   return fail;
}
