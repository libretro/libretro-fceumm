/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2024 NewRisingSun
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

uint8	MMC3_type;
uint8	MMC3_index;
uint8	MMC3_reg[16];
uint8	MMC3_regMask;
uint8	MMC3_mirroring;
uint8	MMC3_wramEnable;
uint8	MMC3_reloadValue;
uint8	MMC3_reloadRequest;
uint8	MMC3_irqEnable;
uint8	MMC3_counter;

static SFORMAT MMC3_stateRegs[] ={
	{ MMC3_reg, 8, "REGS" },
	{ &MMC3_index, 1, "CMD" },
	{ &MMC3_mirroring, 1, "A000" },
	{ &MMC3_wramEnable, 1, "A001" },
	{ &MMC3_reloadRequest, 1, "IRQR" },
	{ &MMC3_counter, 1, "IRQC" },
	{ &MMC3_reloadValue, 1, "IRQL" },
	{ &MMC3_irqEnable, 1, "IRQA" },
	{ 0 }
};

void	(*MMC3_Sync)();
uint16	(*MMC3_GetPRGBank)(uint8 bank);
uint16	(*MMC3_GetCHRBank)(uint8 bank);
uint8	FP_FASTAPASS(1) (*MMC3_ReadWRAM) (uint32 A);
void	FP_FASTAPASS(2) (*MMC3_WriteWRAM)(uint32 A, uint8 V);

void	MMC3_syncWRAM () {
	setprg8r(0x10, 0x6000, 0);
	SetReadHandler (0x6000, 0x7FFF, MMC3_wramEnable &0x80? MMC3_ReadWRAM: NULL);
	SetWriteHandler(0x6000, 0x7FFF, MMC3_wramEnable &0x80 && ~MMC3_wramEnable &0x40? MMC3_WriteWRAM: NULL);
}

uint16	MMC3_getPRGBank (uint8 bank) {
	if (MMC3_index &0x40 && ~bank &1) bank ^=2;
	return bank &2? 0xFE | bank &1: MMC3_reg[6 | bank &1];
}

uint16	MMC3_getCHRBank (uint8 bank) {
	if (MMC3_index &0x80) bank ^=4;
	return bank &4? reg[bank -2]: reg[bank >>1] &~1 | bank &1;
}

void	MMC3_syncPRG (int AND, int OR) {
	int bank;
	for (bank =0; bank <4; bank++) setprg8(0x8000 | bank <<13, MMC3_GetPRGBank(bank) &AND |OR);
}

void	MMC3_syncCHR (int AND, int OR) {
	int bank;
	for (bank =0; bank <4; bank++) setchr1(bank <<10, MMC3_GetCHRBank(bank) &AND |OR);
}

void	MMC3_syncMirror () {
	setmirror(MMC3_mirroring &1? MI_H: MI_V);
}

void	MMC3_clockCounter () {
	uint8 prevCounter =MMC3_counter;
	MMC3_counter =MMC3_reloadRequest || !MMC3_counter? MMC3_reloadValue: --MMC3_counter;
	if ((prevCounter || MMC3_type ==MMC3_TYPE_SHARP || MMC3_reloadRequest) && !MMC3_counter && MMC3_enableIRQ) X6502_IRQBegin(FCEU_IQEXT);
	MMC3_reloadRequest =0;
}

DECLFW(MMC3_write) {
	switch(A &0xE001) {
		case 0x8000:	MMC3_index =V; break;
		case 0x8001:	MMC3_reg[MMC3_index &MMC3_regMask] =V; break;
		case 0xA000:	MMC3_mirroring =V; break;
		case 0xA001:	MMC3_wramEnable =V; break;
		case 0xC000:	MMC3_reloadValue =V; break;
		case 0xC001:	MMC3_reloadRequest =1; MMC3_counter =0; break;
		case 0xE000:	X6502_IRQEnd(FCEU_IQEXT); /* Fall-through */
		case 0xE001:	MMC3_irqEnable =A &1; break;
	}
	if (A <0xC000) MMC3_Sync();
}

void MMC3_power (CartInfo *info) {
	SetReadHandler (0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, MMC3_write);
	MMC3_reg[0] =0; MMC3_reg[1] =2; MMC3_reg[2] =4; MMC3_reg[3] =5; MMC3_reg[4] =6; MMC3_reg[5] =7; MMC3_reg[6] =0; MMC3_reg[7] =1; 
	MMC3_index =MMC3_mirroring =MMC3_wramEnable =MMC3_reloadValue =MMC3_reloadRequest =MMC3_irqEnable =MMC3_counter =0;
	MMC3_Sync();
}

void MMC3_restore (int version) {
	MMC3_Sync();
}

void MMC3_init (CartInfo *info, void (*sync)()) {
	MMC3_init_enhanced(info, sync, MMC3_TYPE_SHARP, 8);
}

void MMC3_init_enhanced (CartInfo *info, void (*sync)(), uint8 type, uint8 regs) {
	MMC3_type =type;
	MMC3_setNumberOfRegs(regs);
	MMC3_Sync =sync;
	MMC3_setBankCallback(MMC3_getPRGBank, MMC3_getCHRBank);
	MMC3_setWRAMCallback(CartBR, CartBW);
	info->Power =MMC3_power;
	info->Reset =MMC3_Sync;
	GameStateRestore = MMC3_restore;
	GameHBIRQHook =MMC3_clockCounter;
	AddExState(MMC3_StateRegs, ~0, 0, 0);
}


void MMC3_setNumberOfRegs (uint8 num) {
	MMC3_regMask =num -1;
	MMC3_stateRegs[0].s =num;
}

void MMC3_setBankCallback (uint16 (*prg)(uint8), uint16 (*chr)(uint8)) {
	MMC3_GetPRGBank =prg;
	MMC3_GetCHRBank =chr;
}

void MMC3_setWRAMCallback (FP_FASTAPASS(1)(*read) (uint32), FP_FASTAPASS(2)(*write)(uint32,uint8)) {
	MMC3_readWRAM =read;
	MMC3_writeWRAM =write;
}
