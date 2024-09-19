/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2009 CaH4e3
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
 * VRC-6
 *
 */

#include "mapinc.h"
#include "vrc6.h"
#include "vrcirq.h"
#include "vrc6sound.h"

static uint32 vrc6_A0;
static uint32 vrc6_A1;

VRC6 vrc6;

void (*VRC6_pwrap)(uint16 A, uint16 V);
void (*VRC6_cwrap)(uint16 A, uint16 V);

static SFORMAT StateRegs[] =
{
	{ vrc6.prg, 2, "PRG" },
	{ vrc6.chr, 8, "CHR" },
	{ &vrc6.mirr, 1, "MIRR" },

	{ 0 }
};

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V & 0x3F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V & 0xFF);
}

void VRC6_FixMIR(void) {
	switch (vrc6.mirr & 3) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

void VRC6_FixPRG(void) {
	VRC6_pwrap(0x8000, (vrc6.prg[0] << 1) | 0x00);
	VRC6_pwrap(0xa000, (vrc6.prg[0] << 1) | 0x01);
	VRC6_pwrap(0xc000, vrc6.prg[1]);
	VRC6_pwrap(0xe000, ~0);
}

void VRC6_FixCHR(void) {
	int i;

	for (i = 0; i < 8; i++) {
		VRC6_cwrap(i << 10, vrc6.chr[i]);
	}
}

DECLFW(VRC6_Write) {
	int index;

	A = (A & 0xF000) | ((A & vrc6_A1) ? 0x02 : 0x00) | ((A & vrc6_A0) ? 0x01 : 0x00);
	switch (A & 0xF000) {
	case 0x8000:
		vrc6.prg[0] = V;
		VRC6_pwrap(0x8000, (V << 1) | 0x00);
		VRC6_pwrap(0xA000, (V << 1) | 0x01);
		break;
	case 0x9000:
		VRC6Sound_Write(A, V);
		break;
	case 0xA000:
		VRC6Sound_Write(A, V);
		break;
	case 0xB000:
		VRC6Sound_Write(A, V);
		if ((A & 0x03) == 0x03) {
			vrc6.mirr = (V >> 2) & 3;
			VRC6_FixMIR();
		}
		break;
	case 0xC000:
		vrc6.prg[1] = V;
		VRC6_pwrap(0xC000, V);
		break;
	case 0xD000:
	case 0xE000:
		index = ((A - 0xD000) >> 10) | (A & 0x03);
		vrc6.chr[index] = V;
		VRC6_cwrap(index << 10, V);
		break;
	case 0xF000:
		index = A & 0x03;
		switch (index) {
		case 0x00: VRCIRQ_Latch(V); break;
		case 0x01: VRCIRQ_Control(V); break;
		case 0x02: VRCIRQ_Acknowledge(); break;
		}
	}
}

void VRC6_IRQCPUHook(int a) {
	VRCIRQ_CPUHook(a);
}

void VRC6_Reset(void) {
	vrc6.prg[0] = 0;
	vrc6.prg[1] = 1;

	vrc6.chr[0] = 0;
	vrc6.chr[1] = 1;
	vrc6.chr[2] = 2;
	vrc6.chr[3] = 3;
	vrc6.chr[4] = 4;
	vrc6.chr[5] = 5;
	vrc6.chr[6] = 6;
	vrc6.chr[7] = 7;

	vrc6.mirr = 0;

	VRC6_FixPRG();
	VRC6_FixCHR();
	VRC6_FixMIR();
}

void VRC6_Power(void) {
	VRC6_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC6_Write);

	if (WRAMSIZE) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	}
}

void VRC6_Close(void) {
}

void VRC6_Restore(int version) {
	VRC6_FixPRG();
	VRC6_FixCHR();
	VRC6_FixMIR();
}

void VRC6_Init(CartInfo *info, uint32 A0, uint32 A1, int wram) {
	VRC6_pwrap = GENPWRAP;
	VRC6_cwrap = GENCWRAP;

	vrc6_A0 = A0;
	vrc6_A1 = A1;

	if (wram) {
		if (info->iNES2) {
			WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
		} else if (info->mapper == 26) {
			WRAMSIZE = 8192;
		}
		if (WRAMSIZE) {
			WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
			SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
			AddExState(WRAM, WRAMSIZE, 0, "WRAM");
			if (info->battery) {
				info->SaveGame[0] = WRAM;
				info->SaveGameLen[0] = WRAMSIZE;
			}
		}
	}
	AddExState(StateRegs, ~0, 0, NULL);

	info->Power = VRC6_Power;
	info->Close = VRC6_Close;
	GameStateRestore = VRC6_Restore;

	VRCIRQ_Init(TRUE);
	MapIRQHook = VRC6_IRQCPUHook;
	AddExState(&VRCIRQ_StateRegs, ~0, 0, 0);

	VRC6Sound_ESI();
	VRC6Sound_AddStateInfo();
}
