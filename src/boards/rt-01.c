/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2016 CaH4e3
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
 *
 * Test Ver. 1.01 Dlya Proverki TV Pristavok (RT-01, by SS aka Snake)
 * A simple board with 16K PRG ROM + 2K CHR ROM with no mapper, hardwired mirroring
 * PRG EPROM has copy protected areas with "weak bits", which is tested at some
 * points of the program. Trying to simalate "weak bits" behaviour
 * 
 */

#include "mapinc.h"

/* Use a local xorshift32 instead of libc rand() so weak-bit reads are
 * deterministic across runs and reproducible in replay/netplay. The
 * sequence still appears "noisy" to the game's protection check (which
 * just samples bits at runtime), but two emulator runs that read the
 * same protected addresses in the same order will see the same bits. */
static uint32_t weakbits_state = 0x13371337;

static INLINE uint8_t weakbits_next(void) {
	uint32_t x = weakbits_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	weakbits_state = x;
	return (uint8_t)x;
}

static DECLFR(UNLRT01Read) {
#if 0
	u16 i, prot_areas[2][2] = {
		{ 0x8E80, 0x8EFF },
		{ 0xFE80, 0xFEFF },
	};
#endif
	if(((A >= 0xCE80) && (A < 0xCF00)) ||
	   ((A >= 0xFE80) && (A < 0xFF00))) {
		return 0xF2 | (weakbits_next() & 0x0D);
	} else
		return CartBR(A);
}

static void UNLRT01Power(void) {
	weakbits_state = 0x13371337;	/* deterministic at every power-on */
	setprg16(0x8000, 0);
	setprg16(0xC000, 0);
	setchr2(0x0000,0);
	setchr2(0x0800,0);
	setchr2(0x1000,0);
	setchr2(0x1800,0);
	SetReadHandler(0x8000, 0xFFFF, UNLRT01Read);
}

void UNLRT01_Init(CartInfo *info) {
	info->Power = UNLRT01Power;
	/* Save weak-bits PRNG state so savestate/rewind/netplay round-trips
	 * stay deterministic. */
	AddExState(&weakbits_state, 4 | FCEUSTATE_RLSB, 0, "WBKS");
}
