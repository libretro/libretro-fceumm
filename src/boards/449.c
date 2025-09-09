/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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

uint8 submapper;
uint8 pad;
uint8 padSelect;

static DECLFR(readPad_submapper0) {
	return CartBR(A &~0xF | pad &0xF);
}

static DECLFR(readPad_submapper1) {
	return pad;
}

static DECLFR(readPad_submapper2) {
	return CartBR(A &~0x3 | pad &0x3);
}

static void sync () {
	int prg = Latch_address >>2 &0x1F | Latch_address >>3 &0x20 | Latch_address >>4 &0x40;
	if (Latch_address &0x080) {
		if (Latch_address &0x001)
			setprg32(0x8000, prg >>1);
		else {
			setprg16(0x8000, prg);
			setprg16(0xC000, prg);
		}
		setmirror(Latch_data &0x10? MI_1: MI_0);
	} else {
		setprg16(0x8000, prg);
		setprg16(0xC000, prg | 7);
	}
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], submapper == 0 && Latch_address &0x80? 0: 1);
	setchr8(Latch_data);
	setmirror(Latch_address &0x002? MI_H: MI_V);
	SetReadHandler(0x8000, 0xFFFF, submapper == 0 && Latch_address &0x200? readPad_submapper0: submapper == 2 && padSelect &1? readPad_submapper2: CartBR);
}

static DECLFW(writePad_submapper2) {
	padSelect = V;
	sync();
}

static void power () {
	pad = padSelect = 0;
	Latch_power();
	if (submapper == 1)
		SetReadHandler(0x5000, 0x5FFF, readPad_submapper1);
	else
	if (submapper == 2)
		SetWriteHandler(0x6000, 0x7FFF, writePad_submapper2);
}

static void reset () {
	pad++;
	padSelect = 0;
	Latch_address = 0;
	sync();
}

void Mapper449_Init (CartInfo *info) {
	submapper = info->submapper;
	Latch_init(info, sync, 0x8000, 0xFFFF, NULL);
	info->Power = power;
	info->Reset = reset;
	AddExState(&pad, 1, 0, "DIPS");
	if (submapper == 2) AddExState(&padSelect, 1, 0, "DIPE");
}
