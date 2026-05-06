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

#include <streams/memory_stream.h>

#include "fceu-memory.h"
#include "fceu-types.h"
#include "fceu-endian.h"

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
