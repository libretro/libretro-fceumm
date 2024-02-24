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
#include "gamegenie.h"

#define SFMDATA_SIZE 128
#define RLSB         FCEUSTATE_RLSB

static void (*SPreSave)(void);
static void (*SPostSave)(void);

static SFORMAT SFMDATA[SFMDATA_SIZE];
static int SFEXINDEX;

extern SFORMAT FCEUPPU_STATEINFO[];
extern SFORMAT FCEUSND_STATEINFO[];
extern SFORMAT FCEUCTRL_STATEINFO[];

SFORMAT SFCPU[] = {
   { &cpu.PC, 2 | RLSB, "PC\0" },
   { &cpu.A, 1, "A\0\0" },
   { &cpu.X, 1, "X\0\0" },
   { &cpu.Y, 1, "Y\0\0" },
   { &cpu.S, 1, "S\0\0" },
   { &cpu.P, 1, "P\0\0" },
   { &cpu.openbus, 1, "DB\0"},
   { &RAM, RAM_SIZE | FCEUSTATE_INDIRECT, "RAM" },
   { 0 }
};

SFORMAT SFCPUC[] = {
   { &cpu.jammed, 1, "JAMM" },
   { &cpu.IRQlow, 4 | RLSB, "IQLB" },
   { &cpu.tcount, 4 | RLSB, "ICoa" },
   { &cpu.count, 4 | RLSB, "ICou" },
   { &timestampbase, sizeof(timestampbase) | RLSB, "TSBS" },
   { &cpu.mooPI, 1, "MooP"},
   { 0 }
};

static int SubWrite(memstream_t *mem, SFORMAT *sf)
{
   uint32 acc = 0;

   while(sf->v)
   {
      if(sf->s == (~(uint32)0)) /* Link to another struct. */
      {
         uint32 tmp;

         if(!(tmp = SubWrite(mem, (SFORMAT *)sf->v)))
            return(0);
         acc += tmp;
         sf++;
         continue;
      }

      acc += 8; /* Description + size */
      acc += sf->s & (~FCEUSTATE_FLAGS);

      if(mem) /* Are we writing or calculating the size of this block? */
      {
         memstream_write(mem, sf->desc, 4);
         write32le_mem(sf->s & (~FCEUSTATE_FLAGS), mem);

#ifdef MSB_FIRST
         if(sf->s & RLSB)
            FlipByteOrder((uint8 *)sf->v, sf->s & (~FCEUSTATE_FLAGS));
#endif
         if (sf->s & FCEUSTATE_INDIRECT) {
            memstream_write(mem, *(char **)sf->v, sf->s & (~FCEUSTATE_FLAGS));
         } else {
            memstream_write(mem, (char *)sf->v, sf->s & (~FCEUSTATE_FLAGS));
         }

         /* Now restore the original byte order. */
#ifdef MSB_FIRST
         if(sf->s & RLSB)
            FlipByteOrder((uint8 *)sf->v, sf->s & (~FCEUSTATE_FLAGS));
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

static SFORMAT *CheckS(SFORMAT *sf, uint32 tsize, char *desc)
{
   while (sf->v)
   {
      if (sf->s == (~(uint32)0))
      { /* Link to another SFORMAT structure. */
         SFORMAT *tmp;
         if ((tmp = CheckS((SFORMAT*)sf->v, tsize, desc)))
            return(tmp);
         sf++;
         continue;
      }
      if (!strncmp(desc, sf->desc, 4))
      {
         if (tsize != (sf->s & (~FCEUSTATE_FLAGS)))
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
   uint64 temp;
   temp = memstream_pos(mem);

   while(memstream_pos(mem) < (temp + size))
   {
      uint32 tsize;
      char toa[4];
      if(memstream_read(mem, toa, 4) <= 0)
         return 0;

      read32le_mem(&tsize, mem);

      if((tmp = CheckS(sf, tsize, toa)))
      {
         if (tmp->s & FCEUSTATE_INDIRECT) {
            memstream_read(mem, *(char **)tmp->v, tmp->s & (~FCEUSTATE_FLAGS));
         } else {
            memstream_read(mem, (char *)tmp->v, tmp->s & (~FCEUSTATE_FLAGS));
         }

#ifdef MSB_FIRST
         if(tmp->s & RLSB)
            FlipByteOrder((uint8 *)tmp->v, tmp->s & (~FCEUSTATE_FLAGS));
#endif
      }
      else
         memstream_seek(mem, tsize, SEEK_CUR);
   }
   return 1;
}

static int ReadStateChunks(memstream_t *st, int32 totalsize)
{
   int t;
   uint32 size;
   int ret = 1;

   while (totalsize > 0)
   {
      t = memstream_getc(st);
      if (t == EOF)
         break;
      if (!read32le_mem(&size, st))
         break;
      totalsize -= size + 5;

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
               cpu.mooPI = cpu.P; */ /* Quick and dirty hack. */
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

void FCEUSS_Save_Mem(void)
{
   memstream_t *mem = memstream_open(1);
   uint8 header[16] = {0};
   size_t totalsize = 0;

   header[0] = 'F';
   header[1] = 'C';
   header[2] = 'S';
   header[3] = 'M';

   FCEU_en32lsb(header + 8, FCEU_VERSION_NUMERIC);
   memstream_write(mem, header, 16);

   FCEUPPU_SaveState();
   FCEUSND_SaveState();

   totalsize  = WriteStateChunk(mem, 1, SFCPU);
   totalsize += WriteStateChunk(mem, 2, SFCPUC);
   totalsize += WriteStateChunk(mem, 3, FCEUPPU_STATEINFO);
   totalsize += WriteStateChunk(mem, 4, FCEUCTRL_STATEINFO);
   totalsize += WriteStateChunk(mem, 5, FCEUSND_STATEINFO);

   if (SPreSave) SPreSave();
   totalsize += WriteStateChunk(mem, 0x10, SFMDATA);
   if (SPostSave) SPostSave();

   memstream_seek(mem, 4, SEEK_SET);
   write32le_mem(totalsize, mem);

   memstream_close(mem);
}

void FCEUSS_Load_Mem(void)
{
   memstream_t *mem = memstream_open(0);
   uint8 header[16] = {0};
   size_t totalsize = 0;
   int stateversion = 0;
   int x = 0;

   memstream_read(mem, header, 16);

   if (memcmp(header, "FCSM", 4) != 0)
      return;

   totalsize = FCEU_de32lsb(header + 4);
   stateversion = FCEU_de32lsb(header + 8);

   x = ReadStateChunks(mem, totalsize);

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

void AddExState(void *v, uint32 s, int type, char *desc)
{
   /* prevent adding a terminator to the list if a NULL pointer was provided */
   if (v == NULL) return;
   memset(SFMDATA[SFEXINDEX].desc, 0, sizeof(SFMDATA[SFEXINDEX].desc));
   if (desc)
      strncpy(SFMDATA[SFEXINDEX].desc, desc, sizeof(SFMDATA[SFEXINDEX].desc));
   SFMDATA[SFEXINDEX].v = v;
   SFMDATA[SFEXINDEX].s = s;
   if (type)
      SFMDATA[SFEXINDEX].s |= RLSB;
   if (SFEXINDEX < (SFMDATA_SIZE - 1)) {
      SFEXINDEX++;
   } else {
      static int warn_once = 1;
      if (warn_once) {
         FCEU_printf("\n");
         FCEU_PrintError(" Error in AddExState: SFEINDEX overflow. SFMDATA_SIZE too small.\n");
         FCEU_printf("\n");
         warn_once = 0;
      }
   }
   SFMDATA[SFEXINDEX].v = 0;   /* End marker. */
}
