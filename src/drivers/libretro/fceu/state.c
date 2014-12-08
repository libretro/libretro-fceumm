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
#include "movie.h"
#include "fceu-memory.h"
#include "ppu.h"
#include "netplay.h"
#include "video.h"

static void (*SPreSave)(void);
static void (*SPostSave)(void);

static int SaveStateStatus[10];

static SFORMAT SFMDATA[64];
static int SFEXINDEX;

#define RLSB     FCEUSTATE_RLSB	//0x80000000

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
#ifdef COPYFAMI
	{ RAM, 0x4000, "RAM" },
#else
	{ RAM, 0x800, "RAM" },
#endif
	{ 0 }
};

SFORMAT SFCPUC[] = {
	{ &X.jammed, 1, "JAMM" },
	{ &X.IRQlow, 4 | RLSB, "IQLB" },
	{ &X.tcount, 4 | RLSB, "ICoa" },
	{ &X.count, 4 | RLSB, "ICou" },
	{ &timestampbase, sizeof(timestampbase) | RLSB, "TSBS" },
	{ 0 }
};

static int SubWrite_Mem(memstream_t *mem, SFORMAT *sf)
{
   uint32 acc=0;

   while(sf->v)
   {
      if(sf->s==~0) /* Link to another struct. */
      {
         uint32 tmp;

         if(!(tmp=SubWrite_Mem(mem, (SFORMAT *)sf->v)))
            return(0);
         acc+=tmp;
         sf++;
         continue;
      }

      acc+=8; /* Description + size */
      acc+=sf->s&(~RLSB);

      if(mem) /* Are we writing or calculating the size of this block? */
      {
         memstream_write(mem, sf->desc, 4);
         write32le_mem(sf->s&(~RLSB), mem);

#ifndef LSB_FIRST
         if(sf->s&RLSB)
            FlipByteOrder(sf->v,sf->s&(~RLSB));
#endif

         memstream_write(mem, (uint8 *)sf->v, sf->s&(~RLSB));
         /* Now restore the original byte order. */
#ifndef LSB_FIRST
         if(sf->s&RLSB)
            FlipByteOrder(sf->v,sf->s&(~RLSB));
#endif
      }
      sf++;
   }

   return acc;
}

static int SubWrite(MEM_TYPE *st, SFORMAT *sf) {
	uint32 acc = 0;

	while (sf->v) {
		if (sf->s == ~0) {	// Link to another struct.
			uint32 tmp;

			if (!(tmp = SubWrite(st, (SFORMAT*)sf->v)))
				return(0);
			acc += tmp;
			sf++;
			continue;
		}

		acc += 8;		// Description + size
		acc += sf->s & (~RLSB);

		if (st) {	// Are we writing or calculating the size of this block? */
			fwrite(sf->desc, 1, 4, st);
			write32le(sf->s & (~RLSB), st);

			#ifndef LSB_FIRST
			if (sf->s & RLSB)
				FlipByteOrder(sf->v, sf->s & (~RLSB));
			#endif

			fwrite((uint8*)sf->v, 1, sf->s & (~RLSB), st);
			/* Now restore the original byte order. */
			#ifndef LSB_FIRST
			if (sf->s & RLSB)
				FlipByteOrder(sf->v, sf->s & (~RLSB));
			#endif
		}
		sf++;
	}

	return(acc);
}

static int WriteStateChunk(MEM_TYPE *st, int type, SFORMAT *sf)
{
   int bsize;

   fputc(type, st);

   bsize = SubWrite(0, sf);
   write32le(bsize, st);

   if (!SubWrite(st, sf))
      return(0);
   return(bsize + 5);
}

static int WriteStateChunk_Mem(memstream_t *mem, int type, SFORMAT *sf)
{
   int bsize;

   memstream_putc(mem, type);

   bsize = SubWrite(0,sf);
   write32le_mem(bsize, mem);

   if (!SubWrite_Mem(mem, sf))
      return 0;
   return bsize + 5;
}

static SFORMAT *CheckS(SFORMAT *sf, uint32 tsize, char *desc)
{
   while (sf->v)
   {
      if (sf->s == ~0)
      {	/* Link to another SFORMAT structure. */
         SFORMAT *tmp;
         if ((tmp = CheckS((SFORMAT*)sf->v, tsize, desc)))
            return(tmp);
         sf++;
         continue;
      }
      if (!strncmp(desc, sf->desc, 4))
      {
         if (tsize != (sf->s & (~RLSB)))
            return(0);
         return(sf);
      }
      sf++;
   }
   return(0);
}

static int ReadStateChunk_Mem(memstream_t *mem, SFORMAT *sf, int size)
{
   SFORMAT *tmp;
   int temp;
   temp=memstream_pos(mem);

   while(memstream_pos(mem)<temp+size)
   {
      uint32 tsize;
      char toa[4];
      if(memstream_read(mem, toa, 4)<=0)
         return 0;

      read32le_mem(&tsize,mem);

      if((tmp=CheckS(sf,tsize,toa)))
      {
         memstream_read(mem, (uint8 *)tmp->v, tmp->s&(~RLSB));

#ifndef LSB_FIRST
         if(tmp->s&RLSB)
            FlipByteOrder(tmp->v,tmp->s&(~RLSB));
#endif
      }
      else
         memstream_seek(mem,tsize,SEEK_CUR);
   }
   return 1;
}

static int ReadStateChunk(MEM_TYPE *st, SFORMAT *sf, int size)
{
	SFORMAT *tmp;
	int temp;
	temp = ftell(st);

	while (ftell(st) < temp + size)
   {
      uint32 tsize;
      char toa[4];
      if (fread(toa, 1, 4, st) <= 0)
         return 0;

      read32le(&tsize, st);

      if ((tmp = CheckS(sf, tsize, toa))) {
         fread((uint8*)tmp->v, 1, tmp->s & (~RLSB), st);

#ifndef LSB_FIRST
         if (tmp->s & RLSB)
            FlipByteOrder(tmp->v, tmp->s & (~RLSB));
#endif
      }
      else
         fseek(st, tsize, SEEK_CUR);
   }
	return 1;
}

static int ReadStateChunks_Mem(memstream_t *st, int32 totalsize)
{
   int t;
   uint32 size;
   int ret = 1;

   while (totalsize > 0)
   {
      t = memstream_getc(st);
      if (t == EOF)
         break;
      if (!read32le_mem(&size,st))
         break;
      totalsize -= size + 5;

      switch(t)
      {
         case 1:
            if (!ReadStateChunk_Mem(st, SFCPU, size))
               ret = 0;
            break;
         case 2:
            if (!ReadStateChunk_Mem(st, SFCPUC, size))
               ret = 0;
            else
               X.mooPI = X.P; // Quick and dirty hack.
            break;
         case 3:
            if (!ReadStateChunk_Mem(st, FCEUPPU_STATEINFO, size))
               ret = 0;
            break;
         case 4:
            if (!ReadStateChunk_Mem(st, FCEUCTRL_STATEINFO, size))
               ret = 0;
            break;
         case 5:
            if (!ReadStateChunk_Mem(st, FCEUSND_STATEINFO, size))
               ret = 0;
            break;
         case 0x10:
            if (!ReadStateChunk_Mem(st, SFMDATA, size))
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

static int ReadStateChunks(MEM_TYPE *st, int32 totalsize)
{
   int t;
   uint32 size;
   int ret = 1;

   while (totalsize > 0)
   {
      t = fgetc(st);
      if (t == EOF)
         break;
      if (!read32le(&size, st))
         break;
      totalsize -= size + 5;

      switch (t)
      {
         case 1:
            if (!ReadStateChunk(st, SFCPU, size))
               ret = 0;
            break;
         case 2:
            if (!ReadStateChunk(st, SFCPUC, size))
               ret = 0;
            else
               X.mooPI = X.P;	// Quick and dirty hack.
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
            if (fseek(st, size, SEEK_CUR) < 0)
               goto endo;
            break;
      }
   }
endo:
   return ret;
}


int CurrentState = 0;
extern int geniestage;

int FCEUSS_SaveFP(MEM_TYPE *st)
{
	static uint32 totalsize;
	uint8 header[16] = { 0 };

	header[0] = 'F';
	header[1] = 'C';
	header[2] = 'S';
	header[3] = 0xFF;

	header[3] = 0xFF;
	FCEU_en32lsb(header + 8, FCEU_VERSION_NUMERIC);
	fwrite(header, 1, 16, st);
	FCEUPPU_SaveState();
	FCEUSND_SaveState();
	totalsize = WriteStateChunk(st, 1, SFCPU);
	totalsize += WriteStateChunk(st, 2, SFCPUC);
	totalsize += WriteStateChunk(st, 3, FCEUPPU_STATEINFO);
	totalsize += WriteStateChunk(st, 4, FCEUCTRL_STATEINFO);
	totalsize += WriteStateChunk(st, 5, FCEUSND_STATEINFO);
	if (SPreSave)
      SPreSave();
	totalsize += WriteStateChunk(st, 0x10, SFMDATA);
	if (SPreSave)
      SPostSave();

	fseek(st, 4, SEEK_SET);
	write32le(totalsize, st);
	return(1);
}

void FCEUSS_Save_Mem(void)
{
   memstream_t *mem = memstream_open(1);

   static uint32 totalsize;
   static uint8 header[16] = "FCS";

   memset(header + 4, 0, 12);
   header[3] = 0xFF;
   FCEU_en32lsb(header + 8, FCEU_VERSION_NUMERIC);
   memstream_write(mem, header, 16);

   FCEUPPU_SaveState();
   totalsize = WriteStateChunk_Mem(mem, 1, SFCPU);
   totalsize += WriteStateChunk_Mem(mem, 2, SFCPUC);
   totalsize += WriteStateChunk_Mem(mem, 3, FCEUPPU_STATEINFO);
   totalsize += WriteStateChunk_Mem(mem, 4, FCEUCTRL_STATEINFO);
   totalsize += WriteStateChunk_Mem(mem, 5, FCEUSND_STATEINFO);

   if (SPreSave)
      SPreSave();

   totalsize += WriteStateChunk_Mem(mem, 0x10, SFMDATA);

   if (SPreSave)
      SPostSave();

   memstream_seek(mem, 4, SEEK_SET);
   write32le_mem(totalsize, mem);

   memstream_close(mem);
}

void FCEUSS_Save(char *fname)
{
	MEM_TYPE *st = NULL;
	char *fn;

	if (geniestage == 1)
   {
		FCEU_DispMessage("Cannot save FCS in GG screen.");
		return;
	}

	st = (MEM_TYPE*)memstream_open(1);

	FCEUSS_SaveFP(st);

	SaveStateStatus[CurrentState] = 1;
	fclose(st);
}

int FCEUSS_LoadFP(MEM_TYPE *st)
{
   int x;
   uint8 header[16];
   int stateversion;

   fread(&header, 1, 16, st);
   if (memcmp(header, "FCS", 3))
      return(0);

   if (header[3] == 0xFF)
      stateversion = FCEU_de32lsb(header + 8);
   else
      stateversion = header[3] * 100;

   x = ReadStateChunks(st, *(uint32*)(header + 4));
   if (stateversion < 9500)
      X.IRQlow = 0;

   if (GameStateRestore)
      GameStateRestore(stateversion);
   if (x)
   {
      FCEUPPU_LoadState(stateversion);
      FCEUSND_LoadState(stateversion);
   }
   return(x);
}

void FCEUSS_Load_Mem(void)
{
   memstream_t *mem = memstream_open(0);

   uint8 header[16];
   int stateversion;

   memstream_read(mem, header, 16);

   if (memcmp(header,"FCS",3) != 0)
      return;

   if (header[3] == 0xFF)
      stateversion = FCEU_de32lsb(header + 8);
   else
      stateversion = header[3] * 100;

   int x = ReadStateChunks_Mem(mem, *(uint32*)(header + 4));

   if (stateversion < 9500)
      X.IRQlow=0;

   if (GameStateRestore)
      GameStateRestore(stateversion);

   if (x)
   {
      FCEUPPU_LoadState(stateversion);
      FCEUSND_LoadState(stateversion);
   }

   memstream_close(mem);
}

int FCEUSS_Load(char *fname)
{
	MEM_TYPE *st;
	char *fn;

	st = (MEM_TYPE*)memstream_open(0);

	if (FCEUSS_LoadFP(st))
   {
		SaveStateStatus[CurrentState] = 1;
		fclose(st);
		return(1);
	}
   else
   {
		SaveStateStatus[CurrentState] = 1;
		fclose(st);
		return(0);
	}
}

void FCEUSS_CheckStates(void)
{
	MEM_TYPE *st = NULL;
	char *fn;
	int ssel;

	for (ssel = 0; ssel < 10; ssel++)
   {
		st = FCEUD_UTF8fopen(fn = FCEU_MakeFName(FCEUMKF_STATE, ssel, 0), "rb");
		free(fn);
		if (st)
      {
			SaveStateStatus[ssel] = 1;
			fclose(st);
		}
      else
			SaveStateStatus[ssel] = 0;
	}

	CurrentState = 0;
}

void ResetExState(void (*PreSave)(void), void (*PostSave)(void))
{
	SPreSave = PreSave;
	SPostSave = PostSave;
	SFEXINDEX = 0;
}

void AddExState(void *v, uint32 s, int type, char *desc)
{
	memset(SFMDATA[SFEXINDEX].desc, 0, sizeof(SFMDATA[SFEXINDEX].desc));
	if (desc)
		strncpy(SFMDATA[SFEXINDEX].desc, desc, sizeof(SFMDATA[SFEXINDEX].desc));
	SFMDATA[SFEXINDEX].v = v;
	SFMDATA[SFEXINDEX].s = s;
	if (type) SFMDATA[SFEXINDEX].s |= RLSB;
	if (SFEXINDEX < 63) SFEXINDEX++;
	SFMDATA[SFEXINDEX].v = 0;	// End marker.
}

void FCEUI_SelectState(int w)
{
	if (w == -1)
      return;

	CurrentState = w;
	FCEU_DispMessage("-select state-");
}

void FCEUI_SaveState(char *fname)
{
	FCEUSS_Save(fname);
}

void FCEUI_LoadState(char *fname)
{
	FCEUSS_Load(fname);
}

void FCEU_DrawSaveStates(uint8 *XBuf)
{
}

