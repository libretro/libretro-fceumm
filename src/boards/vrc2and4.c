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
#include "vrc2and4.h"

int    VRC24_A0, VRC24_A1; /* What cartridge connector address pins are connected to the chip's A0 and A1 input pins? */
uint8  VRC24_isVRC4;       /* VRC2 or VRC4? VRC2 has no single-screen mirroring, no PRG A14 swap and no IRQ counter */
uint8  VRC24_useRepeatBit; /* VRC4 clones ignore the "repeat" bit in the IRQ Mode register */
uint8  VRC24_prg[2];
uint16 VRC24_chr[8];
uint8  VRC24_mirroring;
uint8  VRC24_misc;
uint8  VRC2_pins;          /* EEPROM interface */
uint8  VRC4_latch;
uint8  VRC4_mode;
uint8  VRC4_count;
signed short int VRC4_cycles;

static uint8 *WRAM;
static uint32 WRAMSize;

int    (*VRC24_GetPRGBank)(int);
int    (*VRC24_GetCHRBank)(int);
void   (*VRC24_Sync)();
DECLFW((*VRC24_ExternalSelect));
DECLFR((*VRC24_WRAMRead));
DECLFW((*VRC24_WRAMWrite));

static SFORMAT VRC24_stateRegs[] ={
	{ VRC24_prg, 2, "PREG" },
	{ &VRC24_chr[0], 2 | FCEUSTATE_RLSB, "CRH0" },
	{ &VRC24_chr[1], 2 | FCEUSTATE_RLSB, "CRH1" },
	{ &VRC24_chr[2], 2 | FCEUSTATE_RLSB, "CRH2" },
	{ &VRC24_chr[3], 2 | FCEUSTATE_RLSB, "CRH3" },
	{ &VRC24_chr[4], 2 | FCEUSTATE_RLSB, "CRH4" },
	{ &VRC24_chr[5], 2 | FCEUSTATE_RLSB, "CRH5" },
	{ &VRC24_chr[6], 2 | FCEUSTATE_RLSB, "CRH6" },
	{ &VRC24_chr[7], 2 | FCEUSTATE_RLSB, "CRH7" },
	{ &VRC24_mirroring, 1, "MIRR" },
	{ &VRC24_misc, 1, "MISC" },
	{ 0 }
};

static SFORMAT VRC2_stateRegs[] ={
	{ &VRC2_pins, 1, "PINS" },
};

static SFORMAT VRC4_stateRegs[] ={
	{ &VRC4_latch, 1, "LATC" },
	{ &VRC4_mode, 1, "MODE" },
	{ &VRC4_count, 1, "COUN" },
	{ &VRC4_cycles, 2 | FCEUSTATE_RLSB, "CYCL" },
	{ 0 }
};

void VRC24_syncPRG(int AND, int OR) {
	setprg8(0x8000, VRC24_GetPRGBank(0) &AND | OR);
	setprg8(0xA000, VRC24_GetPRGBank(1) &AND | OR);
	setprg8(0xC000, VRC24_GetPRGBank(2) &AND | OR);
	setprg8(0xE000, VRC24_GetPRGBank(3) &AND | OR);
}

void VRC24_syncCHR(int AND, int OR) {
	setchr1(0x0000, VRC24_GetCHRBank(0) &AND | OR);
	setchr1(0x0400, VRC24_GetCHRBank(1) &AND | OR);
	setchr1(0x0800, VRC24_GetCHRBank(2) &AND | OR);
	setchr1(0x0C00, VRC24_GetCHRBank(3) &AND | OR);
	setchr1(0x1000, VRC24_GetCHRBank(4) &AND | OR);
	setchr1(0x1400, VRC24_GetCHRBank(5) &AND | OR);
	setchr1(0x1800, VRC24_GetCHRBank(6) &AND | OR);
	setchr1(0x1C00, VRC24_GetCHRBank(7) &AND | OR);
}

void VRC24_syncWRAM(int OR) {
	setprg8r(0x10, 0x6000, OR);
}

int VRC24_getPRGBank(int bank) {
	if (~bank &1 && VRC24_misc &2) bank ^=2;
	return bank &2? (0xFE | bank &1): VRC24_prg[bank &1];
}

int VRC24_getCHRBank(int bank) {
	return VRC24_chr[bank &7];
}

void VRC24_syncMirror() {
	setmirror(VRC24_isVRC4 && VRC24_mirroring &2? (VRC24_mirroring &1? MI_1: MI_0): (VRC24_mirroring &1? MI_H: MI_V));
}

DECLFR(VRC24_wramRead) {
	if (VRC24_WRAMRead)
		VRC24_WRAMRead(A);
	else
	if (WRAMSize)
		return CartBR(((A -0x6000) &(WRAMSize -1)) +0x6000);
	else
		return 0xFF;
}

DECLFW(VRC24_wramWrite) {
	if (WRAMSize)
		CartBW(((A -0x6000) &(WRAMSize -1)) +0x6000, V);
	else
	if (!VRC24_isVRC4) {
		VRC2_pins =V;
		VRC24_Sync();
	}
	
	if (VRC24_WRAMWrite)
		VRC24_WRAMWrite(A, V);
}

DECLFW(VRC24_writeReg) {
	unsigned int index, addr;
	addr =A &0xF000 | (A &VRC24_A0? 1: 0) | (A &VRC24_A1? 2: 0); /* address as the chip sees it */
	switch (addr &0xF000) {
	case 0x8000: case 0xA000:
		VRC24_prg[addr >>13 &1] =V;
		VRC24_Sync();
		break;
	case 0x9000:
		if (!VRC24_isVRC4 || (addr &3) ==0) {
			VRC24_mirroring =V;
			VRC24_Sync();
		} else
		if (VRC24_isVRC4 && (addr &3) ==2) {
			VRC24_misc =V;
			VRC24_Sync();
		} 
		if (VRC24_isVRC4 && (addr &3) ==3 && VRC24_ExternalSelect) {
			VRC24_ExternalSelect(A, V); /* Provide original address */
		}
		break;
	case 0xF000:
		if (VRC24_isVRC4) switch (addr &3) {
			case 0: VRC4_latch =VRC4_latch &0xF0 | V &0x0F; break;
			case 1: VRC4_latch =VRC4_latch &0x0F | V <<4;   break;
			case 2: VRC4_mode =V;
				if (VRC4_mode &0x02) {
					VRC4_count =VRC4_latch;
					VRC4_cycles =341;
				}
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 3: if (VRC24_useRepeatBit) VRC4_mode =VRC4_mode &~0x02 | VRC4_mode <<1 &0x02;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
		}
		break;
	default:
		index =(addr -0xB000) >>11 | addr >>1 &1;
		if (addr &1)
			VRC24_chr[index] =VRC24_chr[index] & 0x0F | V <<4;
		else
			VRC24_chr[index] =VRC24_chr[index] &~0x0F | V &0x0F;
		VRC24_Sync();
		break;
	}
}

void FP_FASTAPASS(1) VRC4_cpuCycle(int a) {
	while (a--) {
		if (VRC4_mode &0x02 && (VRC4_mode &0x04 || (VRC4_cycles -=3) <=0)) {
			if (~VRC4_mode &0x04) VRC4_cycles +=341;
			if (!++VRC4_count) {
				VRC4_count =VRC4_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

void VRC24_power(void) {
	VRC24_prg[0] =0; VRC24_prg[1] =0;
	VRC24_chr[0] =0; VRC24_chr[1] =1; VRC24_chr[2] =2; VRC24_chr[3] =3; VRC24_chr[4] =4; VRC24_chr[5] =5; VRC24_chr[6] =6; VRC24_chr[7] =7;
	VRC24_mirroring =VRC24_misc =VRC2_pins =VRC4_latch =VRC4_mode =VRC4_count =VRC4_cycles =0;
	SetReadHandler(0x6000, 0x7FFF,  VRC24_wramRead);
	SetWriteHandler(0x6000, 0x7FFF, VRC24_wramWrite);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, VRC24_writeReg);
	VRC24_Sync();
}

void VRC24_close(void) {
	if (WRAM) {
		FCEU_gfree(WRAM);
		WRAM =NULL;
	}
}

void VRC24_init(CartInfo *info, void (*_sync)(), int _A0, int _A1, uint8 _isVRC4, uint8 _useRepeatBit, uint8 defaultWRAMSizeKiB) {
	VRC24_Sync =_sync;
	VRC24_A0 =_A0;
	VRC24_A1 =_A1;
	VRC24_isVRC4 =_isVRC4;
	VRC24_useRepeatBit =_useRepeatBit;
	VRC24_GetPRGBank =VRC24_getPRGBank;
	VRC24_GetCHRBank =VRC24_getCHRBank;
	VRC24_WRAMRead =NULL;
	VRC24_WRAMWrite =NULL;
	VRC24_ExternalSelect =NULL;
	info->Power =VRC24_power;
	info->Close =VRC24_close;
	GameStateRestore =_sync;
	AddExState(VRC24_stateRegs, ~0, 0, 0);
	if (VRC24_isVRC4) {
		MapIRQHook =VRC4_cpuCycle;
		AddExState(VRC4_stateRegs, ~0, 0, 0);
	} else {
		AddExState(VRC2_stateRegs, ~0, 0, 0);
	}
	
	WRAMSize =info->iNES2? (info->PRGRamSize +info->PRGRamSaveSize): (defaultWRAMSizeKiB *1024);
	if (WRAMSize) {
		WRAM =(uint8*)FCEU_gmalloc(WRAMSize);
		SetupCartPRGMapping(0x10, WRAM, WRAMSize, 1);
		AddExState(WRAM, WRAMSize, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] =WRAM;
			info->SaveGameLen[0] =WRAMSize;
		}
	}
}
