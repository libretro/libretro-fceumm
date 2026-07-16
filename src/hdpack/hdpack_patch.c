/* HD pack <patch> support: applies the pack's IPS patch to an
 * in-memory copy of the ROM before the core loads it, exactly like
 * Mesen's automatic soft-patching.  The <patch> tag carries the SHA-1
 * of the ROM the patch expects; a mismatching ROM is left untouched
 * (with a log message) so wrong revisions are never corrupted. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hdpack.h"
#include "hdpack_data.h"

/* ---- SHA-1 (FIPS 180-1), small C89 implementation ------------------- */

typedef struct hd_sha1_ctx
{
   uint32_t h[5];
   uint32_t len_lo;
   uint32_t len_hi;
   uint8_t  block[64];
   uint32_t block_len;
} hd_sha1_ctx;

static uint32_t hd_rol32(uint32_t v, int bits)
{
   return (v << bits) | (v >> (32 - bits));
}

static void hd_sha1_init(hd_sha1_ctx *c)
{
   c->h[0] = 0x67452301u;
   c->h[1] = 0xEFCDAB89u;
   c->h[2] = 0x98BADCFEu;
   c->h[3] = 0x10325476u;
   c->h[4] = 0xC3D2E1F0u;
   c->len_lo = 0;
   c->len_hi = 0;
   c->block_len = 0;
}

static void hd_sha1_block(hd_sha1_ctx *c)
{
   uint32_t w[80];
   uint32_t a, b, d, e, f, k, t;
   uint32_t cc;
   int i;

   for (i = 0; i < 16; i++)
   {
      w[i] = ((uint32_t)c->block[i * 4] << 24) |
             ((uint32_t)c->block[i * 4 + 1] << 16) |
             ((uint32_t)c->block[i * 4 + 2] << 8) |
             (uint32_t)c->block[i * 4 + 3];
   }
   for (i = 16; i < 80; i++)
      w[i] = hd_rol32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

   a = c->h[0]; b = c->h[1]; cc = c->h[2]; d = c->h[3]; e = c->h[4];

   for (i = 0; i < 80; i++)
   {
      if (i < 20)
      {
         f = (b & cc) | ((~b) & d);
         k = 0x5A827999u;
      }
      else if (i < 40)
      {
         f = b ^ cc ^ d;
         k = 0x6ED9EBA1u;
      }
      else if (i < 60)
      {
         f = (b & cc) | (b & d) | (cc & d);
         k = 0x8F1BBCDCu;
      }
      else
      {
         f = b ^ cc ^ d;
         k = 0xCA62C1D6u;
      }
      t = hd_rol32(a, 5) + f + e + k + w[i];
      e = d;
      d = cc;
      cc = hd_rol32(b, 30);
      b = a;
      a = t;
   }

   c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d; c->h[4] += e;
   c->block_len = 0;
}

static void hd_sha1_update(hd_sha1_ctx *c, const uint8_t *data, size_t len)
{
   uint32_t lo = c->len_lo + (uint32_t)(len << 3);
   if (lo < c->len_lo)
      c->len_hi++;
   c->len_lo = lo;
   c->len_hi += (uint32_t)(len >> 29);

   while (len)
   {
      size_t take = 64 - c->block_len;
      if (take > len)
         take = len;
      memcpy(c->block + c->block_len, data, take);
      c->block_len += (uint32_t)take;
      data += take;
      len -= take;
      if (c->block_len == 64)
         hd_sha1_block(c);
   }
}

static void hd_sha1_final(hd_sha1_ctx *c, uint8_t digest[20])
{
   uint32_t lo = c->len_lo;
   uint32_t hi = c->len_hi;
   uint8_t pad = 0x80;
   uint8_t zero = 0;
   uint8_t lenb[8];
   int i;

   hd_sha1_update(c, &pad, 1);
   while (c->block_len != 56)
      hd_sha1_update(c, &zero, 1);

   lenb[0] = (uint8_t)(hi >> 24); lenb[1] = (uint8_t)(hi >> 16);
   lenb[2] = (uint8_t)(hi >> 8);  lenb[3] = (uint8_t)hi;
   lenb[4] = (uint8_t)(lo >> 24); lenb[5] = (uint8_t)(lo >> 16);
   lenb[6] = (uint8_t)(lo >> 8);  lenb[7] = (uint8_t)lo;
   hd_sha1_update(c, lenb, 8);

   for (i = 0; i < 5; i++)
   {
      digest[i * 4]     = (uint8_t)(c->h[i] >> 24);
      digest[i * 4 + 1] = (uint8_t)(c->h[i] >> 16);
      digest[i * 4 + 2] = (uint8_t)(c->h[i] >> 8);
      digest[i * 4 + 3] = (uint8_t)c->h[i];
   }
}

/* ---- IPS ------------------------------------------------------------- */

/* Applies an IPS patch to a copy of `in`.  Handles data records, RLE
 * records (size 0), the output growing past the input size, and the
 * common truncation extension (3-byte length after EOF).  Returns the
 * patched buffer (malloc'd) or NULL on malformed input. */
static uint8_t *hd_apply_ips(const uint8_t *ips, size_t ips_size,
      const uint8_t *in, size_t in_size, size_t *out_size)
{
   uint8_t *out;
   size_t out_alloc;
   size_t out_len = in_size;
   size_t pos = 5;

   if (ips_size < 8 || memcmp(ips, "PATCH", 5) != 0)
      return NULL;

   out_alloc = in_size + 0x10000;
   out = (uint8_t*)malloc(out_alloc);
   if (!out)
      return NULL;
   memcpy(out, in, in_size);

   for (;;)
   {
      uint32_t offset;
      uint32_t size;
      size_t end;

      if (pos + 3 > ips_size)
         goto malformed;
      if (memcmp(ips + pos, "EOF", 3) == 0)
      {
         pos += 3;
         /* truncation extension */
         if (pos + 3 <= ips_size)
         {
            uint32_t trunc = ((uint32_t)ips[pos] << 16) |
                  ((uint32_t)ips[pos + 1] << 8) | ips[pos + 2];
            if (trunc && trunc < out_len)
               out_len = trunc;
         }
         break;
      }

      offset = ((uint32_t)ips[pos] << 16) |
            ((uint32_t)ips[pos + 1] << 8) | ips[pos + 2];
      pos += 3;
      if (pos + 2 > ips_size)
         goto malformed;
      size = ((uint32_t)ips[pos] << 8) | ips[pos + 1];
      pos += 2;

      if (size == 0)
      {
         /* RLE record */
         uint32_t count;
         uint8_t value;
         if (pos + 3 > ips_size)
            goto malformed;
         count = ((uint32_t)ips[pos] << 8) | ips[pos + 1];
         value = ips[pos + 2];
         pos += 3;
         end = (size_t)offset + count;
         if (end > out_alloc)
         {
            uint8_t *no;
            while (out_alloc < end)
               out_alloc *= 2;
            no = (uint8_t*)realloc(out, out_alloc);
            if (!no)
               goto malformed;
            out = no;
         }
         if (end > out_len)
         {
            if ((size_t)offset > out_len)
               memset(out + out_len, 0, offset - out_len);
            out_len = end;
         }
         memset(out + offset, value, count);
      }
      else
      {
         if (pos + size > ips_size)
            goto malformed;
         end = (size_t)offset + size;
         if (end > out_alloc)
         {
            uint8_t *no;
            while (out_alloc < end)
               out_alloc *= 2;
            no = (uint8_t*)realloc(out, out_alloc);
            if (!no)
               goto malformed;
            out = no;
         }
         if (end > out_len)
         {
            if ((size_t)offset > out_len)
               memset(out + out_len, 0, offset - out_len);
            out_len = end;
         }
         memcpy(out + offset, ips + pos, size);
         pos += size;
      }
   }

   *out_size = out_len;
   return out;

malformed:
   free(out);
   return NULL;
}

/* ---- public entry point ------------------------------------------------ */

int HDNes_PatchRom(const uint8_t *rom, size_t rom_size,
      uint8_t **patched, size_t *patched_size)
{
   uint8_t digest[20];
   hd_sha1_ctx ctx;
   uint32_t i;

   *patched = NULL;
   *patched_size = 0;

   if (!hd.patch_count || !rom || !rom_size)
      return 0;

   hd_sha1_init(&ctx);
   hd_sha1_update(&ctx, rom, rom_size);
   hd_sha1_final(&ctx, digest);

   for (i = 0; i < hd.patch_count; i++)
   {
      if (memcmp(hd.patches[i].sha1, digest, 20) == 0)
      {
         uint8_t *ips = NULL;
         size_t ips_size = 0;
         uint8_t *out;
         size_t out_size = 0;

         if (!hd_pack_read_file(hd.patches[i].filename, &ips, &ips_size))
         {
            hd_log("Patch file not found: %s", hd.patches[i].filename);
            return 0;
         }
         out = hd_apply_ips(ips, ips_size, rom, rom_size, &out_size);
         free(ips);
         if (!out)
         {
            hd_log("Invalid IPS patch: %s", hd.patches[i].filename);
            return 0;
         }
         hd_log("Applied patch: %s", hd.patches[i].filename);
         *patched = out;
         *patched_size = out_size;
         return 1;
      }
   }

   hd_log("The loaded ROM does not match any <patch> SHA-1 in this "
         "pack; running unpatched.");
   return 0;
}
