/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2012 CaH4e3
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

#include "mapinc.h"
#include "vrcirq.h"
#include "vrc7sound.h"
#include "vrc7.h"

static uint32 vrc7_A0;
static uint32 vrc7_A1;

VRC7 vrc7;

void (*VRC7_pwrap)(uint16 A, uint16 V);
void (*VRC7_cwrap)(uint16 A, uint16 V);
void (*VRC7_mwrap)(uint8 V);

static SFORMAT StateRegs[] =
{
	{ vrc7.prg, 3, "PREG" },
	{ vrc7.chr, 8, "CREG" },
	{ &vrc7.mirr, 1, "MIRR" },

	{ 0 }
};

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V & 0x3F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V & 0xFF);
}

static void GENMWRAP(uint8 V) {
	vrc7.mirr = V;

	switch (vrc7.mirr & 3) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

void VRC7_FixPRG(void) {
	setprg8r(0x10, 0x6000, 0);
	VRC7_pwrap(0x8000, vrc7.prg[0]);
	VRC7_pwrap(0xa000, vrc7.prg[1]);
	VRC7_pwrap(0xc000, vrc7.prg[2]);
	VRC7_pwrap(0xe000, ~0);
}

void VRC7_FixCHR(void) {
	int i;
	for (i = 0; i < 8; i++) {
		VRC7_cwrap(i << 10, vrc7.chr[i]);
	}

	if (VRC7_mwrap) {
		VRC7_mwrap(vrc7.mirr);
	}
}

DECLFW(VRC7_Write) {
	int index;
	switch (A & 0xF000) {
	case 0x8000:
	case 0x9000:
		index = ((A >> 11) & 0x02) | ((A & vrc7_A0) ? 0x01 : 0x00);
		switch (index) {
		case 0x00:
		case 0x01:
		case 0x02:
			vrc7.prg[index] = V;
			VRC7_FixPRG();
			break;
		default:
			VRC7Sound_Write(A, V);
			break;
		}
		break;

	case 0xA000:
	case 0xB000:
	case 0xC000:
	case 0xD000:
		index = ((A - 0xA000) >> 11) | ((A & vrc7_A0) ? 0x01 : 0x00);
		vrc7.chr[index] = V;
		VRC7_FixCHR();
		break;

	case 0xE000:
	case 0xF000:
		index = ((A >> 11) & 0x02) | ((A & vrc7_A0) ? 0x01 : 0x00);
		switch (index) {
		case 0x00: if (VRC7_mwrap) VRC7_mwrap(V); break;
		case 0x01: VRCIRQ_Latch(V); break;
		case 0x02: VRCIRQ_Control(V); break;
		case 0x03: VRCIRQ_Acknowledge(); break;
		}
		break;
	}
}

void VRC7_Power(void) {
	vrc7.prg[0] = 0;
	vrc7.prg[1] = 1;
	vrc7.prg[2] = ~1;

	vrc7.chr[0] = 0;
	vrc7.chr[1] = 1;
	vrc7.chr[2] = 2;
	vrc7.chr[3] = 3;
	vrc7.chr[4] = 4;
	vrc7.chr[5] = 5;
	vrc7.chr[6] = 6;
	vrc7.chr[7] = 7;

	vrc7.mirr = 0;

	VRC7_FixPRG();
	VRC7_FixCHR();

	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC7_Write);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
}

void VRC7_Close(void) {
}

static void StateRestore(int version) {
	VRC7_FixPRG();
	VRC7_FixCHR();
}

void VRC7_Init(CartInfo *info, uint32 A0, uint32 A1) {
	VRC7_pwrap = GENPWRAP;
	VRC7_cwrap = GENCWRAP;
	VRC7_mwrap = GENMWRAP;

	vrc7_A0 = A0;
	vrc7_A1 = A1;

	WRAMSIZE = 8192;
	WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
	SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
	AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	if (info->battery) {
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = WRAMSIZE;
	}
	AddExState(StateRegs, ~0, 0, NULL);

	info->Power = VRC7_Power;
	info->Close = VRC7_Close;
	GameStateRestore = StateRestore;

	VRCIRQ_Init(TRUE);
	MapIRQHook = VRCIRQ_CPUHook;
	AddExState(&VRCIRQ_StateRegs, ~0, 0, 0);

	VRC7Sound_ESI();
	VRC7Sound_AddStateInfo();
}
