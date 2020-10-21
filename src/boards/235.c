/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
 *  Copyright (C) 2020 negativeExponent
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
#include "../ines.h"

static uint8 *CHRRAM;
static uint32 CHRRAMSIZE;

static uint16 cmdreg;
static uint8 unrom, reg, type, openbus;

static SFORMAT StateRegs[] =
{
	{ &cmdreg,  2 | FCEUSTATE_RLSB, "CREG" },
	{ &unrom,   1, "UNRM" },
	{ &reg,     1, "UNRG" },
	{ &type,    1, "TYPE" },
	{ &openbus, 1, "OPNB" },
	{ 0 }
};

static void Sync(void) {
	if (type && unrom) {
		setprg16(0x8000, 0x80 | (reg & 7));
		setprg16(0xC000, 0x80 | 7);
		setchr8(0);
		setmirror(MI_V);
	} else {
		uint8 bank = ((cmdreg & 0x300) >> 3) | (cmdreg & 0x1F);
		if (cmdreg & 0x400)
			setmirror(MI_0);
		else
			setmirror(((cmdreg >> 13) & 1) ^ 1);
		if (bank >= PRGsize[0] / 32768)
			openbus = 1;
		else if (cmdreg & 0x800) {
			setprg16(0x8000, (bank << 1) | ((cmdreg >> 12) & 1));
			setprg16(0xC000, (bank << 1) | ((cmdreg >> 12) & 1));
		} else
			setprg32(0x8000, bank);
		setchr8(0);
	}
}

static DECLFR(M235Read) {
	if (openbus) {
		openbus = 0;
		return X.DB;
	}
	return CartBR(A);
}

static DECLFW(M235Write) {
	cmdreg = A;
	reg = V;
	Sync();
}

static void M235Close(void) {
	if (CHRRAM)
		FCEU_free(CHRRAM);
	CHRRAM = NULL;
}

static void M235Reset(void) {
	cmdreg = 0;
	unrom = (unrom + type) & 1;
	Sync();
}

static void M235Power(void) {
	SetWriteHandler(0x8000, 0xFFFF, M235Write);
	SetReadHandler(0x8000, 0xFFFF, M235Read);
	cmdreg = 0;
	Sync();
}

static void M235Restore(int version) {
	Sync();
}

void Mapper235_Init(CartInfo *info) {
	info->Reset = M235Reset;
	info->Power = M235Power;
	info->Close = M235Close;
	GameStateRestore = M235Restore;
	AddExState(&StateRegs, ~0, 0, 0);

	/* some nes 2.0 header do can have no chr-ram.
	 * one such cart is 210-in-1 and Contra 4-in-1 (212-in-1,212 Hong Kong,Reset Based)(Unl).nes (0x745A6791)
	 */
	if (CHRsize[0] == 0) {
		CHRRAMSIZE = 8192;
		CHRRAM = (uint8*)FCEU_gmalloc(CHRRAMSIZE);
		SetupCartCHRMapping(0, CHRRAM, CHRRAMSIZE, 1);
		AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
	}

	type = 0;
	/* carts with unrom game, reset-based */
	if ((info->CRC32) == 0x745A6791) /* 210-in-1 and Contra 4-in-1 (212-in-1,212 Hong Kong,Reset Based)(Unl).nes */
		type = 1;
}
