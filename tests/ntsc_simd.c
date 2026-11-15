/* The NTSC filter's vector blit (SSE2 or NEON) against its scalar one.
 *
 * Builds the filter tables for every preset, with and without merged
 * fields, then blits random pixels with per-pixel random emphasis at
 * several input widths and every burst phase through both blitters and
 * compares the output byte for byte, including a guard area after each
 * row that neither may write. Without a vector arm for the target it
 * reports that and passes.
 *
 * Build for both output depths (32-bit when FRONTEND_SUPPORTS_RGB888):
 *   cc -std=c89 -pedantic -O2 -Isrc/ntsc -DFRONTEND_SUPPORTS_RGB888 \
 *      tests/ntsc_simd.c -o ntsc_simd -lm && ./ntsc_simd
 *   cc -std=c89 -pedantic -O2 -Isrc/ntsc tests/ntsc_simd.c -o ntsc_simd -lm && ./ntsc_simd
 * MSYS2 MINGW64: the same commands, running ./ntsc_simd.exe. */
#define NES_NTSC_TEST 1
#include "../src/ntsc/nes_ntsc.c"

#include <stdio.h>
#include <stdlib.h>

#define IN_PITCH   256
#define ROWS       24
#define OUT_PITCH  (NES_NTSC_OUT_WIDTH(256) + 16)
#define GUARD      0xA5

static unsigned long rng = 12345;

static unsigned next_rand(void)
{
   rng = rng * 1103515245UL + 12345UL;
   return (unsigned)((rng >> 16) & 0x7FFF);
}

int main(void)
{
#ifndef NES_NTSC_SIMD
   printf("no vector arm for this target: scalar only\nPASS\n");
   return 0;
#else
   static const int widths[] = { 256, 255, 250, 240, 241, 4 };
   nes_ntsc_setup_t const *presets[4];
   static unsigned char in[IN_PITCH * ROWS], inD[IN_PITCH * ROWS];
   static nes_ntsc_out_t out_simd[OUT_PITCH * ROWS], out_ref[OUT_PITCH * ROWS];
   nes_ntsc_t *ntsc = (nes_ntsc_t *)malloc(sizeof(nes_ntsc_t));
   int p, merge, w, burst, i, cases = 0, fail = 0;

   presets[0] = &nes_ntsc_composite;
   presets[1] = &nes_ntsc_svideo;
   presets[2] = &nes_ntsc_rgb;
   presets[3] = &nes_ntsc_monochrome;
   if (!ntsc)
      return 1;

   for (p = 0; p < 4; p++)
   for (merge = 0; merge < 2; merge++)
   {
      nes_ntsc_setup_t setup = *presets[p];
      setup.merge_fields = merge;
      nes_ntsc_init(ntsc, &setup);

      for (w = 0; w < (int)(sizeof(widths) / sizeof(widths[0])); w++)
      for (burst = 0; burst < nes_ntsc_burst_count; burst++)
      {
         for (i = 0; i < IN_PITCH * ROWS; i++)
         {
            in[i]  = (unsigned char)next_rand();
            inD[i] = (unsigned char)next_rand();
         }
         memset(out_simd, GUARD, sizeof(out_simd));
         memset(out_ref, GUARD, sizeof(out_ref));

         nes_ntsc_blit(ntsc, in, inD, IN_PITCH, burst, widths[w], ROWS,
               out_simd, OUT_PITCH * sizeof(nes_ntsc_out_t));
         nes_ntsc_blit_scalar(ntsc, in, inD, IN_PITCH, burst, widths[w], ROWS,
               out_ref, OUT_PITCH * sizeof(nes_ntsc_out_t));
         cases++;
         if (memcmp(out_simd, out_ref, sizeof(out_ref)))
         {
            for (i = 0; i < OUT_PITCH * ROWS; i++)
               if (out_simd[i] != out_ref[i])
                  break;
            printf("FAIL: preset %d merge %d width %d burst %d: pixel %d of row %d"
                  " is %lx, scalar %lx\n", p, merge, widths[w], burst,
                  i % OUT_PITCH, i / OUT_PITCH,
                  (unsigned long)out_simd[i], (unsigned long)out_ref[i]);
            fail = 1;
         }
      }
   }

   free(ntsc);
   printf("%s arm, %d-bit output: %d cases %s\n",
#ifdef NES_NTSC_SIMD_SSE2
         "SSE2",
#else
         "NEON",
#endif
         NES_NTSC_OUT_DEPTH, cases, fail ? "differ" : "identical to scalar");
   printf("%s\n", fail ? "FAIL" : "PASS");
   return fail;
#endif
}
