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

static uint8 reg[4];
static readfunc read4016 =NULL;

static SFORMAT stateRegs[] ={
	{ &reg, 2, "REGS" },
	{ 0 }
};

static void sync () {
	if (reg[2] &4)		
		setprg32(0x8000, reg[0]);
	else {
		setprg16(0x8000, reg[0]);
		setprg16(0xC000, reg[0]);
	}
	setchr8(0);
	setmirror(reg[2] &1? MI_H: MI_V);
}

static DECLFR(remapButtons) {
	int i;
	uint8 result =0x40;
	GetWriteHandler(0x4016)(0x4016, 1);
	GetWriteHandler(0x4016)(0x4016, 0);
	if (A ==0x4016) {
		for (i =0; i <8; i++) {
			result <<=1;
			result |=read4016(0x4016) &1;
		}
		result =(result &0x90? 0x01: 0x00) |   /* START/A */
		        (result &0x60? 0x02: 0x00)   ; /* SELECT/B */
	} else
	if (A ==0x4017) {
		int i;
		for (i =0; i <8; i++) {
			result <<=1;
			result |=read4016(0x4016) &1;
		}
		result =(result &0x04? 0x08: 0x00) | /* DOWN  */
		        (result &0x08? 0x02: 0x00) | /* UP    */
			(result &0x02? 0x04: 0x00) | /* LEFT  */
			(result &0x01? 0x10: 0x00)   /* RIGHT */
		;
	}
	return result;
}

static DECLFW(writeReg) {
	reg[A >>8 &3] =V;
	sync();
}

static void reset(void) {
	reg[0] =reg[1] =reg[2] =reg[3] =0;
	sync();
}

static void power(void) {
	if (read4016 !=&remapButtons) read4016 =GetReadHandler(0x4016);
	reset();
	SetReadHandler(0x4016, 0x4017, remapButtons);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, writeReg);
}

static void close(void) {
	read4016 =NULL;
}

void Mapper476_Init(CartInfo *info) {
	AddExState(stateRegs, ~0, 0, 0);
	info->Power =power;
	info->Reset =reset;
	info->Close =close;
	
}
