/* Lazy image decode for HD pack tile sheets and backgrounds.
 *
 * All decoding goes through libretro-common: rpng for .png, rdds for
 * .dds (uncompressed + BC-compressed), rwebp for .webp (VP8L and VP8).
 * Every decoder is asked for ARGB output (supports_rgba = false), which
 * is the same channel order Mesen uses internally, then alpha is
 * premultiplied exactly like Mesen's HdPackBitmapInfo::PremultiplyAlpha
 * so the blend paths in hdpack_render.c can be ported one to one. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <formats/rpng.h>
#include <formats/rdds.h>
#include <formats/rwebp.h>
#include <formats/image.h>

#include "hdpack.h"
#include "hdpack_data.h"

static int hd_str_iends_with(const char *s, const char *suffix)
{
   size_t sl = strlen(s);
   size_t xl = strlen(suffix);
   size_t i;
   if (xl > sl)
      return 0;
   s += sl - xl;
   for (i = 0; i < xl; i++)
   {
      char a = s[i];
      char b = suffix[i];
      if (a >= 'A' && a <= 'Z')
         a = (char)(a - 'A' + 'a');
      if (b >= 'A' && b <= 'Z')
         b = (char)(b - 'A' + 'a');
      if (a != b)
         return 0;
   }
   return 1;
}

static void hd_premultiply_alpha(uint32_t *pixels, size_t count)
{
   size_t i;
   for (i = 0; i < count; i++)
   {
      if (pixels[i] < 0xFF000000u)
      {
         uint8_t *output = (uint8_t*)&pixels[i];
         unsigned alpha = (unsigned)output[3] + 1;
         output[0] = (uint8_t)((alpha * output[0]) >> 8);
         output[1] = (uint8_t)((alpha * output[1]) >> 8);
         output[2] = (uint8_t)((alpha * output[2]) >> 8);
      }
   }
}

static int hd_decode_png(const uint8_t *data, size_t size,
      uint32_t **pixels, unsigned *width, unsigned *height)
{
   rpng_t *rpng = rpng_alloc();
   int ret = 0;
   int process_ret;

   if (!rpng)
      return 0;
   if (!rpng_set_buf_ptr(rpng, (void*)data, size))
      goto end;
   if (!rpng_start(rpng))
      goto end;
   while (rpng_iterate_image(rpng)) { }
   if (!rpng_is_valid(rpng))
      goto end;
   do
   {
      process_ret = rpng_process_image(rpng, (void**)pixels, size,
            width, height, false);
   } while (process_ret == IMAGE_PROCESS_NEXT);
   ret = (process_ret == IMAGE_PROCESS_END && *pixels);
end:
   rpng_free(rpng);
   return ret;
}

static int hd_decode_dds(const uint8_t *data, size_t size,
      uint32_t **pixels, unsigned *width, unsigned *height)
{
   rdds_t *rdds = rdds_alloc();
   int ret = 0;
   if (!rdds)
      return 0;
   if (rdds_set_buf_ptr(rdds, (void*)data))
   {
      int process_ret;
      do
      {
         process_ret = rdds_process_image(rdds, (void**)pixels, size,
               width, height, false);
      } while (process_ret == IMAGE_PROCESS_NEXT);
      ret = (process_ret == IMAGE_PROCESS_END && *pixels);
   }
   rdds_free(rdds);
   return ret;
}

static int hd_decode_webp(const uint8_t *data, size_t size,
      uint32_t **pixels, unsigned *width, unsigned *height)
{
   rwebp_t *rwebp = rwebp_alloc();
   int ret = 0;
   if (!rwebp)
      return 0;
   if (rwebp_set_buf_ptr(rwebp, (void*)data, size))
   {
      int process_ret;
      do
      {
         process_ret = rwebp_process_image(rwebp, (void**)pixels, size,
               width, height, false);
      } while (process_ret == IMAGE_PROCESS_NEXT);
      ret = (process_ret == IMAGE_PROCESS_END && *pixels);
   }
   rwebp_free(rwebp);
   return ret;
}

int hd_bitmap_init(hd_bitmap *bmp)
{
   uint32_t *pixels = NULL;
   unsigned width = 0;
   unsigned height = 0;
   int ok = 0;

   if (bmp->init_done)
      return !bmp->failed;
   bmp->init_done = 1;

   if (!bmp->file_data || !bmp->file_size)
   {
      bmp->failed = 1;
      return 0;
   }

   if (hd_str_iends_with(bmp->name, ".png"))
      ok = hd_decode_png(bmp->file_data, bmp->file_size, &pixels,
            &width, &height);
   else if (hd_str_iends_with(bmp->name, ".dds"))
      ok = hd_decode_dds(bmp->file_data, bmp->file_size, &pixels,
            &width, &height);
   else if (hd_str_iends_with(bmp->name, ".webp"))
      ok = hd_decode_webp(bmp->file_data, bmp->file_size, &pixels,
            &width, &height);
   else
   {
      /* Extension not recognized: sniff the magic so packs converted
       * with unusual file names still work. */
      static const uint8_t png_magic[4] = { 0x89, 'P', 'N', 'G' };
      if (bmp->file_size >= 4 &&
            memcmp(bmp->file_data, png_magic, 4) == 0)
         ok = hd_decode_png(bmp->file_data, bmp->file_size, &pixels,
               &width, &height);
      else if (bmp->file_size >= 4 &&
            memcmp(bmp->file_data, "DDS ", 4) == 0)
         ok = hd_decode_dds(bmp->file_data, bmp->file_size, &pixels,
               &width, &height);
      else if (bmp->file_size >= 12 &&
            memcmp(bmp->file_data, "RIFF", 4) == 0 &&
            memcmp(bmp->file_data + 8, "WEBP", 4) == 0)
         ok = hd_decode_webp(bmp->file_data, bmp->file_size, &pixels,
               &width, &height);
   }

   /* Raw file data is no longer needed either way. */
   free(bmp->file_data);
   bmp->file_data = NULL;
   bmp->file_size = 0;

   if (!ok || !pixels || !width || !height)
   {
      hd_log("Image file %s is invalid.", bmp->name ? bmp->name : "?");
      if (pixels)
         free(pixels);
      bmp->failed = 1;
      return 0;
   }

   hd_premultiply_alpha(pixels, (size_t)width * height);
   bmp->pixels = pixels;
   bmp->width = width;
   bmp->height = height;

   /* Precompute the per-row non-transparent extent so the background
    * span blit can skip transparent runs. A pixel is transparent iff
    * its premultiplied ARGB is 0 (alpha 0). Failure to allocate is not
    * fatal: the blit falls back to scanning the whole row. */
   bmp->row_min = (uint32_t*)malloc((size_t)height * sizeof(uint32_t));
   bmp->row_max = (uint32_t*)malloc((size_t)height * sizeof(uint32_t));
   if (bmp->row_min && bmp->row_max)
   {
      uint32_t r, c;
      const uint32_t *p = pixels;
      for (r = 0; r < height; r++)
      {
         uint32_t mn = width;
         uint32_t mx = 0;
         for (c = 0; c < width; c++)
         {
            if (p[c])
            {
               if (c < mn)
                  mn = c;
               mx = c;
            }
         }
         bmp->row_min[r] = mn;
         bmp->row_max[r] = mx;
         p += width;
      }
   }
   else
   {
      if (bmp->row_min)
         free(bmp->row_min);
      if (bmp->row_max)
         free(bmp->row_max);
      bmp->row_min = NULL;
      bmp->row_max = NULL;
   }
   return 1;
}

void hd_bitmap_free(hd_bitmap *bmp)
{
   if (bmp->name)
      free(bmp->name);
   if (bmp->file_data)
      free(bmp->file_data);
   if (bmp->pixels)
      free(bmp->pixels);
   if (bmp->row_min)
      free(bmp->row_min);
   if (bmp->row_max)
      free(bmp->row_max);
   memset(bmp, 0, sizeof(*bmp));
}
