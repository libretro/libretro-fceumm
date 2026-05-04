/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2001 Aaron Oneal
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

#ifndef __FCEU_TYPES_H
#define __FCEU_TYPES_H

#include <stdint.h>

/* The custom int8/int16/int32/int64/uint8/uint16/uint32/uint64 typedefs
 * that lived here have been replaced throughout the codebase with the
 * C99 stdint.h equivalents (int8_t/uint8_t/etc). This header is kept
 * for the GINLINE / INLINE / FASTAPASS / FP_FASTAPASS macros and the
 * readfunc/writefunc function-pointer typedefs that depend on them. */

#ifdef __GNUC__
	#define GINLINE inline
#elif MSVC | _MSC_VER
	#define GINLINE		/* Can't declare a function INLINE
						 * and global in MSVC.  Bummer.
						 */
#endif

#ifndef INLINE

#if defined(_MSC_VER)
#define INLINE __forceinline
#elif defined(__GNUC__)
#define INLINE __inline__
#elif defined(_MWERKS_)
#define INLINE inline
#else
#define INLINE
#endif
#endif

#ifdef __GNUC__
	#ifdef C80x86
		#define FASTAPASS(x) __attribute__((regparm(x)))
		#define FP_FASTAPASS FASTAPASS
	#else
		#define FASTAPASS(x)
		#define FP_FASTAPASS(x)
	#endif
#elif MSVC
	#define FP_FASTAPASS(x)
	#define FASTAPASS(x) __fastcall
#else
	#define FP_FASTAPASS(x)
	#define FASTAPASS(x)
#endif

typedef void (FP_FASTAPASS(2) *writefunc)(uint32_t A, uint8_t V);
typedef uint8_t (FP_FASTAPASS(1) *readfunc)(uint32_t A);

#endif
