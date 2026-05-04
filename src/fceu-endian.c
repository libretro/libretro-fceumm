/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*  Contains file I/O functions that write/read data    */
/*  LSB first.              */

#include <string.h>

#include "fceu-memory.h"
#include "fceu-types.h"
#include "fceu-endian.h"

void FlipByteOrder(uint8_t *src, uint32_t count)
{
   uint8_t *start = src;
   uint8_t *end;

   if ((count & 1) || !count)
      return;     /* This shouldn't happen. */

   end = src + count - 1;
   /* Iterate while start < end, not 'count' times: the original loop
    * over-iterated and effectively performed a no-op on every even count.
    * That broke savestate portability across LE<->BE for every
    * FCEUSTATE_RLSB-marked field. */
   while (start < end)
   {
      uint8_t tmp;

      tmp = *end;
      *end = *start;
      *start = tmp;
      end--;
      start++;
   }
}

void FlipByteOrderStrided(uint8_t *src, uint32_t count, uint32_t stride)
{
   uint32_t off;

   if (stride <= 1 || !count)
      return;
   if (count % stride)
      return;     /* malformed - bail rather than half-swap */

   /* For arrays of multi-byte primitives we byte-swap each element
    * independently. The previous code (FlipByteOrder) reversed the
    * entire buffer end-to-end, which is correct for a single
    * primitive but wrong for an array - it would swap element 0
    * with element N-1 and reverse their bytes too, scrambling the
    * data. With per-element strides, [a0 a1 b0 b1] becomes
    * [a1 a0 b1 b0] (correct), not [b1 b0 a1 a0] (legacy bug). */
   for (off = 0; off < count; off += stride)
   {
      uint8_t *s = src + off;
      uint8_t *e = s + stride - 1;
      while (s < e)
      {
         uint8_t tmp = *e;
         *e = *s;
         *s = tmp;
         e--;
         s++;
      }
   }
}

int write32le_mem(uint32_t b, memstream_t *mem)
{
   uint8_t s[4];
#ifndef MSB_FIRST
   /* On LE hosts the in-memory representation already matches the
    * on-disk LE format. Copy the four bytes directly, letting the
    * compiler emit a single 32-bit store. */
   memcpy(s, &b, 4);
#else
   s[0]=b;
   s[1]=b>>8;
   s[2]=b>>16;
   s[3]=b>>24;
#endif
   return((memstream_write(mem, s, 4)<4)?0:4);
}

int read32le_mem(uint32_t *Bufo, memstream_t *mem)
{
   uint32_t buf;
   if(memstream_read(mem, &buf, 4)<4)
      return 0;
#ifdef MSB_FIRST
   *Bufo = ((buf&0xFF)<<24)|((buf&0xFF00)<<8)|((buf&0xFF0000)>>8)|((buf&0xFF000000)>>24);
#else
   /* LE: bytes already in place. Use memcpy rather than the previous
    * '*(uint32_t*)Bufo = buf;' which aliased through a uint32_t* into
    * what's nominally a uint32_t already - functionally equivalent
    * but cleaner. */
   *Bufo = buf;
#endif
   return 1;
}

void FCEU_en32lsb(uint8_t *buf, uint32_t morp)
{
#ifndef MSB_FIRST
   /* LE in-memory layout matches the desired LE on-disk layout. */
   memcpy(buf, &morp, 4);
#else
   buf[0] = morp;
   buf[1] = morp >> 8;
   buf[2] = morp >> 16;
   buf[3] = morp >> 24;
#endif
}

uint32_t FCEU_de32lsb(const uint8_t *morp)
{
#ifndef MSB_FIRST
   uint32_t v;
   memcpy(&v, morp, 4);
   return v;
#else
   return(morp[0] | (morp[1] << 8) | (morp[2] << 16) | (morp[3] << 24));
#endif
}
