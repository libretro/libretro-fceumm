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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fceu-types.h"
#include "video.h"
#include "fceu.h"
#include "general.h"
#include "fceu-memory.h"
#include "crc32.h"
#include "state.h"
#include "palette.h"
#include "nsf.h"
#include "input.h"
#include "vsuni.h"

uint8 *XBuf = NULL;
uint8 *XDBuf = NULL;
int show_crosshair = 0;

void FCEU_KillVirtualVideo(void)
{
	if (XBuf)
		free(XBuf);
   XBuf = 0;
   if (XDBuf)
		free(XDBuf);
   XDBuf = 0;
}

int FCEU_InitVirtualVideo(void)
{
   /* 256 bytes per scanline, * 240 scanline maximum, +8 for alignment, */
   if (!XBuf)
      XBuf = (uint8*)(FCEU_malloc(256 * (256 + extrascanlines + 8)));
   if (!XDBuf)
      XDBuf = (uint8*)(FCEU_malloc(256 * (256 + extrascanlines + 8)));

   if (!XBuf || !XDBuf)
      return 0;

   memset(XBuf, 128, 256 * (256 + extrascanlines + 8));
   memset(XDBuf, 128, 256 * (256 + extrascanlines + 8));
   return 1;
}

void FCEU_DispMessage(enum retro_log_level level, unsigned duration, const char *format, ...)
{
   static char msg[512] = {0};
   va_list ap;

   if (!format || (*format == '\0'))
      return;

   va_start(ap, format);
   vsprintf(msg, format, ap);
   va_end(ap);

   FCEUD_DispMessage(level, duration, msg);
}
