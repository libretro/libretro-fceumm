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

#include "mapinc.h"

static uint8 prg[4];
static uint8 chr;
static uint8 latch;
static uint8 reg1M;
static uint8 reg4M;

static uint8 IRQa_fds;
static int16 IRQCount_fds;
static int16 IRQCount_sgd;

static SFORMAT StateRegs[] = {
	{ &reg1M, 1, "REG1" },
	{ &reg4M, 1, "REG4" },
	{ &latch, 1, "LATC" },
	{ &chr,   1, "CREG" },
	{ prg,    4, "PREG" },

	{ &IRQa_fds,     1, "IRQA" },
	{ &IRQCount_fds, 2, "FDSC" },

	{ &IRQCount_sgd, 2, "SGDC" },

	{ 0 },
};

static void Sync(void) {
	setprg8r(0x10, 0x6000, 0);
	/* PRG memory can be writable */
	SetupCartPRGMapping(0, PRGptr[0], PRGsize[0], !(reg1M & 0x02));
	if (!(reg4M & 0x01)) {
		setprg8(0x8000, prg[0]);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, prg[2]);
		setprg8(0xE000, prg[3]);
	} else {
		switch (reg1M >> 5) {
		case 0:
			setprg16(0x8000, latch & 0x07);
			setprg16(0xC000, 0x07);
			break;
		case 1:
			setprg16(0x8000, (latch >> 2) & 0x0F);
			setprg16(0xC000, 0x07);
			break;
		case 2:
			setprg16(0x8000, latch & 0x0F);
			setprg16(0xC000, 0x0F);
			break;
		case 3:
			setprg16(0x8000, 0x0F);
			setprg16(0xC000, latch & 0x0F);
			break;
		case 4:
			setprg32(0x8000, (latch >> 4) & 0x03);
			break;
		case 5:
			setprg32(0x8000, 0x03);
			break;
		case 6:
			setprg8(0x8000, latch & 0x0F);
			setprg8(0xA000, latch >> 4);
			setprg16(0xC000, 0x07);
			break;
		case 7:
			setprg8(0x8000, latch & 0x0E);
			setprg8(0xA000, (latch >> 4) | 0x01);
			setprg16(0xC000, 0x07);
			break;
		}
	}

	/* CHR RAN can be write-protected */
	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], !((reg1M & 0xE0) & 0x80));
	setchr8(chr);

	switch (reg1M & 0x11) {
	case 0x00: setmirror(MI_0); break;
	case 0x01: setmirror(MI_V); break;
	case 0x10: setmirror(MI_1); break;
	case 0x11: setmirror(MI_H); break;
	}
}

static DECLFW(M561WriteReg) {
	switch (A) {
	case 0x4024:
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x4025:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQa_fds = V;
		if (IRQa_fds & 0x42) {
			IRQCount_fds = 0;
		}
		break;
	case 0x4100:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQCount_sgd = (int16)((IRQCount_sgd & 0xFF00) | V);
		if (!V) {
			IRQCount_sgd = V;
		}
		break;
	case 0x4101:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQCount_sgd = (int16)((IRQCount_sgd & 0x00FF) | (V << 8));
		break;
	case 0x42FC:
	case 0x42FD:
	case 0x42FE:
	case 0x42FF:
		reg1M = (V & 0xF0) | (A & 0x03);
		Sync();
		break;
	case 0x43FC:
	case 0x43FD:
	case 0x43FE:
	case 0x43FF:
		reg4M = (V & 0xF0) | (A & 0x03);
		chr = V & 0x03;
		Sync();
		break;
	}
}

static DECLFW(M561Write) {
	if (reg1M & 0x02) {
		latch = V;
		switch (reg1M >> 5) {
		case 0:
		case 2:
			chr = 0;
			break;
		case 1:
		case 4:
		case 5:
			chr = latch & 0x03;
			break;
		case 3:
			chr = (latch >> 4) & 0x03;
			break;
		default:
			/* keep chr bank from last mode */
			break;
		}
		prg[(A >> 13) & 0x03] = V >> 2;
		Sync();
	} else {
		CartBW(A, V);
	}
}

static void M561Reset(void) {
	IRQa_fds = IRQCount_fds = IRQCount_sgd = 0;
	Sync();
}

static void M561Power(void) {
	reg1M = (iNESCart.submapper << 5) | ((iNESCart.mirror == MI_V) ? 0x01 : 0x11) | 0x02;
	reg4M = 0x03;
	prg[0] = 0x1C;
	prg[1] = 0x1D;
	prg[2] = 0x1E;
	prg[3] = 0x1F;
	chr = 0;

	SetWriteHandler(0x4020, 0x4FFF, M561WriteReg);

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, M561Write);

	SetReadHandler(0x6000, 0x7FFF, CartBR);
    SetWriteHandler(0x6000, 0x7FFF, CartBW);
	FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);

	Sync();

	if (MISC_ROM_SIZE && (MISC_ROM_SIZE >= 4)) {
		uint16 trainerLoadAddr = 0x7000;
		uint16 trainerInitAddr = 0x7003;
		uint32 trainerSize = 512;
		uint8 *trainerData = MISC_ROM_PTR;
		int i;

		if (MISC_ROM_SIZE != 512) {
			trainerLoadAddr = (MISC_ROM_PTR[1] << 8) | MISC_ROM_PTR[0];
			trainerInitAddr = (MISC_ROM_PTR[3] << 8) | MISC_ROM_PTR[2];
			trainerSize = MISC_ROM_SIZE - 4;
			trainerData = MISC_ROM_PTR + 4;
		}

		for (i = 0; i < MISC_ROM_SIZE; i++) {
			WRAM[(trainerLoadAddr & 0x1FFF) + i] = trainerData[i];
		}

		if (trainerInitAddr) {
			/* JSR init */
			(GetWriteHandler(0x0700))(0x0700, 0x20);
			(GetWriteHandler(0x0701))(0x0701, trainerInitAddr & 0xFF);
			(GetWriteHandler(0x0702))(0x0702, trainerInitAddr >> 8);

			/* JMP ($FFFC) */
			(GetWriteHandler(0x0703))(0x0703, 0x6C);
			(GetWriteHandler(0x0704))(0x0704, 0xFC);
			(GetWriteHandler(0x0705))(0x0705, 0xFF);

			X6502_SetNewPC(0x700);
		}
	}
	(GetWriteHandler(0x4017))(0x4017, 0x40);
}

static void StateRestore(int version) {
	Sync();
}

static void M561CPUIRQHook(int a) {
	IRQCount_fds += 3 * a;
	while ((IRQCount_fds >= 448) && (IRQa_fds & 0x80)) {
		IRQCount_fds -= 448;
		X6502_IRQBegin(FCEU_IQEXT);
	}
	if (IRQCount_sgd < 0) {
		IRQCount_sgd += a;
		if (IRQCount_sgd >= 0) {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

void Mapper561_Init(CartInfo *info) {
	uint32 wramsize = info->PRGRamSize + info->PRGRamSaveSize;
	int i, ssize = PRG_ROM_SIZE;

	info->Power = M561Power;
	info->Reset = M561Reset;
	MapIRQHook = M561CPUIRQHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = wramsize ? wramsize : 8192;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_malloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, TRUE);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}

	if ((info->submapper == 3) && (PRG_ROM_SIZE < (256 * 1024))) {
		uint8 *tmp = (uint8 *)FCEU_malloc(PRG_ROM_SIZE);

		for (i = 0; i < ssize; i++) {
			tmp[i] = PRG_ROM_PTR[i];
		}
		ssize = (256 * 1024);
		PRG_ROM_PTR = (uint8 *)realloc(PRG_ROM_PTR, ssize);
		for (i = 0; i < ssize; i++) {
			PRG_ROM_PTR[i] = tmp[i];
		}
		FCEU_free(tmp);
		SetupCartPRGMapping(0, PRG_ROM_PTR, ssize, FALSE);
	}

	if (CHR_ROM_SIZE) {
		/* Game Doctor only used CHR RAM, but some .nes files has CHR ROM instead. */
		/* Remap CHR as writable. */
		SetupCartCHRMapping(0, CHR_ROM_PTR, CHR_ROM_SIZE, TRUE);
		AddExState(CHR_ROM_PTR, CHR_ROM_SIZE, 0, "CHRM");
	}

	/* PRG memory can be writable, so add to states */
	AddExState(PRG_ROM_PTR, ssize, 0, "PRGM");
}
