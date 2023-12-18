/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2020 NewRisingSun
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

/* Multicarts based around the ING003C and TEC9719 enhanced MMC3 ASICs.
   ING003C's scanline counter is inverted from MMC3, while TEC9719 is normal.
   Both ASICs natively only support 256 KiB of CHR ROM/RAM; cartridges with more than that re-purpose higher PRG address as CHR address lines. There are two connection variants of this re-purposing.   
   Mapper 422: TEC9719, CHR A18=PRG A20, CHR A19=PRG A21
   Mapper 126: TEC9719, CHR A18=PRG A21, CHR A19=PRG A20
   Mapper 534: ING003C, CHR A18=PRG A20, CHR A19=PRG A21
   
   Cartridges with more than one PRG ROM chip are sometimes not connected to form one continuous address space. Submappers are used to avoid having to repeat data to accomodate them.
   Submapper 0: Normal connection
   Submapper 1: PRG A21 (2 MiB bank) selects between two 1 MiB chips
   Submapper 2: Register bit 6001.2 (undocumented in data sheet) selects between two 1 MiB chips
   
   Both ASICs invert the register bit that selects PRG A21 (6000.5), hence "EXPREGS[0] ^0x20".
*/

#include "mapinc.h"
#include "mmc3.h"

static uint8 reverseCHR_A18_A19;
static uint8 invertC000;
static uint8 SL0;
static uint8 submapper;

static uint8 getMMC3Bank(int bank) {
	if (~bank &1 && MMC3_cmd &0x40) bank ^=2;
	return bank &2? 0xFE | bank &1: DRegBuf[6 | bank &1];
}

static void wrapPRG(uint32 A, uint8 V) {
	int prgAND = EXPREGS[0] &0x40? 0x0F: 0x1F; /* 128 KiB or 256 KiB inner PRG bank selection */
	int prgOR  =(EXPREGS[0] <<4 &0x70 | (EXPREGS[0] ^0x20) <<3 &0x180) &~prgAND; /* Outer PRG bank */
	if (submapper ==1) prgOR |=prgOR >>1 &0x80;
	if (submapper ==2) prgOR |=EXPREGS[1] <<5 &0x80;
	for (A =0; A <4; A++) {
		/* In UNROM-like mode (CT3=1, CT2=1, CT0=1), MMC3 sees A13=0 and A14=CPU A14 during reads, making register 6 apply from $8000-$BFFF, and the fixed bank from $C000-$FFFF.
		   In NROM-128, NROM-256, ANROM and UNROM modes (CT0=1), MMC3 sees A13=0 and A14=0, making register 6 apply from $8000-$FFFF. */
		V =getMMC3Bank(A &((EXPREGS[3] &0x0D) ==0x0D? 2: EXPREGS[3] &0x01? 0: 3));
		
		/* UNROM and ANROM modes mean that MMC3 register 6 selects 16 and 32 KiB rather than 8 KiB banks. */
		if (EXPREGS[3] &0x08) switch(EXPREGS[3] &0x03) {
			case 0: V =V &3 | V <<1 &~3; break; /* PRG A14 appears twice */
			case 1: V =A &3 | V <<1 &~1; break; /* 16 KiB mode, bit 0 OR'd with CPU A14 */
			case 2: V =V &3 | V <<2 &~3; break; /* PRG A13 and PRG A14 appear twice */
			case 3: V =A &3 | V <<2 &~3; break; /* 32 KiB mode */
		} else
		if (EXPREGS[3] &0x01) { /* regular NROM modes */
			V =A &1 | V &~1;
			if (EXPREGS[3] &0x02) V =A &2 | V &~2; /* NROM-256 */
		}
		setprg8(0x8000 +A*0x2000, V &prgAND | prgOR &~prgAND);		
	}
	mwrap(A000B); /* After 8000 write */
}

static void wrapCHR(uint32 A, uint8 V) {
	int chrAND = EXPREGS[0] &0x80? 0x7F: 0xFF; /* 128 KiB or 256 KiB innter CHR bank selection */
	int chrOR; /* outer CHR bank */
	if (reverseCHR_A18_A19) /* Mapper 126 swaps CHR A18 and A19 */
		chrOR =(EXPREGS[0] <<4 &0x080 | (EXPREGS[0] ^0x20) <<3 &0x100 | EXPREGS[0] <<5 &0x200) &~chrAND;
	else
		chrOR =(EXPREGS[0] ^0x20) <<4 &0x380 &~chrAND;
	
	if (EXPREGS[3] &0x10) /* CNROM mode: 8 KiB inner CHR bank comes from outer bank register #2 */
		setchr8(EXPREGS[2] &(chrAND >>3) | (chrOR &~chrAND) >>3);
	else /* MMC3 CHR mode */
		setchr1(A, (V & chrAND) | chrOR);
}

static void wrapMirroring(uint8 V) {
	A000B =V;
	if (EXPREGS[3] &0x20) { /* ANROM mirroring */
		if (DRegBuf[6] &0x10)
			setmirror(MI_1);
		else
			setmirror(MI_0);
	} else
	if (EXPREGS[1] &0x02) { /* Extended MMC3 mirroring */
		switch(A000B &3) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	} else              /* Normal MMC3 mirroring */
		setmirror(A000B &1? MI_H: MI_V);
	
}

static void writeWRAM(uint32 A, uint8 V) {
	CartBW(A, V);
	if ((A &3) ==2) { /* CNROM Bank (D0-D3), Bank Enable (D4-D6) and Bank Enable Lock (D7) */
		int latchMask =0xFF &~(EXPREGS[2] &0x80? 0x70: 0x00) &~(EXPREGS [2] >>3 &0x0E);
		EXPREGS[2] =EXPREGS[2] &~latchMask | V &latchMask;
		FixMMC3CHR(MMC3_cmd);
	} else
	if (~EXPREGS[3] &0x80) {
		/* Lock bit clear: Update any outer bank register */
		EXPREGS[A &3] =V;
		FixMMC3PRG(MMC3_cmd);
		FixMMC3CHR(MMC3_cmd);
		mwrap(A000B); /* After 6001 or 6003 swite */
	}
}

static void writeCart(uint32 A, uint8 V) {
	if ((EXPREGS[3] &0x09) ==0x09) /* UNROM and ANROM modes treat all writes to $8000-$FFFF as if they were going to $8000-$9FFF */
		MMC3_CMDWrite(0x8000 | (EXPREGS[3] &0x08? 1: A) &1, V); /* A0 substitution only looks at bit 3 of register 3 */
	else
	if (A >=0xC000)
		MMC3_IRQWrite(A, V ^(invertC000? 0xFF: 0x00)); /* Mapper 534 inverts the MMC3 scanline counter reload value */
	else
		MMC3_CMDWrite(A, V);	
}

static uint8 readPRG(uint32 A) {
	if (EXPREGS[1] &1) A =A &~1 | SL0 &1; /* Replace A0 with SL0 input */
	return CartBR(A);
}

static void reset(void) {
	SL0++; /* Soft-resetting cycles through SL0 settings */
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	MMC3RegReset();
}

static void power(void) {
	SL0 =0;
	EXPREGS[0] = EXPREGS[1] = EXPREGS[2] = EXPREGS[3] = 0;
	GenMMC3Power();
	SetWriteHandler(0x6000, 0x7FFF, writeWRAM);
	SetWriteHandler(0x8000, 0xFFFF, writeCart);
	SetReadHandler(0x8000, 0xFFFF, readPRG);
}

static void init(CartInfo *info) {
	GenMMC3_Init(info, 512, 256, 8, info->battery);
	cwrap =wrapCHR;
	pwrap =wrapPRG;
	mwrap =wrapMirroring;
	submapper =info->submapper;

	info->Power = power;
	info->Reset = reset;

	AddExState(EXPREGS, 4, 0, "EXPR");
	AddExState(&SL0, 1, 0, "DPSW");
}

void Mapper126_Init(CartInfo *info) {
	if (info->CRC32 ==0xEAD80031 || info->CRC32 ==0x6FCBC309) { /* Old dumps: Invert CHR A18 */		
		int i, a, b;
		for (i =0; i <0x40000; i++) {
			a =VROM[0x00000 +i]; b =VROM[0x40000 +i];
			VROM[0x00000 +i] =b; VROM[0x40000 +i] =a;
			a =VROM[0x80000 +i]; b =VROM[0xC0000 +i];
			VROM[0x80000 +i] =b; VROM[0xC0000 +i] =a;
		}
	}
	reverseCHR_A18_A19 = 1;
	invertC000 = 0;
	init(info);
}

void Mapper422_Init(CartInfo *info) {
	if (info->CRC32 ==0x6D61FE21 || info->CRC32 ==0x3FF46175 || info->CRC32 ==0xA3FF9D9B || info->CRC32 ==0x2BDD0FC2 || info->CRC32 ==0x5789017D || info->CRC32 ==0x46A01871 || info->CRC32 ==0x2466B80A) {
		/* Old dumps: Invert CHR A19 */
		int i, a, b;
		for (i =0; i <0x80000; i++) {
			a =VROM[0x00000 +i]; b =VROM[0x80000 +i];
			VROM[0x00000 +i] =b; VROM[0x80000 +i] =a;
		}
	}
	reverseCHR_A18_A19 = 0;
	invertC000 = 0;
	init(info);
}

void Mapper534_Init(CartInfo *info) {
	if (info->CRC32 ==0x871CFD16) {
		/* Old dump: Invert PRG A20 */
		int i, a, b;
		for (i =0; i <0x100000; i++) {
			a =ROM[0x000000 +i]; b =ROM[0x100000 +i];
			ROM[0x000000 +i] =b; ROM[0x100000 +i] =a;
		}
	} else
	if (info->CRC32 ==0xB2724618 || info->CRC32 ==0x42A9219D) {
		/* Old dumps: Invert PRG A21 */
		int i, a, b;
		for (i =0; i <0x200000; i++) {
			a =ROM[0x000000 +i]; b =ROM[0x200000 +i];
			ROM[0x000000 +i] =b; ROM[0x200000 +i] =a;
		}
	}	
	reverseCHR_A18_A19 = 0;
	invertC000 = 1;
	init(info);
}
