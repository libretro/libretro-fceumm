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
 *
 * Front Fareast Magic Card 1M or 2M RAM cartridges
 *
 */

#include "mapinc.h"
#include "mapper006.h"

static uint8 mode_1m;
static uint8 mode_2m;
static uint8 mode_smc;

static uint8 prg[4];
static uint8 chr[8];
static uint8 nt[4];
static uint8 chr_mmc4latch[2];
static uint8 latch;
static uint8 chr_lock;

static uint8 smc_irq_enabled;
static uint32 smc_irq_counter;

static uint8 fds_control;
static int16 fds_irq_counter;

static uint8 scratch[0x1000];

static SFORMAT StateRegs[] = {
	{ &mode_1m,         1, "MC1M" },
	{ &mode_2m,         1, "MC2M" },
	{ &mode_smc,        1, "SMCM" },

	{ prg,              4, "PREG" },
	{ chr,              8, "CREG" },
	{ nt,               4, "NTAR" },
	{ chr_mmc4latch,    2, "MMC4" },
	{ &latch,           1, "LATC" },
	{ &chr_lock,        1, "CHRL" },

	{ &smc_irq_enabled, 1, "IRQA" },
	{ &smc_irq_enabled, 4, "IRQC" },

	{ &fds_control,     1, "FDSI" },
	{ &fds_irq_counter, 4, "FDSC" },

	{ 0 },
};

static void SyncPRG(void) {
	uint8 prg_writable = !(mode_1m & 0x02);

	SetupCartPRGMapping(0, PRGptr[0], PRGsize[0], prg_writable);

	if (!(mode_2m & 0x01)) {
		setprg8(0x8000, prg[0]);
		setprg8(0xA000, prg[1]);
		setprg8(0xC000, prg[2]);
		setprg8(0xE000, prg[3]);
	} else {
		switch (mode_1m >> 5) {
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
		case 6:
		case 7:
			setprg32(0x8000, 0x03);
			break;
		}
	}
}

static void SyncCHR(void) {
	uint8 chr_writable = !(((mode_1m & 0xE1) >= 0x81) || chr_lock);

	SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], chr_writable);

	if (mode_smc & 0x01) {
		if (mode_smc & 0x04) {
			setchr1(0x0000, chr[0]);
			setchr1(0x0400, chr[1]);
			setchr1(0x0800, chr[2]);
			setchr1(0x0C00, chr[3]);
			setchr1(0x1000, chr[4]);
			setchr1(0x1400, chr[5]);
			setchr1(0x1800, chr[6]);
			setchr1(0x1C00, chr[7]);
		} else {
			setchr4(0x0000, chr[0 | chr_mmc4latch[0]] >> 2);
			setchr4(0x1000, chr[4 | chr_mmc4latch[1]] >> 2);
		}
	} else {
		switch (mode_1m >> 5) {
		case 0:
		case 2:
			setchr8(0);
			break;
		case 1:
		case 4:
		case 5:
			setchr8(latch & 0x03);
			break;
		case 3:
			setchr8((latch >> 4) & 0x03);
			break;
		case 6:
			setchr8(latch & 0x01);
			break;
		case 7:
			setchr8(0x03);
			break;
		}
	}
}

static void SyncMIR(void) {
	if (mode_smc & 0x02) {
		switch (mode_1m & 0x11) {
		case 0x00: setmirror(MI_0); break;
		case 0x10: setmirror(MI_1); break;
		case 0x01: setmirror(MI_V); break;
		case 0x11: setmirror(MI_H); break;
		}
	} else {
		setntamem(CHRptr[0] + 0x400 * (nt[0] & CHRmask1[0]), 1, 0);
		setntamem(CHRptr[0] + 0x400 * (nt[1] & CHRmask1[0]), 1, 1);
		setntamem(CHRptr[0] + 0x400 * (nt[2] & CHRmask1[0]), 1, 2);
		setntamem(CHRptr[0] + 0x400 * (nt[3] & CHRmask1[0]), 1, 3);
	}
}

static void Sync(void) {
	setprg4r(0x11, 0x5000, 0);
	setprg8r(0x10, 0x6000, 0);

	SyncPRG();
	SyncCHR();
	SyncMIR();
}

static DECLFR(M006ReadREG) {
	return mode_smc;
}

static DECLFW(M006WriteREG) {
	switch (A) {
	case 0x4024:
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x4025:
		X6502_IRQEnd(FCEU_IQEXT);
		if (!smc_irq_enabled) {
			fds_control = V;
			if (V & 0x42) {
				fds_irq_counter = 0;
			}
		}
		break;
	case 0x42FC:
	case 0x42FD:
	case 0x42FE:
	case 0x42FF:
		mode_1m = (V & 0xF0) | (A & 0x03);
		if (mode_1m >= 0x80) {
			chr_lock = FALSE;
		}
		SyncPRG();
		SyncCHR();
		SyncMIR();
		break;
	case 0x43FC:
	case 0x43FD:
	case 0x43FE:
	case 0x43FF:
		mode_2m = (V & 0xF0) | (A & 0x03);
		latch = V;
		SyncPRG();
		SyncCHR();
		break;
	case 0x4500:
		mode_smc = V;
		SyncCHR();
		SyncMIR();
		break;
	case 0x4501:
		smc_irq_enabled = FALSE;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x4502:
		smc_irq_counter = (smc_irq_counter & 0xFF00) | V;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x4503:
		smc_irq_enabled = TRUE;
		smc_irq_counter = (smc_irq_counter & 0x00FF) | (V << 8);
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0x4504:
	case 0x4505:
	case 0x4506:
	case 0x4507:
		if (mode_2m & 0x02) {
			V >>= 2;
		}
		prg[A & 0x03] = V;
		SyncPRG();
		break;
	case 0x4510:
	case 0x4511:
	case 0x4512:
	case 0x4513:
	case 0x4514:
	case 0x4515:
	case 0x4516:
	case 0x4517:
		chr[A & 0x07] = V;
		SyncCHR();
		break;
	case 0x4518:
	case 0x4519:
	case 0x451A:
	case 0x451B:
		nt[A & 0x03] = V;
		SyncMIR();
		break;
	}
}

static DECLFW(M006WriteLatch) {
	if (mode_1m & 0x02) {
		latch = V;
		if (mode_2m & 0x03) {
			prg[(A >> 13) & 0x03] = V >> 2;
		}
		SyncPRG();
		SyncCHR();
	} else {
		CartBW(A, V);
	}
}

static void M006Reset(void) {
	smc_irq_enabled = FALSE;
	smc_irq_counter = 0;
	fds_control = 0;
	fds_irq_counter = 0;
	Sync();
}

static uint32 prg_size_8K = 0;
static void SetTrainer(void) {
	int nmiHandler;

	#define PRGPAGE_DMR(a)    Page[(a) >> 11][(a)]
	#define PRGPAGE_DMW(a, d) Page[(a) >> 11][(a)] = (d)

	nmiHandler = PRGPAGE_DMR(0xFFFA) | (PRGPAGE_DMR(0xFFFB) << 8);
	if (nmiHandler == 0x5032) {
		PRGPAGE_DMW(0xFFFA, scratch[0x4F]);
		PRGPAGE_DMW(0xFFFB, scratch[0x50]);
	}
	memcpy(&scratch[0], &smc5000[0], sizeof(smc5000));
	scratch[0x4F] = PRGPAGE_DMR(0xFFFA);
	scratch[0x50] = PRGPAGE_DMR(0xFFFB);
	if (iNESCart.mapper == 17) {
		PRGPAGE_DMW(0xFFFA, 0x32);
		PRGPAGE_DMW(0xFFFB, 0x50);
	}
	if (iNESCart.HasTrainer && WRAM) {
		int i;
		uint8 *trainerData = 0;
		uint16 trainerAddr =
			iNESCart.mapper != 17   ? 0x7000 :
			iNESCart.submapper == 0 ? 0x7000 :
			iNESCart.submapper << 8 | 0x5C00;

		for (i = 0; i < (int)MISC_ROM_SIZE; i++) {
			PRGPAGE_DMW((trainerAddr & 0x7F00) + i, MISC_ROM_PTR[i]);
		}

		X6502_SetNewPC((iNESCart.mapper == 17) ? trainerAddr : 0x5000);
	}
	(GetWriteHandler(0x4017))(0x4017, 0x40);

	#undef PRGPAGE_DMR
	#undef PRGPAGE_DMW
}

static void M006Power(void) {
	mode_1m = (((iNESCart.mapper == 6) ? iNESCart.submapper : 1) << 5) |
			  ((iNESCart.mirror & MI_V) ? 0x01 : 0x11) |
			  0x02;
	chr_lock = FALSE;

	mode_2m = ((iNESCart.mapper == 12) ||
		(iNESCart.mapper == 17)) ? 0x00 : 0x03;
	mode_smc = (iNESCart.mapper == 17) ? 0x47 : 0x42;
	latch    = 0;

	prg[0] = prg_size_8K - 4;
	prg[1] = prg_size_8K - 3;
	prg[2] = prg_size_8K - 2;
	prg[3] = prg_size_8K - 1;

	chr[0] = 0;
	chr[1] = 1;
	chr[2] = 2;
	chr[3] = 3;
	chr[4] = 4;
	chr[5] = 5;
	chr[6] = 6;
	chr[7] = 7;

	M006Reset();

	SetReadHandler(0x5000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x7FFF, CartBW);

	SetReadHandler (0x4500, 0x4500, M006ReadREG);
	SetWriteHandler(0x4024, 0x451B, M006WriteREG);
	SetWriteHandler(0x8000, 0xFFFF, M006WriteLatch);

	SetTrainer();
}

static void ClockSMCCounter(void) {
	if (smc_irq_enabled) {
		smc_irq_counter++;
		if (smc_irq_counter >= 0x10000 ) {
			smc_irq_counter = 0;
			smc_irq_enabled = FALSE;
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M006CPUHook(int a) {
	while (a--) {
		fds_irq_counter += 3;
		while ((fds_irq_counter >= 448) && (fds_control & 0x80)) {
			X6502_IRQBegin(FCEU_IQEXT);
			fds_irq_counter -= 448;
		}

		if (!(mode_smc & 0x08)) {
			ClockSMCCounter();
		}
	}
}

static void M006HBHook(void) {
	if (mode_smc & 0x08) {
		int i;
		for (i = 0; i < 8; i++) {
			ClockSMCCounter();
		}
	}
}

static void M006PPUHook(uint32 A) {
	if ((A & 0x3000) != 0x2000) {
		int needsync = FALSE;

		if ((mode_1m >= 0xA0) && !(mode_1m & 0x01)) {
			chr_lock = (mode_1m & 0x10) != 0;
			needsync = TRUE;
		}

		if ((mode_smc & 0x05) == 0x01) {
			uint8 value = (A >> 4) & 0x02;
			uint8 bank  = (A >> 12) & 0x01;

			switch (A & 0x0FF0) {
			case 0x0FD0:
			case 0x0FE0:
				if (chr_mmc4latch[bank] != value) {
					chr_mmc4latch[bank] = value;
					needsync = TRUE;
				}
				break;
			}
		}
		if (needsync) {
			Sync();
		}
	}
}

static void Close(void) {
}

static void StateRestore(int version) {
	Sync();
}

void Mapper006_Init(CartInfo *info) {
	int ws = info->PRGRamSize + info->PRGRamSaveSize;

	info->Power = M006Power;
	info->Reset = M006Reset;

	MapIRQHook = M006CPUHook;
	PPU_hook = M006PPUHook;
	GameHBIRQHook2 = M006HBHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);

	WRAMSIZE = ws ? ws : 8192;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_malloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, TRUE);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
	}

	SetupCartPRGMapping(0x11, scratch, 4096, TRUE);
	AddExState(scratch, 4096, 0, "SRAM");

	prg_size_8K = PRG_BANK_COUNT(8);

	if (CHR_ROM_SIZE) {
		if (info->mapper == 12) {
			int i;
			size_t ssize, prgsize;

			prgsize = 512 * 1024;
			PRG_ROM_PTR = realloc(PRG_ROM_PTR, prgsize);
			for (i = 0; i < ((CHR_ROM_SIZE < (256 * 1024)) ? CHR_ROM_SIZE : (256 * 1024)); i++) {
				PRG_ROM_PTR[(256 * 1024) + i] = CHR_ROM_PTR[i];
			}
			SetupCartPRGMapping(0, PRG_ROM_PTR, prgsize, TRUE);

			memset(CHR_ROM_PTR, 0, CHR_ROM_SIZE);
			ssize = info->CHRRamSize ? info->CHRRamSize : (32 * 1024);
			SetupCartCHRMapping(0, CHR_ROM_PTR, ssize, TRUE);
			AddExState(CHR_ROM_PTR, ssize, 0, "CHRR");
			VROM_size = 0;
		} else {
			SetupCartCHRMapping(0, CHRptr[0], CHRsize[0], TRUE);
			AddExState(CHR_ROM_PTR, CHR_ROM_SIZE, 0, "CHRR");
		}
	}

	if ((iNESCart.iNES2 == 0) && (iNESCart.mapper == 6)) {
		iNESCart.submapper = 1;
	}

	if (iNESCart.mapper == 8) {
		iNESCart.mapper = 6;
		iNESCart.submapper = 4;
	}
}
