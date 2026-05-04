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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <compat/strl.h>

#include "fceu-types.h"
#include "x6502.h"
#include "fceu.h"
#include "sound.h"
#include "fceu-endian.h"
#include "fds.h"
#include "general.h"
#include "state.h"
#include "fceu-memory.h"
#include "ppu.h"
#include "video.h"

static void (*SPreSave)(void);
static void (*SPostSave)(void);

/* static int SaveStateStatus[10]; */

static SFORMAT SFMDATA[64];
int SFEXINDEX;

/* Convenience aliases for the SFORMAT 's' field bit layout. The
 * encoding is documented in state.h. */
#define RLSB         FCEUSTATE_RLSB
#define STRIDE_MASK  FCEUSTATE_STRIDE_MASK
#define STRIDE_SHIFT FCEUSTATE_STRIDE_SHIFT
#define SIZE_MASK    FCEUSTATE_SIZE_MASK

/* Extract the on-disk byte count of an SFORMAT entry. */
static INLINE uint32_t sf_size(uint32_t s) { return s & SIZE_MASK; }

/* Extract the byte stride (the size in bytes of one element of an
 * RLSB array). When the stride bits are zero we fall back to
 * "whole entry is one element", preserving the legacy single-
 * primitive interpretation that pre-stride code relied on. */
static INLINE uint32_t sf_stride(uint32_t s)
{
   uint32_t st = (s & STRIDE_MASK) >> STRIDE_SHIFT;
   if (st)
      return st;
   return s & SIZE_MASK;
}

/* Byte-swap an SFORMAT entry's value buffer in place. Only called
 * when MSB_FIRST is defined - on LE hosts the on-disk LE format
 * matches the in-memory layout and no swap is needed. */
static INLINE void sf_flip(void *v, uint32_t s)
{
   uint32_t size   = sf_size(s);
   uint32_t stride = sf_stride(s);
   if (stride == size)
      FlipByteOrder((uint8_t *)v, size);
   else
      FlipByteOrderStrided((uint8_t *)v, size, stride);
}

extern SFORMAT FCEUPPU_STATEINFO[];
extern SFORMAT FCEUSND_STATEINFO[];
extern SFORMAT FCEUCTRL_STATEINFO[];

SFORMAT SFCPU[] = {
   { &X.PC, 2 | RLSB, "PC\0" },
   { &X.A, 1, "A\0\0" },
   { &X.X, 1, "X\0\0" },
   { &X.Y, 1, "Y\0\0" },
   { &X.S, 1, "S\0\0" },
   { &X.P, 1, "P\0\0" },
   { &X.DB, 1, "DB"},
   { RAM, 0x800, "RAM" },
   { 0 }
};

SFORMAT SFCPUC[] = {
   { &X.jammed, 1, "JAMM" },
   { &X.IRQlow, 4 | RLSB, "IQLB" },
   { &X.tcount, 4 | RLSB, "ICoa" },
   { &X.count, 4 | RLSB, "ICou" },
   { &timestampbase, sizeof(timestampbase) | RLSB, "TSBS" },
   { &X.mooPI, 1, "MooP"},
   { 0 }
};

static int SubWrite(memstream_t *mem, SFORMAT *sf)
{
   uint32_t acc = 0;

   while(sf->v)
   {
      if(sf->s == (~(uint32_t)0)) /* Link to another struct. */
      {
         uint32_t tmp;

         if(!(tmp = SubWrite(mem, (SFORMAT *)sf->v)))
            return(0);
         acc += tmp;
         sf++;
         continue;
      }

      acc += 8; /* Description + size */
      acc += sf_size(sf->s);

      if(mem) /* Are we writing or calculating the size of this block? */
      {
         memstream_write(mem, sf->desc, 4);
         write32le_mem(sf_size(sf->s), mem);

#ifdef MSB_FIRST
         if(sf->s & RLSB)
            sf_flip(sf->v, sf->s);
#endif
         memstream_write(mem, (char *)sf->v, sf_size(sf->s));

         /* Now restore the original byte order. */
#ifdef MSB_FIRST
         if(sf->s & RLSB)
            sf_flip(sf->v, sf->s);
#endif
      }
      sf++;
   }

   return acc;
}

static int WriteStateChunk(memstream_t *mem, int type, SFORMAT *sf)
{
   int bsize;

   memstream_putc(mem, type);

   bsize = SubWrite(0, sf);
   write32le_mem(bsize, mem);

   if (!SubWrite(mem, sf))
      return 0;
   return bsize + 5;
}

static SFORMAT *CheckS(SFORMAT *sf, uint32_t tsize, char *desc)
{
   while (sf->v)
   {
      if (sf->s == (~(uint32_t)0))
      { /* Link to another SFORMAT structure. */
         SFORMAT *tmp;
         if ((tmp = CheckS((SFORMAT*)sf->v, tsize, desc)))
            return(tmp);
         sf++;
         continue;
      }
      if (!strncmp(desc, sf->desc, 4))
      {
         if (tsize != sf_size(sf->s))
            return(0);
         return(sf);
      }
      sf++;
   }
   return(0);
}

static int ReadStateChunk(memstream_t *mem, SFORMAT *sf, int size)
{
   SFORMAT *tmp;
   uint64_t temp;
   temp = memstream_pos(mem);

   while(memstream_pos(mem) < (temp + size))
   {
      uint32_t tsize;
      char toa[4];
      /* memstream_read returns the number of bytes read, or 0/negative on
       * error. Treat anything less than the requested four bytes as a
       * truncation; the previous <= 0 check let a 1- to 3-byte short read
       * leave toa[] partially uninitialised before strncmp consumed it. */
      if (memstream_read(mem, toa, 4) != 4u)
         return 0;

      if (!read32le_mem(&tsize, mem))
         return 0;

      if((tmp = CheckS(sf, tsize, toa)))
      {
         /* sf_size(tmp->s) was already validated to equal tsize by
          * CheckS, and tmp->v points at the fixed-size in-memory buffer
          * for this state field, so the read is bounded. Still treat a
          * short read here as a parse failure rather than leaving the
          * buffer in a half-updated state. */
         if (memstream_read(mem, (char *)tmp->v, sf_size(tmp->s)) != (uint64_t)sf_size(tmp->s))
            return 0;

#ifdef MSB_FIRST
         if(tmp->s & RLSB)
            sf_flip(tmp->v, tmp->s);
#endif
      }
      else
      {
         /* Skip past the unknown chunk's payload. memstream_seek already
          * bounds-checks against the stream end and returns -1 on
          * out-of-range, but the caller previously ignored the return,
          * leaving the read loop spinning at the same offset and re-
          * parsing whatever bytes happened to follow. Treat a failed
          * skip as a parse error so we don't silently load a truncated
          * state. */
         if (memstream_seek(mem, tsize, SEEK_CUR) < 0)
            return 0;
      }
   }
   return 1;
}

static int ReadStateChunks(memstream_t *st, int32_t totalsize)
{
   int t;
   uint32_t size;
   int ret = 1;

   while (totalsize > 0)
   {
      t = memstream_getc(st);
      if (t == EOF)
         break;
      if (!read32le_mem(&size, st))
         break;
      /* size came straight off disk and is attacker-controlled. The
       * arithmetic below was previously
       *
       *   totalsize -= size + 5;
       *
       * but `size + 5` is computed as uint32_t and can wrap to a small
       * value (e.g. size = 0xFFFFFFFB makes size + 5 == 0), so totalsize
       * fails to decrement and the outer loop spins through the rest of
       * the stream. Reject any size that would either (a) overflow the
       * +5 housekeeping bytes or (b) exceed the remaining declared
       * payload, which is the only legitimate range. */
      if (size > (uint32_t)0x7FFFFFFFu - 5
            || (uint32_t)totalsize < size + 5)
         break;
      totalsize -= (int32_t)(size + 5);

      switch(t)
      {
         case 1:
            if (!ReadStateChunk(st, SFCPU, size))
               ret = 0;
            break;
         case 2:
            if (!ReadStateChunk(st, SFCPUC, size))
               ret = 0;
            /* else
               X.mooPI = X.P; */ /* Quick and dirty hack. */
            break;
         case 3:
            if (!ReadStateChunk(st, FCEUPPU_STATEINFO, size))
               ret = 0;
            break;
         case 4:
            if (!ReadStateChunk(st, FCEUCTRL_STATEINFO, size))
               ret = 0;
            break;
         case 5:
            if (!ReadStateChunk(st, FCEUSND_STATEINFO, size))
               ret = 0;
            break;
         case 0x10:
            if (!ReadStateChunk(st, SFMDATA, size))
               ret = 0;
            break;
         default:
            if (memstream_seek(st, size, SEEK_CUR) < 0)
               goto endo;
            break;
      }
   }
endo:
   return ret;
}

extern int geniestage;

void FCEUSS_Save_Mem(void)
{
   memstream_t *mem = memstream_open(1);

   uint32_t totalsize;
   uint8_t header[16] = {0};

   header[0] = 'F';
   header[1] = 'C';
   header[2] = 'S';
   header[3] = 0xFF;

   FCEU_en32lsb(header + 8, FCEU_VERSION_NUMERIC);
   memstream_write(mem, header, 16);

   FCEUPPU_SaveState();
   totalsize  = WriteStateChunk(mem, 1, SFCPU);
   totalsize += WriteStateChunk(mem, 2, SFCPUC);
   totalsize += WriteStateChunk(mem, 3, FCEUPPU_STATEINFO);
   totalsize += WriteStateChunk(mem, 4, FCEUCTRL_STATEINFO);
   totalsize += WriteStateChunk(mem, 5, FCEUSND_STATEINFO);

   if (SPreSave)
      SPreSave();

   totalsize += WriteStateChunk(mem, 0x10, SFMDATA);

   if (SPostSave)
      SPostSave();

   memstream_seek(mem, 4, SEEK_SET);
   write32le_mem(totalsize, mem);

   memstream_close(mem);
}

void FCEUSS_Load_Mem(void)
{
   memstream_t *mem = memstream_open(0);

   uint8_t header[16] = {0};
   int stateversion;
   uint32_t totalsize_u;
   int32_t totalsize;
   int x;

   /* memstream_open can't legitimately return NULL in the libretro path
    * (the buffer is set by retro_unserialize via memstream_set_buffer),
    * but treat NULL defensively rather than dereferencing. */
   if (!mem)
      return;

   /* If the buffer is shorter than the 16-byte header the read returns
    * fewer bytes; the magic-string memcmp would otherwise compare against
    * a stack-uninitialised tail. Zeroed header[] above plus the read-
    * length check below leave the comparisons well-defined. */
   if (memstream_read(mem, header, 16) != 16u)
   {
      memstream_close(mem);
      return;
   }

   if (memcmp(header, "FCS", 3) != 0)
   {
      memstream_close(mem);
      return;
   }

   if (header[3] == 0xFF)
      stateversion = FCEU_de32lsb(header + 8);
   else
      stateversion = header[3] * 100;

   /* totalsize on disk is unsigned 32-bit. The previous code stored it
    * straight into a signed int, so a malicious header with bit 31 set
    * would silently produce a negative totalsize - the chunk loop would
    * then never enter and the savestate would "load" without applying
    * any of its data. Read into a uint32_t and reject obviously bogus
    * sizes that exceed INT32_MAX or what's actually in the buffer. */
   totalsize_u = FCEU_de32lsb(header + 4);
   if (totalsize_u > 0x7FFFFFFFu)
   {
      memstream_close(mem);
      return;
   }
   totalsize = (int32_t)totalsize_u;

   x = ReadStateChunks(mem, totalsize);

   if (stateversion < 9500)
      X.IRQlow = 0;

   if (GameStateRestore)
      GameStateRestore(stateversion);

   if (x)
   {
      FCEUPPU_LoadState(stateversion);
      FCEUSND_LoadState(stateversion);
   }

   memstream_close(mem);
}

void ResetExState(void (*PreSave)(void), void (*PostSave)(void))
{
   SPreSave  = PreSave;
   SPostSave = PostSave;
   SFEXINDEX = 0;
   SFMDATA[0].s = 0;
}

void AddExState(void *v, uint32_t s, int type, char *desc)
{
   /* prevent adding a terminator to the list if a NULL pointer was provided */
   if (v == NULL) return;
   /* Need at least one slot for the new entry plus one for the terminator.
    * If we're already at the second-to-last slot (62), we can write the
    * entry there and the terminator at slot 63, but no further entries
    * may be added. Beyond that, drop the entry to avoid silently
    * overwriting the previous one with the terminator. */
   if (SFEXINDEX >= (int)(sizeof(SFMDATA) / sizeof(SFMDATA[0])) - 1)
   {
      static int warned = 0;
      if (!warned)
      {
         FCEU_PrintError(" SFMDATA[] full; some state entries dropped (\"%s\").\n",
               desc ? desc : "?");
         warned = 1;
      }
      return;
   }
   memset(SFMDATA[SFEXINDEX].desc, 0, sizeof(SFMDATA[SFEXINDEX].desc));
   if (desc)
      strlcpy(SFMDATA[SFEXINDEX].desc, desc, sizeof(SFMDATA[SFEXINDEX].desc));
   SFMDATA[SFEXINDEX].v = v;
   SFMDATA[SFEXINDEX].s = s;
   if (type)
      SFMDATA[SFEXINDEX].s |= RLSB;
   SFEXINDEX++;
   SFMDATA[SFEXINDEX].v = 0;   /* End marker. */
}

void FCEU_DrawSaveStates(uint8_t *XBuf)
{
}

