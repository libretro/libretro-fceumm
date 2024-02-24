/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007 CaH4e3
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
 *
 * VRC-2/VRC-4 Konami
 * VRC-4 Pirate
 *
 * VRC2
 * Nickname	PCB		A0	A1	Registers					iNES mapper	submapper
 * VRC2a	351618	A1	A0	$x000, $x002, $x001, $x003	22			0
 * VRC2b	many†	A0	A1	$x000, $x001, $x002, $x003	23			3
 * VRC2c	351948	A1	A0	$x000, $x002, $x001, $x003	25			3
 * VRC4
 * Nickname	PCB		A0	A1	Registers					iNES mapper	submapper
 * VRC4a	352398	A1	A2	$x000, $x002, $x004, $x006	21			1
 * VRC4b	351406	A1	A0	$x000, $x002, $x001, $x003	25			1
 * VRC4c	352889	A6	A7	$x000, $x040, $x080, $x0C0	21			2
 * VRC4d	352400	A3	A2	$x000, $x008, $x004, $x00C	25			2
 * VRC4e	352396	A2	A3	$x000, $x004, $x008, $x00C	23			2
 * VRC4f	-		A0	A1	$x000, $x001, $x002, $x003	23			1
 *
 */

#include "mapinc.h"
#include "vrc2and4.h"
#include "vrcirq.h"

uint8 vrc2and4_VRC4;
uint32 vrc2and4_A0;
uint32 vrc2and4_A1;

void (*VRC24_FixPRG)(void);
void (*VRC24_FixCHR)(void);
void (*VRC24_FixMIR)(void);

void (*VRC24_pwrap)(uint16 A, uint16 V);
void (*VRC24_cwrap)(uint16 A, uint16 V);
void (*VRC24_miscWrite)(uint16 A, uint8 V);

VRC24 vrc24;

static SFORMAT StateRegs[] = {
	{ vrc24.prg, 2, "PREG" },
	{ vrc24.chr, 16, "CREG" },
	{ &vrc24.cmd, 1, "CMDR" },
	{ &vrc24.mirr, 1, "MIRR" },
	{ &vrc24.latch, 1, "LATC" },

	{ 0 }
};

static void GENFIXPRG(void) {
	if (vrc24.cmd & 2) {
		VRC24_pwrap(0x8000, ~1);
		VRC24_pwrap(0xC000, vrc24.prg[0]);
	} else {
		VRC24_pwrap(0x8000, vrc24.prg[0]);
		VRC24_pwrap(0xC000, ~1);
	}
	VRC24_pwrap(0xA000, vrc24.prg[1]);
	VRC24_pwrap(0xE000, ~0);
}

static void GENFIXCHR(void) {
	VRC24_cwrap(0x0000, vrc24.chr[0]);
	VRC24_cwrap(0x0400, vrc24.chr[1]);
	VRC24_cwrap(0x0800, vrc24.chr[2]);
	VRC24_cwrap(0x0C00, vrc24.chr[3]);
	VRC24_cwrap(0x1000, vrc24.chr[4]);
	VRC24_cwrap(0x1400, vrc24.chr[5]);
	VRC24_cwrap(0x1800, vrc24.chr[6]);
	VRC24_cwrap(0x1C00, vrc24.chr[7]);
}

static void GENFIXMIR(void) {
	switch (vrc24.mirr & (vrc2and4_VRC4 ? 0x03 : 0x01)) {
	case 0: setmirror(MI_V); break;
	case 1: setmirror(MI_H); break;
	case 2: setmirror(MI_0); break;
	case 3: setmirror(MI_1); break;
	}
}

static DECLFR(VRC24WRAMRead) {
	A = 0x6000 | (A & (WRAMSIZE - 1));
	return CartBR(A);
}

static DECLFW(VRC24WRAMWrite) {
	A = 0x6000 | (A & (WRAMSIZE - 1));
	CartBW(A, V);
}

static DECLFR(VRC24LatchRead) {
	return (cpu.openbus & ~0x01) | (vrc24.latch & 0x01);
}

static DECLFW(VRC24LatchWrite) {
	vrc24.latch = V;
	VRC24_FixPRG();
	VRC24_FixCHR();
}

DECLFW(VRC24_Write) {
	uint8 index;

	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		vrc24.prg[(A >> 13) & 0x01] = V & 0x1F;
		VRC24_FixPRG();
		break;

	case 0x9000:
		index = ((A & vrc2and4_A1) ? 0x02 : 0x00) | ((A & vrc2and4_A0) ? 0x01 : 0x00);
		switch (index & (vrc2and4_VRC4 ? 0x03 : 0x00)) {
		case 0:
		case 1:
			if (V != 0xFF) {
				vrc24.mirr = V;
				VRC24_FixMIR();
			}
			break;
		case 2:
			vrc24.cmd = V;
			VRC24_FixPRG();
			break;
		case 3:
			if (VRC24_miscWrite) {
				VRC24_miscWrite(A, V);
			}
			break;
		}
		break;

	case 0xF000:
		index = ((A & vrc2and4_A1) ? 0x02 : 0x00) | ((A & vrc2and4_A0) ? 0x01 : 0x00);
		switch (index) {
		case 0x00: VRCIRQ_LatchNibble(V, 0); break;
		case 0x01: VRCIRQ_LatchNibble(V, 1); break;
		case 0x02: VRCIRQ_Control(V); break;
		case 0x03: VRCIRQ_Acknowledge(); break;
		}
		break;

	default:
		index = (((A - 0xB000) >> 11) & 0x06) | ((A & vrc2and4_A1) ? 0x01 : 0x00);
		if (A & vrc2and4_A0) {
			/* m25 can be 512K, rest are 256K or less */
			vrc24.chr[index] = (vrc24.chr[index] & 0x000F) | (V << 4);
		} else {
			vrc24.chr[index] = (vrc24.chr[index] & 0x0FF0) | (V & 0x0F);
		}
		VRC24_cwrap(index << 10, vrc24.chr[index]);
		break;
	}
}

static void GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V & 0x1F);
}

static void GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V & 0x1FF);
}

void VRC24_IRQCPUHook(int a) {
	VRCIRQ_CPUHook(a);
}

void VRC24_Reset(void) {
	vrc24.prg[0] = 0;
	vrc24.prg[1] = 1;

	vrc24.chr[0] = 0;
	vrc24.chr[1] = 1;
	vrc24.chr[2] = 2;
	vrc24.chr[3] = 3;
	vrc24.chr[4] = 4;
	vrc24.chr[5] = 5;
	vrc24.chr[6] = 6;
	vrc24.chr[7] = 7;

	vrc24.cmd = vrc24.mirr = 0;

	VRC24_FixPRG();
	VRC24_FixCHR();
}

void VRC24_Power(void) {
	VRC24_Reset();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC24_Write);

	if (WRAMSIZE) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, VRC24WRAMRead);
		SetWriteHandler(0x6000, 0x7FFF, VRC24WRAMWrite);
		FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
	} else if (!vrc2and4_VRC4) {
		/* microwire interface */
		SetReadHandler(0x6000, 0x6FFF, VRC24LatchRead);
		SetWriteHandler(0x6000, 0x6FFF, VRC24LatchWrite);
	}

	if (UNIFchrrama) {
		setchr8(0);
	}
}

static void StateRestore(int version) {
	VRC24_FixPRG();
	VRC24_FixCHR();
	VRC24_FixMIR();
}

void VRC24_Close(void) {
}

void VRC24_Init(CartInfo *info, uint8 vrc4, uint32 A0, uint32 A1, int wram, int irqRepeated) {
	VRC24_FixPRG = GENFIXPRG;
	VRC24_FixCHR = GENFIXCHR;
	VRC24_FixMIR = GENFIXMIR;

	VRC24_pwrap = GENPWRAP;
	VRC24_cwrap = GENCWRAP;
	VRC24_miscWrite = NULL;

	vrc2and4_A0 = A0;
	vrc2and4_A1 = A1;
	vrc2and4_VRC4 = vrc4;

	if (wram) {
		if (info->iNES2) {
			WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
		} else {
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

	info->Power = VRC24_Power;
	info->Close = VRC24_Close;
	GameStateRestore = StateRestore;

	VRCIRQ_Init(irqRepeated);
	MapIRQHook = VRCIRQ_CPUHook;
	AddExState(&VRCIRQ_StateRegs, ~0, 0, 0);
}
