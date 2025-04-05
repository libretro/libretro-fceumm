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

static uint8 reg[5], dip; /* Fourth register is the CNROM latch */
static uint8 MMC3_reg[8], MMC3_index, MMC3_mirroring, MMC3_wram, MMC3_reload, MMC3_count, MMC3_irq;
static uint8 VRC4_prg[2];
static uint8 VRC4_mirroring;
static uint8 VRC4_misc;
static uint16 VRC4_chr[8];
static uint8 VRCIRQ_latch;
static uint8 VRCIRQ_mode;
static uint8 VRCIRQ_count;
static signed short int VRCIRQ_cycles;
static uint8 *PRGCHR =NULL;

static SFORMAT stateRegs[] = {
	{ reg,             5, "REGS" },
	{ &dip,            1, "DIPS" },
	{ MMC3_reg,        1, "MMC3" },
	{ &MMC3_index,     1, "M3IX" },
	{ &MMC3_mirroring, 1, "M3MI" },
	{ &MMC3_wram,      1, "M3WR" },
	{ &MMC3_reload,    1, "M3RL" },
	{ &MMC3_count,     1, "M3CN" },
	{ &MMC3_irq,       1, "M3IQ" },
	{ VRC4_prg,        2, "V4PR" },
	{ &VRC4_mirroring, 1, "V4MI" },
	{ &VRC4_misc,      1, "V4MS" },
	{ VRC4_chr,       16, "V4CH" },
	{ &VRCIRQ_latch,   1, "VILA" },
	{ &VRCIRQ_mode,    1, "VIMO" },
	{ &VRCIRQ_count,   1, "VICO" },
	{ &VRCIRQ_cycles,  2, "VICY" },
	{ 0 }
};


static void sync () {
	int chrAND;
	int chrOR;
	/*/ 5003.0-2: PRG size (1 MiB->8 KiB, although 1 MiB and 512 KiB are not connected and so function as 256 KiB) and mode (>=64 KiB: MMC3, <64 KiB: NROM-256/128/64) */
	int prgAND =0x7F >>(reg[2] &7) &0x1F;
	if (prgAND &0x04) {
		if (reg[3] &0x10) {
			setprg8(0x8000 ^(VRC4_misc <<13 &0x4000), VRC4_prg[0] &prgAND | reg[0] &~prgAND);
			setprg8(0xA000,                           VRC4_prg[1] &prgAND | reg[0] &~prgAND);
			setprg8(0xC000 ^(VRC4_misc <<13 &0x4000),        0xFE &prgAND | reg[0] &~prgAND);
			setprg8(0xE000,                                  0xFF &prgAND | reg[0] &~prgAND);
		} else {
			setprg8(0x8000 ^(MMC3_index <<8 &0x4000), MMC3_reg[6] &prgAND | reg[0] &~prgAND);
			setprg8(0xA000,                           MMC3_reg[7] &prgAND | reg[0] &~prgAND);
			setprg8(0xC000 ^(MMC3_index <<8 &0x4000),        0xFE &prgAND | reg[0] &~prgAND);
			setprg8(0xE000,                                  0xFF &prgAND | reg[0] &~prgAND);
		}
	} else {
		setprg8(0x8000, 0 &prgAND | reg[0] &~prgAND);
		setprg8(0xA000, 1 &prgAND | reg[0] &~prgAND);
		setprg8(0xC000, 2 &prgAND | reg[0] &~prgAND);
		setprg8(0xE000, 3 &prgAND | reg[0] &~prgAND);
	}

	/* // 5003.3-5: CHR size (1 MiB->8 KiB, though 1 MiB and 512 KiB are not reachable with inner bank registers and so function as 256 KiB) and mode (>=64 KiB: MMC3, <64 KiB: (C)NROM-256/128/64) */
	chrAND =0x3FF >>(reg[2] >>3 &7) &0xFF;
	if (chrAND &0x20) {
		if (reg[3] &0x10) {
			setchr1(0x0000, VRC4_chr[0] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0400, VRC4_chr[1] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0800, VRC4_chr[2] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0C00, VRC4_chr[3] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1000, VRC4_chr[4] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1400, VRC4_chr[5] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1800, VRC4_chr[6] &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1C00, VRC4_chr[7] &chrAND | (reg[1] <<3) &~chrAND);
		} else {
			setchr1(0x0000 ^(MMC3_index <<5 &0x1000),(MMC3_reg[0] &0xFE)&chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0400 ^(MMC3_index <<5 &0x1000),(MMC3_reg[0] |0x01)&chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0800 ^(MMC3_index <<5 &0x1000),(MMC3_reg[1] &0xFE)&chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x0C00 ^(MMC3_index <<5 &0x1000),(MMC3_reg[1] |0x01)&chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1000 ^(MMC3_index <<5 &0x1000), MMC3_reg[2]       &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1400 ^(MMC3_index <<5 &0x1000), MMC3_reg[3]       &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1800 ^(MMC3_index <<5 &0x1000), MMC3_reg[4]       &chrAND | (reg[1] <<3) &~chrAND);
			setchr1(0x1C00 ^(MMC3_index <<5 &0x1000), MMC3_reg[5]       &chrAND | (reg[1] <<3) &~chrAND);
		}
	} else {
		chrAND >>=3;
		setchr8(reg[4] &chrAND | reg[1] &~chrAND);
	}
	
	if (reg[3] &0x10)
		setmirror(VRC4_mirroring &3 ^(VRC4_mirroring &2? 0: 1));
	else
		setmirror(MMC3_mirroring &1 ^1);
	
	SetReadHandler(0x8000, 0xFFFF, reg[0] &0xC0 && (reg[0] &0xC0) ==dip? NULL: CartBR);
}

static DECLFW(writeCNROM) {
	reg[4] =V;
	sync();
}

static DECLFW(writeMMC3) {
	switch(A &0xE001) {
	case 0x8000: MMC3_index =V;              sync();    break;
	case 0x8001: MMC3_reg[MMC3_index &7] =V; sync();    break;
	case 0xA000: MMC3_mirroring =V;          sync();    break;
	case 0xA001: MMC3_wram =V;               sync();    break;
	case 0xC000: MMC3_reload =V;                        break;
	case 0xC001: MMC3_count =0;                         break;
	case 0xE000: MMC3_irq =0; X6502_IRQEnd(FCEU_IQEXT); break;
	case 0xE001: MMC3_irq =1;                           break;
	}
}

static DECLFW(writeVRC4) {
	uint8 index;
	if (reg[3] &1)
		A =(A &0xA? 1: 0) | (A &0x5? 2: 0) | A &0xF000;
	else
		A =(A &0x5? 1: 0) | (A &0xA? 2: 0) | A &0xF000;
	switch (A &0xF000) {
	case 0x8000: case 0xA000:
		VRC4_prg[A >>13 &1] =V;
		sync();
		break;
	case 0x9000:
		if (~A &2)
			VRC4_mirroring =V;
		else
		if (~A &1)
			VRC4_misc =V;
		sync();
		break;
	case 0xF000:
		switch (A &3) {
		case 0: VRCIRQ_latch =VRCIRQ_latch &0xF0 | V &0x0F; break;
		case 1: VRCIRQ_latch =VRCIRQ_latch &0x0F | V <<4;   break;
		case 2: VRCIRQ_mode =V;
		        if (VRCIRQ_mode &0x02) {
				VRCIRQ_count =VRCIRQ_latch;
				VRCIRQ_cycles =341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3: VRCIRQ_mode =VRCIRQ_mode &~0x02 | VRCIRQ_mode <<1 &0x02;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
		break;
	default:
		index =(A -0xB000) >>11 &~1 | A >>1 &1;
		if (A &1)
			VRC4_chr[index] =VRC4_chr[index] & 0x0F | V <<4;
		else
			VRC4_chr[index] =VRC4_chr[index] &~0x0F | V &0x0F;
		sync();
		break;
	}
}

static void FP_FASTAPASS(1) cpuCycle(int a) {
	if ((reg[0] &3) ==3) while (a--) { /* VRC4 mode */
		if (VRCIRQ_mode &0x02 && (VRCIRQ_mode &0x04 || (VRCIRQ_cycles -=3) <=0)) {
			if (~VRCIRQ_mode &0x04) VRCIRQ_cycles +=341;
			if (!++VRCIRQ_count) {
				VRCIRQ_count =VRCIRQ_latch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}
	
static void horizontalBlanking(void) {
	if (~reg[0] &2) { /* MMC3 mode */
		MMC3_count =!MMC3_count? MMC3_reload: --MMC3_count;
		if (!MMC3_count && MMC3_irq) X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void applyMode() {
	if ((reg[2] >>3 &7) >=5)
		SetWriteHandler(0x8000, 0xFFFF, writeCNROM);
	else
	if (reg[3] &0x10)
		SetWriteHandler(0x8000, 0xFFFF, writeVRC4); 
	else
		SetWriteHandler(0x8000, 0xFFFF, writeMMC3);
}

static void Mapper445_restore (int version) {
	applyMode();
	sync();
}

static DECLFW(writeReg) {
	if (~reg[3] &0x20) {
		reg[A &3] =V;
		if ((A &3) ==3) applyMode();
		sync();
	}
}

static void Mapper445_power(void) {
	int i;
	for (i =0; i <4; i++) reg[i] =0;
	for (i =0; i <8; i++) MMC3_reg[i] =0;
	for (i =0; i <2; i++) VRC4_prg[i] =0;
	for (i =0; i <8; i++) VRC4_chr[i] =0;
	MMC3_index =MMC3_mirroring =MMC3_wram =MMC3_reload =MMC3_count =MMC3_irq =0;	
	VRC4_mirroring =VRC4_misc =VRCIRQ_latch =VRCIRQ_mode =VRCIRQ_count =VRCIRQ_cycles =0;
	dip =0;
	
	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
	applyMode();
	sync();
}

static void Mapper445_reset (void) {
	int i;
	for (i =0; i <5; i++) reg[i] =0;
	dip +=0x40;
	applyMode();
	sync();
}

static void Mapper445_close(void) {
	if (PRGCHR) FCEU_gfree(PRGCHR);
	PRGCHR =NULL;
}

void Mapper445_Init (CartInfo *info) {
	info->Reset = Mapper445_reset;
	info->Power = Mapper445_power;
	info->Close = Mapper445_close;
	MapIRQHook = cpuCycle;
	GameHBIRQHook = horizontalBlanking;
	GameStateRestore = Mapper445_restore;
	AddExState(stateRegs, ~0, 0, 0);	
}

