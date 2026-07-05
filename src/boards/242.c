/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2026 NewRisingSun
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

#include "mapinc.h"
#include "asic_latch.h"
#include "cartram.h"

static uint8_t pad;
static uint8_t padMask; /* Mask determining the number of address-based pad positions to cycle through */
static uint8_t submapper;
static uint8_t twoChips;

static void sync () {
	int prg = Latch_address >>2 &0x1F;
	if (twoChips && !(Latch_address &0x600)) prg = prg &0x07 | 0x20; /* Second chip on two-chip cartridges */
	if (Latch_address &0x080) { /* NROM modes */
		if (Latch_address &0x001)
			setprg32(0x8000, prg >>1);
		else {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);
		}
	} else { /* UNROM-like modes */
		if (Latch_address &0x080) prg &= ~1; /* PRG A14 modification still applies for the switchable bank */
		setprg16(0x8000, prg);
		if (Latch_address &0x200) /* Fixed bank is last bank */
			setprg16(0xC000, prg | 7);
		else                      /* Fixed bank is first bank */
			setprg16(0xC000, prg &~7);
	}
	if (PRGsize[0x10]) setprg8r(0x10, 0x6000, 0); /* PRG-RAM */
	if (Latch_address &0x080 && submapper >0) /* CHR-RAM write protection on multicarts only */
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 0);
	else
		SetupCartCHRMapping(0, CHRptr[0], 0x2000, 1);
	setchr8(0);
	setmirror(Latch_address &0x02? MI_H: MI_V);
}

static DECLFR (interceptPRGRead) {
	if ((Latch_address &0x1FF) == 0x0100 || twoChips && (Latch_address &0x3FF) == 0x00C0) {
		if (padMask == 0x01) /* When there are two menu choices only, switch between open bus and chip enable. */
			return pad &0x01? X.DB: CartBR(A);
		else
			return CartBR(A &~padMask | pad &padMask);
	} else
		return CartBR(A);
}

static void power () {
	pad = 0;
	Latch_power();
	if (submapper >0) SetReadHandler(0x8000, 0xFFFF, interceptPRGRead);
}

static void reset () {
	pad++;
	Latch_clear();
}

void Mapper242_Init (CartInfo *info) {
	submapper = info->submapper;
	twoChips = info->PRGRomSize &0x20000 && info->PRGRomSize >0x20000;
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	if (info->iNES2 && (info->PRGRamSize || info->PRGRamSaveSize) || info->battery) WRAM_init(info, 8);
	info->Power = power;
	info->Reset = reset;
	if (submapper >0) AddExState(&pad, 1, 0, "DIPS"); /* Only multicarts can have multiple menus */
	switch (submapper) {
		default:
			padMask = 0x0F; /* 16 menu choices */ 
			break;
		case 2:
			padMask = 0x1F; /* 32 menu choices */ 
			break;
		case 3:
			padMask = 0x01; /* Two choices between open bus and chip enable */
			break;
	}
}
