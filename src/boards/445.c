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
#include "asic_mmc3.h"
#include "asic_vrc2and4.h"

static uint8 reg[4];
static uint8 dip;

static SFORMAT stateRegs[] = {
	{ reg,  4, "REGS" },
	{ &dip, 1, "DIPS" },
	{ 0 }
};

static void sync () {
	int prgAND = 0x7F >>(reg[2] &7) &0x1F; /* 5003.0-2: PRG size (1 MiB->8 KiB, although 1 MiB and 512 KiB are not connected and so function as 256 KiB) and mode (>=64 KiB: MMC3, <64 KiB: NROM-256/128/64) */
	int chrAND = 0x3FF >>(reg[2] >>3 &7) &0xFF; /* 5003.3-5: CHR size (1 MiB->8 KiB, though 1 MiB and 512 KiB are not reachable with inner bank registers and so function as 256 KiB) and mode (>=64 KiB: MMC3, <64 KiB: (C)NROM-256/128/64) */
	int prgOR  = reg[0] &~prgAND;
	int chrOR  = reg[1] <<3 &~chrAND;

	if (prgAND &0x04) {
		if (reg[3] &0x10)
			VRC24_syncPRG(prgAND, prgOR);
		else
			MMC3_syncPRG(prgAND, prgOR);
	} else {
		setprg8(0x8000, 0 &prgAND | prgOR);
		setprg8(0xA000, 1 &prgAND | prgOR);
		setprg8(0xC000, 2 &prgAND | prgOR);
		setprg8(0xE000, 3 &prgAND | prgOR);
	}
	if (chrAND &0x20) {
		if (reg[3] &0x10)
			VRC24_syncCHR(chrAND, chrOR);
		else
			MMC3_syncCHR(chrAND, chrOR);
	} else {
		chrAND >>= 3;
		chrOR >>= 3;
		setchr8(Latch_data &chrAND | chrOR);
	}
	if (reg[3] &0x10)
		VRC24_syncMirror();
	else
		MMC3_syncMirror();
	SetReadHandler(0x8000, 0xFFFF, reg[0] &0xC0 && (reg[0] &0xC0) == dip? NULL: CartBR);
}

static void applyMode (uint8 clear) {
	if ((reg[2] >>3 &7) >= 5)
		Latch_activate(clear, sync, 0x8000, 0xFFFF, NULL);
	else
	if (reg[3] &0x10) {
		if (reg[3] &0x01)
			VRC4_activate(clear, sync, 0x0A, 0x05, 1, NULL, NULL, NULL, NULL, NULL);
		else
			VRC4_activate(clear, sync, 0x05, 0x0A, 1, NULL, NULL, NULL, NULL, NULL);
	} else
		MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, NULL);
}

static void Mapper445_restore (int version) {
	applyMode(0);
	sync();
}

static DECLFW (writeReg) {
	if (~reg[3] &0x20) {
		reg[A &3] = V;
		if ((A &3) == 3) applyMode(1);
		sync();
	}
}

static void Mapper445_power(void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip =0;
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	applyMode(1);
}

static void Mapper445_reset (void) {
	reg[0] = reg[1] = reg[2] = reg[3] = 0;
	dip += 0x40;
	applyMode(1);
}

void Mapper445_Init (CartInfo *info) {
	Latch_addExState();
	MMC3_addExState();
	VRC24_addExState();
	info->Reset = Mapper445_reset;
	info->Power = Mapper445_power;
	GameStateRestore = Mapper445_restore;
	AddExState(stateRegs, ~0, 0, 0);
}

