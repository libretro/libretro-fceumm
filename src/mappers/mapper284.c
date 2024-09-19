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

/* FIXME:
 * The extender nametable mirroring only works in NEWPPU.
*/

#include "mapinc.h"
#include "dripsound.h"

static uint8 extattrib[2][1024];
static uint8 jumper;
static uint8 control;
static uint8 prg;
static uint8 chr[4];
static uint8 IRQa;
static uint8 IRQLatch;
static int16 IRQCount;
static uint16 lastAddr;

static uint8 DRIPHack = FALSE;

static SFORMAT StateRegs[] = {
	{ &jumper,   1, "JUMP" },
	{ &control,  1, "CTRL" },
	{ &prg,      1, "PREG" },
	{ chr,       4, "CREG" },
	{ &IRQa,     1, "IRQA" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQCount, 2, "IRQC" },
	{ &lastAddr, 2, "LADD" },
	{ 0 }
};

static uint8 Sync(void) {
	if (control & 0x08) {
		setprg8r(0x10, 0x6000, 0);
	}

	setprg16(0x8000, prg);
	setprg16(0xC000, 0x0F);

	setchr2(0x0000, chr[0]);
	setchr2(0x0800, chr[1]);
	setchr2(0x1000, chr[2]);
	setchr2(0x1800, chr[3]);

	switch (control & 0x03) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFW(M284Write) {
	switch (A & 0xE000) {
	case 0x8000:
	case 0xA000:
		switch (A & 0x0F) {
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
			DRIPSound_Write(A, V);
			break;
		case 0x8:
			IRQLatch = V;
			break;
		case 0x9:
			IRQCount = ((V & 0x7F) << 8) | IRQLatch;
			IRQa = V & 0x80;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 0xA:
			control = V & 0x0F;
			Sync();
			break;
		case 0xB:
			prg = V & 0x0F;
			Sync();
			break;
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			chr[A & 0x03] = V & 0x0F;
			Sync();
			break;
		}
		break;
	case 0xC000:
	case 0xE000:
		extattrib[(A & 0x400) >> 10][A & 0x3FF] = V & 0x03;
		break;
	}
}

static DECLFR(M284Read) {
	switch (A & 0xF800) {
	case 0x4800:
		return (jumper | 'd');
	case 0x5000:
	case 0x5800:
		return DRIPSound_Read(A);
	}
	return CartBR(A);
}

#ifdef NEWPPU
uint8 M284PPURead(uint32 A) {
	if ((A > 0x2000) && (A < 0x3F00)) {
		if (control & 0x04) {
			if ((A & 0x3FF) < 0x3C0) {
				lastAddr = A & 0x3FF;
			} else {
				const uint8 ext_attrib[4] = { 0x00, 0x55, 0xAA, 0xFF };
				uint8 bank = 0;

				A &= 0x0FFF;

				switch (iNESCart.mirror) {
				default:
				case MI_0: bank = 0; break;
				case MI_1: bank = 1; break;
				case MI_V: bank = (A & 0x800) ? 1 : 0; break;
				case MI_H: bank = (A & 0x400) ? 1 : 0; break;
				}
				return (ext_attrib[(extattrib[bank][lastAddr & 0x3FF]) & 0x03]);
			}
		}
	}
	return FFCEUX_PPURead_Default(A);
}
#endif

static void M284Reset(void) {
	control = 0;
	prg = 0;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	memset(&extattrib[0][0], 0x00, sizeof(extattrib));
	jumper = !jumper ? 0x80 : 0;

	Sync();
}

static void M284Power(void) {
	control = 0;
	prg = 0;
	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	memset(&extattrib[0][0], 0x00, sizeof(extattrib));

	jumper = 0;

	Sync();

	SetReadHandler(0x4800, 0x5FFF, M284Read);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x8000, 0xFFFF, M284Write);

#ifdef NEWPPU
	FFCEUX_PPURead = M284PPURead;
#endif
}

static void M284CPUIRQHook(int a) {
	if (IRQa) {
		IRQCount -= a;
		if (IRQCount <= 0) {
			IRQa = FALSE;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void StateRestore(int version) {
	Sync();
}

void Mapper284_Init(CartInfo *info) {
	info->Power = M284Power;
	MapIRQHook = M284CPUIRQHook;

	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
	AddExState(extattrib[0], 1024, 0, "ATT0");
	AddExState(extattrib[1], 1024, 0, "ATT1");

	WRAMSIZE = 8192;
	WRAM = FCEU_malloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, TRUE);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");

	DRIPSound_ESI();
	DRIPSound_AddStateInfo();

	DRIPHack = TRUE;

	FCEU_printf(" DRIPGAME Mapper warning: Set video to use NEWPPU for game to work properly.\n");
}
