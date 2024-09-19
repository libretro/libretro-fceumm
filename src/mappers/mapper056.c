/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

/*
 * - Mapper 56 - UNL KS202
 *   FDS Conversion: Super Mario Bros. 3 (Pirate, Alt)
 *   similar to M142 but use WRAM instead? $D000 additional IRQ trigger
 * - fix IRQ counter, noticeable in status bars of both SMB2J(KS7032) and SMB3J(KS202)
 */

 #include "mapinc.h"
 #include "ks202.h"

static uint8 prg[4];
static uint8 chr[8];
static uint8 mirr;

static SFORMAT M056StateRegs[] = {
	{ prg, 8, "PREG" },
	{ chr, 8, "CREG" },
	{ &mirr, 1, "MIRR" },
	{ 0 }
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);

	setprg8(0x8000, (prg[0] & 0x10) | (ks202.reg[1] & 0x0F));
	setprg8(0xA000, (prg[1] & 0x10) | (ks202.reg[2] & 0x0F));
	setprg8(0xC000, (prg[2] & 0x10) | (ks202.reg[3] & 0x0F));
	setprg8(0xE000, (prg[3] & 0x10) | (          ~0 & 0x0F));

	setchr1(0x0000, chr[0]);
	setchr1(0x0400, chr[1]);
	setchr1(0x0800, chr[2]);
	setchr1(0x0C00, chr[3]);
	setchr1(0x1000, chr[4]);
	setchr1(0x1400, chr[5]);
	setchr1(0x1800, chr[6]);
	setchr1(0x1C00, chr[7]);

	setmirror(mirr & 1);
}

static DECLFW(M056Write) {
	switch (A & 0xC00) {
	case 0x000: prg[A & 0x03] = V; break;
	case 0x800: mirr = V; break;
	case 0xC00: chr[A & 0x07] = V; break;
	}
	ks202.reg[ks202.cmd & 0x07] = V;
	Sync();
}

static void M056Reset(void) {
	prg[0] = prg[1] = prg[2] = prg[3] = 0x10;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	chr[4] = chr[5] = chr[6] = chr[7] = 0;
	mirr = 0;
	Sync();
}

static void M056Power(void) {
	prg[0] = prg[1] = prg[2] = prg[3] = 0x10;
	chr[0] = chr[1] = chr[2] = chr[3] = 0;
	chr[4] = chr[5] = chr[6] = chr[7] = 0;
	mirr = 0;
	KS202_Power();
	SetWriteHandler(0xF000, 0xFFFF, M056Write);
}

void Mapper056_Init(CartInfo *info) {
	KS202_Init(info, Sync, 1, 0);
	info->Power = M056Power;
	info->Reset = M056Reset;
	AddExState(&M056StateRegs, ~0, 0, 0);
}
