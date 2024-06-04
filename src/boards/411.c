/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020
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
 */

/* Mapper 411 - A88S-1
 * 1997 Super 7-in-1 (JY-201)
 * 1997 Super 6-in-1 (JY-202)
 * 1997 Super 7-in-1 (JY-203)
 * 1997 龍珠武鬥會 7-in-1 (JY-204)
 * 1997 Super 7-in-1 (JY-205)
 * 1997 Super 7-in-1 (JY-206)
 */

#include "mapinc.h"
#include "mmc3.h"

static uint8 submapper;

static void M411CW(uint32 A, uint8 V) {
	int chrAND, chrOR;
	switch(submapper) {
		default:chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[0] <<4 &0x100 | EXPREGS[1] <<2 &0x200;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
		case 1: chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[1] <<2 &0x100;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
		case 2: chrOR =EXPREGS[1] <<5 &0x080 | EXPREGS[0] <<4 &0x100 | EXPREGS[1] <<2 &0x200;
			chrAND =EXPREGS[1] &0x02? 0xFF: 0x7F;
			break;
	}
	setchr1(A, V &chrAND | chrOR &~chrAND);
}

static void M411PW(uint32 A, uint8 V) {
	int prgAND, prgOR;
	switch(submapper) {
		default:prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x02? 0x1F: 0x0F;
			break;
		case 1: prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x02? 0x1F: 0x0F;
			break;
		case 2: prgOR =EXPREGS[1] <<1 &0x10 | EXPREGS[1] >>1 &0x60;
			prgAND =EXPREGS[1] &0x01? 0x1F: 0x0F;
			break;
	}	
	if (EXPREGS[0] & 0x40) { /* NROM Mode */
		uint32 bank = EXPREGS[0] &5 | EXPREGS[0] >>2 &2 | prgOR >>1;		
		if (EXPREGS[0] & 0x02) /* NROM-256 */
			setprg32(0x8000, bank >> 1);
		else { 	/* NROM-128 */
			setprg16(0x8000, bank);
			setprg16(0xC000, bank);
		}
	} else
		setprg8(A, V &prgAND | prgOR &~prgAND);
}

static DECLFR(M411Read5000) {
	return EXPREGS[2];
}

static DECLFW(M411Write5000) {
	if (submapper ==2 || A &0x800) {
		EXPREGS[A & 1] = V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
	}
}

static void M411Reset(void) {
	EXPREGS[2]++;
}

static void M411Power(void) {
	EXPREGS[0] = 0x0;
	EXPREGS[1] = 0x3;
	EXPREGS[2] = 0x0; /* Serves as DIP value */
	GenMMC3Power();
	SetReadHandler(0x5000, 0x5FFF, M411Read5000);
	SetWriteHandler(0x5000, 0x5FFF, M411Write5000);
}

void Mapper411_Init(CartInfo *info) {
	submapper =info->submapper;
	GenMMC3_Init(info, 256, 256, 0, 0);
	pwrap = M411PW;
	cwrap = M411CW;
	info->Power = M411Power;
	info->Reset = M411Reset;
	AddExState(EXPREGS, 3, 0, "EXPR");
}
