/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *
 *  Copyright (C) 2008 -2020 dragon2snow,loong2snow from www.nesbbs.com
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
 *
 */

#include "mapinc.h"

static uint8 regs[4];
static uint8 hrd_flag;
static void(*Sync)(void);

static SFORMAT StateRegs[] =
{
	{ &hrd_flag, 1, "FLAG" },
	{ regs, 4, "REGS" },
	{ 0 }
};

static void SyncAbG1l(void) {

	if (regs[1] & 0x10)
		setprg32(0x8000,(regs[1] & 0xC0) >> 6);
	else
	{
		setprg16(0x8000, (regs[1] & 0xE0) >> 5);
		setprg16(0xC000, (regs[1] & 0xE0) >> 5);
	}

	/* FIXME: 2020-10-26  */
	switch (regs[2] & 0xC0) {
	case 0xC0: /* Pipe Dream */
		setchr8((regs[0] & 0x03) | (regs[1] & ~0x03));
		break;
	case 0x40: /* Goonies */
		setchr8((regs[0] & 0x01) | (regs[1] & 0x07));
		break;
	default:
		setchr8(regs[1]);
		break;
	}

	setmirror((regs[1] & 0x8) ? 0 : 1);
}

static DECLFW(AbG1lWriteHi) {
	regs[0] = V;
	/* setchr8(((regs[2] & 0xC0) >> 7) << 2 | (regs[0] & 0x03)); */
	Sync();
}

static DECLFW(AbG1lWriteLo) {
	regs[A & 0x03] = V;
	Sync();
}

static DECLFR(AbG1lReadLo) {
	return hrd_flag;
}

static void AbG1lPower(void) {
	hrd_flag = 0;

	regs[0] = 0;
	regs[1] = 0;
	regs[2] = 0;

	Sync();
	SetWriteHandler(0x8000, 0xFFFF, AbG1lWriteHi);
	SetWriteHandler(0x6001, 0x6002, AbG1lWriteLo);
	SetReadHandler(0x6000, 0x7FFF, AbG1lReadLo);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void StateRestore(int version) {
	Sync();
}

static void AbG1lReset(void) {
	hrd_flag++;
	hrd_flag &= 3;
	
	regs[0] = 0;
	regs[1] = 0;
	regs[2] = 0;

	Sync();
}

void AbG1l_Init(CartInfo *info) {

	Sync = SyncAbG1l;

	hrd_flag = 0;

	Sync();

	info->Power = AbG1lPower;
	info->Reset = AbG1lReset;
	AddExState(&StateRegs, ~0, 0, 0);
	GameStateRestore = StateRestore;
}


static void SyncWellNoDG450(void) {

	if (regs[1] & 0x10)
		setprg32(0x8000, (regs[1]) >> 6);
	else
	{
		setprg16(0x8000, (regs[1]) >> 5);
		setprg16(0xC000, (regs[1]) >> 5);
	}

	setchr8(regs[1] & 0x0F);
	setmirror((regs[1] & 0x8) ? 0 : 1);
}

static void WellNoDG450Power(void) {
	hrd_flag = 0;

	regs[0] = 0;
	regs[1] = 0;
	regs[2] = 0;

	Sync();

	SetWriteHandler(0x6001, 0x6002, AbG1lWriteLo);
	SetReadHandler(0x6000, 0x7FFF, AbG1lReadLo);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void WellNoDG450Reset(void) {
	hrd_flag++;
	hrd_flag &= 3;

	regs[0] = 0;
	regs[1] = 0;
	regs[2] = 0;

	Sync();
}

void WellNoDG450_Init(CartInfo *info) {
	Sync = SyncWellNoDG450;
	info->Power = WellNoDG450Power;
	info->Reset = WellNoDG450Reset;
	AddExState(&StateRegs, ~0, 0, 0);
	GameStateRestore = StateRestore;
}
