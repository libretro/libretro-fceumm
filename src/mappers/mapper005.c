/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

/* None of this code should use any of the iNES bank switching wrappers. */

#include "mapinc.h"
#include "mmc5sound.h"

static INLINE void MMC5SPRVROM_BANK1(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask1[0];
		MMC5SPRVPage[A >> 10] = &CHRptr[0][V << 10] - A;
	}
}

static INLINE void MMC5BGVROM_BANK1(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask1[0]; MMC5BGVPage[A >> 10] = &CHRptr[0][V << 10] - A;
	}
}

static INLINE void MMC5SPRVROM_BANK2(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask2[0]; MMC5SPRVPage[A >> 10] = MMC5SPRVPage[(A >> 10) + 1] = &CHRptr[0][V << 11] - A;
	}
}

static INLINE void MMC5BGVROM_BANK2(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask2[0]; MMC5BGVPage[A >> 10] = MMC5BGVPage[(A >> 10) + 1] = &CHRptr[0][V << 11] - A;
	}
}

static INLINE void MMC5SPRVROM_BANK4(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask4[0]; MMC5SPRVPage[A >> 10] = MMC5SPRVPage[(A >> 10) + 1] = MMC5SPRVPage[(A >> 10) + 2] = MMC5SPRVPage[(A >> 10) + 3] = &CHRptr[0][V << 12] - A;
	}
}

static INLINE void MMC5BGVROM_BANK4(uint32 A, uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask4[0]; MMC5BGVPage[A >> 10] = MMC5BGVPage[(A >> 10) + 1] = MMC5BGVPage[(A >> 10) + 2] = MMC5BGVPage[(A >> 10) + 3] = &CHRptr[0][V << 12] - A;
	}
}

static INLINE void MMC5SPRVROM_BANK8(uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask8[0]; MMC5SPRVPage[0] = MMC5SPRVPage[1] = MMC5SPRVPage[2] = MMC5SPRVPage[3] = MMC5SPRVPage[4] = MMC5SPRVPage[5] = MMC5SPRVPage[6] = MMC5SPRVPage[7] = &CHRptr[0][V << 13];
	}
}

static INLINE void MMC5BGVROM_BANK8(uint32 V) {
	if (CHRptr[0]) {
		V &= CHRmask8[0]; MMC5BGVPage[0] = MMC5BGVPage[1] = MMC5BGVPage[2] = MMC5BGVPage[3] = MMC5BGVPage[4] = MMC5BGVPage[5] = MMC5BGVPage[6] = MMC5BGVPage[7] = &CHRptr[0][V << 13];
	}
}

static uint8 PRGBanks[4];
static uint8 WRAMPage;
static uint16 CHRBanksA[8], CHRBanksB[4];
static uint8 WRAMMaskEnable[2];
uint8 mmc5ABMode;                /* A=0, B=1 */

static uint8 IRQScanline, IRQEnable;
static uint8 CHRMode, NTAMirroring, NTFill, ATFill;

static uint8 MMC5IRQR;
static uint8 MMC5LineCounter;
static uint8 mmc5psize, mmc5vsize;
static uint8 mul[2];

static uint8 *MMC5fill = NULL;
static uint8 *ExRAM = NULL;
static uint8 MMC5battery = 0;

static uint8 MMC5WRAMsize; /* configuration, not state */
static uint8 MMC5WRAMIndex[8]; /* configuration, not state */

static uint8 MMC5ROMWrProtect[4];
static uint8 MMC5MemIn[5];

static SFORMAT MMC5_StateRegs[] = {
	{ PRGBanks, 4, "PREG" },
	{ CHRBanksA, 16, "CHRA" },
	{ CHRBanksB, 8, "CHRB" },
	{ &WRAMPage, 1, "WRMP" },
	{ WRAMMaskEnable, 2, "WRMK" },
	{ &mmc5ABMode, 1, "ABMD" },
	{ &IRQScanline, 1, "IRQS" },
	{ &IRQEnable, 1, "IRQE" },
	{ &CHRMode, 1, "CHRM" },
	{ &NTAMirroring, 1, "NTAM" },
	{ &NTFill, 1, "NTFL" },
	{ &ATFill, 1, "ATFL" },
	{ &MMC5IRQR, 1, "IRQR" },
	{ &MMC5LineCounter, 1, "LCTR" },
	{ &mmc5psize, 1, "PSIZ" },
	{ &mmc5vsize, 1, "VSIZ" },
	{ mul, 2, "MUL0" },
	{ MMC5ROMWrProtect, 4, "PROT" },
	{ MMC5MemIn, 5, "MEMN" },

	{ 0 }
};

static void MMC5CHRA(void);
static void MMC5CHRB(void);

static void BuildWRAMSizeTable(void) {
	int x;
	for (x = 0; x < 8; x++) {
		switch (MMC5WRAMsize) {
		case 0: MMC5WRAMIndex[x] = 255; break;                      /* X,X,X,X,X,X,X,X */
		case 1: MMC5WRAMIndex[x] = (x > 3) ? 255 : 0; break;        /* 0,0,0,0,X,X,X,X */
		case 2: MMC5WRAMIndex[x] = (x & 4) >> 2; break;             /* 0,0,0,0,1,1,1,1 */
		case 4: MMC5WRAMIndex[x] = (x > 3) ? 255 : (x & 3); break;  /* 0,1,2,3,X,X,X,X */
		case 8: MMC5WRAMIndex[x] = x; break; 						/* 0,1,2,3,4,5,6,7 */
		}
	}
}

static void MMC5CHRA(void) {
	int x;
	switch (mmc5vsize & 3) {
	case 0:
		setchr8(CHRBanksA[7]);
		MMC5SPRVROM_BANK8(CHRBanksA[7]);
		break;
	case 1:
		setchr4(0x0000, CHRBanksA[3]);
		setchr4(0x1000, CHRBanksA[7]);
		MMC5SPRVROM_BANK4(0x0000, CHRBanksA[3]);
		MMC5SPRVROM_BANK4(0x1000, CHRBanksA[7]);
		break;
	case 2:
		setchr2(0x0000, CHRBanksA[1]);
		setchr2(0x0800, CHRBanksA[3]);
		setchr2(0x1000, CHRBanksA[5]);
		setchr2(0x1800, CHRBanksA[7]);
		MMC5SPRVROM_BANK2(0x0000, CHRBanksA[1]);
		MMC5SPRVROM_BANK2(0x0800, CHRBanksA[3]);
		MMC5SPRVROM_BANK2(0x1000, CHRBanksA[5]);
		MMC5SPRVROM_BANK2(0x1800, CHRBanksA[7]);
		break;
	case 3:
		for (x = 0; x < 8; x++) {
			setchr1(x << 10, CHRBanksA[x]);
			MMC5SPRVROM_BANK1(x << 10, CHRBanksA[x]);
		}
		break;
	}
}

static void MMC5CHRB(void) {
	int x;
	switch (mmc5vsize & 3) {
	case 0:
		setchr8(CHRBanksB[3]);
		MMC5BGVROM_BANK8(CHRBanksB[3]);
		break;
	case 1:
		setchr4(0x0000, CHRBanksB[3]);
		setchr4(0x1000, CHRBanksB[3]);
		MMC5BGVROM_BANK4(0x0000, CHRBanksB[3]);
		MMC5BGVROM_BANK4(0x1000, CHRBanksB[3]);
		break;
	case 2:
		setchr2(0x0000, CHRBanksB[1]);
		setchr2(0x0800, CHRBanksB[3]);
		setchr2(0x1000, CHRBanksB[1]);
		setchr2(0x1800, CHRBanksB[3]);
		MMC5BGVROM_BANK2(0x0000, CHRBanksB[1]);
		MMC5BGVROM_BANK2(0x0800, CHRBanksB[3]);
		MMC5BGVROM_BANK2(0x1000, CHRBanksB[1]);
		MMC5BGVROM_BANK2(0x1800, CHRBanksB[3]);
		break;
	case 3:
		for (x = 0; x < 8; x++) {
			setchr1(x << 10, CHRBanksB[x & 3]);
			MMC5BGVROM_BANK1(x << 10, CHRBanksB[x & 3]);
		}
		break;
	}
}

static void MMC5WRAM(uint32 A, uint32 V) {
	V = MMC5WRAMIndex[V & 7];
	if (V != 255) {
		setprg8r(0x10, A, V);
		FCEU_CheatAddRAM(8, 0x6000, (WRAM + ((V * 8192) & (WRAMSIZE - 1))));
		MMC5MemIn[(A - 0x6000) >> 13] = 1;
	} else {
		MMC5MemIn[(A - 0x6000) >> 13] = 0;
	}
}

static void MMC5PRG(void) {
	int x;
	switch (mmc5psize & 3) {
	case 0:
		MMC5ROMWrProtect[0] = MMC5ROMWrProtect[1] = MMC5ROMWrProtect[2] = MMC5ROMWrProtect[3] = 1;
		setprg32(0x8000, ((PRGBanks[3] & 0x7F) >> 2));
		for (x = 0; x < 4; x++) {
			MMC5MemIn[1 + x] = 1;
		}
		break;
	case 1:
		if (PRGBanks[1] & 0x80) {
			MMC5ROMWrProtect[0] = MMC5ROMWrProtect[1] = 1;
			setprg16(0x8000, (PRGBanks[1] >> 1));
			MMC5MemIn[1] = MMC5MemIn[2] = 1;
		} else {
			MMC5ROMWrProtect[0] = MMC5ROMWrProtect[1] = 0;
			MMC5WRAM(0x8000, PRGBanks[1] & 7 & 0xFE);
			MMC5WRAM(0xA000, (PRGBanks[1] & 7 & 0xFE) + 1);
		}
		MMC5MemIn[3] = MMC5MemIn[4] = 1;
		MMC5ROMWrProtect[2] = MMC5ROMWrProtect[3] = 1;
		setprg16(0xC000, (PRGBanks[3] & 0x7F) >> 1);
		break;
	case 2:
		if (PRGBanks[1] & 0x80) {
			MMC5MemIn[1] = MMC5MemIn[2] = 1;
			MMC5ROMWrProtect[0] = MMC5ROMWrProtect[1] = 1;
			setprg16(0x8000, (PRGBanks[1] & 0x7F) >> 1);
		} else {
			MMC5ROMWrProtect[0] = MMC5ROMWrProtect[1] = 0;
			MMC5WRAM(0x8000, PRGBanks[1] & 7 & 0xFE);
			MMC5WRAM(0xA000, (PRGBanks[1] & 7 & 0xFE) + 1);
		}
		if (PRGBanks[2] & 0x80) {
			MMC5ROMWrProtect[2] = 1;
			MMC5MemIn[3] = 1;
			setprg8(0xC000, PRGBanks[2] & 0x7F);
		} else {
			MMC5ROMWrProtect[2] = 0;
			MMC5WRAM(0xC000, PRGBanks[2] & 7);
		}
		MMC5MemIn[4] = 1;
		MMC5ROMWrProtect[3] = 1;
		setprg8(0xE000, PRGBanks[3] & 0x7F);
		break;
	case 3:
		for (x = 0; x < 3; x++) {
			if (PRGBanks[x] & 0x80) {
				MMC5ROMWrProtect[x] = 1;
				setprg8(0x8000 + (x << 13), PRGBanks[x] & 0x7F);
				MMC5MemIn[1 + x] = 1;
			} else {
				MMC5ROMWrProtect[x] = 0;
				MMC5WRAM(0x8000 + (x << 13), PRGBanks[x] & 7);
			}
		}
		MMC5MemIn[4] = 1;
		MMC5ROMWrProtect[3] = 1;
		setprg8(0xE000, PRGBanks[3] & 0x7F);
		break;
	}
}

static void MMC5MIRR(void) {
	int x;
	for (x = 0; x < 4; x++) {
		switch ((NTAMirroring >> (x << 1)) & 3) {
		case 0: PPUNTARAM |= 1 << x; vnapage[x] = NTARAM; break;
		case 1: PPUNTARAM |= 1 << x; vnapage[x] = NTARAM + 0x400; break;
		case 2: PPUNTARAM |= 1 << x; vnapage[x] = ExRAM; break;
		case 3: PPUNTARAM &= ~(1 << x); vnapage[x] = MMC5fill; break;
		}
	}
}

DECLFW(Mapper5_write) {
	switch (A) {
	case 0x5100:
		mmc5psize = V;
		MMC5PRG();
		break;
	case 0x5101:
		mmc5vsize = V;
		if (!mmc5ABMode) {
			MMC5CHRB();
			MMC5CHRA();
		} else {
			MMC5CHRA();
			MMC5CHRB();
		}
		break;
	case 0x5102:
		WRAMMaskEnable[0] = V;
		break;
	case 0x5103:
		WRAMMaskEnable[1] = V;
		break;
	case 0x5104:
		CHRMode = V;
		MMC5HackCHRMode = V & 3;
		break;
	case 0x5105:
		NTAMirroring = V;
		MMC5MIRR();
		break;
	case 0x5106:
		if (V != NTFill) {
			FCEU_dwmemset(MMC5fill, (V | (V << 8) | (V << 16) | (V << 24)), 0x3c0);
		}
		NTFill = V;
		break;
	case 0x5107:
		if (V != ATFill) {
			unsigned char moop = V | (V << 2) | (V << 4) | (V << 6);
			FCEU_dwmemset(MMC5fill + 0x3c0, moop | (moop << 8) | (moop << 16) | (moop << 24), 0x40);
		}
		ATFill = V;
		break;
	case 0x5113:
		WRAMPage = V;
		MMC5WRAM(0x6000, V & 7);
		break;
	case 0x5114:
	case 0x5115:
	case 0x5116:
	case 0x5117:
		PRGBanks[A & 3] = V;
		MMC5PRG();
		break;
	case 0x5120:
	case 0x5121:
	case 0x5122:
	case 0x5123:
	case 0x5124:
	case 0x5125:
	case 0x5126:
	case 0x5127:
		mmc5ABMode = 0;
		CHRBanksA[A & 7] = V | ((MMC50x5130 & 0x3) << 8);
		MMC5CHRA();
		break;
	case 0x5128:
	case 0x5129:
	case 0x512a:
	case 0x512b:
		mmc5ABMode = 1;
		CHRBanksB[A & 3] = V | ((MMC50x5130 & 0x3) << 8);
		MMC5CHRB();
		break;
	case 0x5130:
		MMC50x5130 = V;
		break;
	case 0x5200:
		MMC5HackSPMode = V;
		break;
	case 0x5201:
		MMC5HackSPScroll = (V >> 3) & 0x1F;
		break;
	case 0x5202:
		MMC5HackSPPage = V & 0x3F;
		break;
	case 0x5203:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQScanline = V;
		break;
	case 0x5204:
		X6502_IRQEnd(FCEU_IQEXT);
		IRQEnable = V & 0x80;
		break;
	case 0x5205:
		mul[0] = V;
		break;
	case 0x5206:
		mul[1] = V;
		break;
	}
}

static DECLFR(MMC5_ReadROMRAM) {
	if (MMC5MemIn[(A - 0x6000) >> 13]) {
		return Page[A >> 11][A];
	}
	return cpu.openbus;
}

static DECLFW(MMC5_WriteROMRAM) {
	if ((A >= 0x8000) && (MMC5ROMWrProtect[(A - 0x8000) >> 13])) {
		return;
	}
	if (MMC5MemIn[(A - 0x6000) >> 13]) {
		if (((WRAMMaskEnable[0] & 3) | ((WRAMMaskEnable[1] & 3) << 2)) == 6) {
			Page[A >> 11][A] = V;
		}
	}
}

DECLFW(MMC5_ExRAMWr) {
	if (MMC5HackCHRMode != 3) {
		ExRAM[A & 0x3ff] = V;
	}
}

static DECLFR(MMC5_ExRAMRd) {
	return ExRAM[A & 0x3ff];
}

static DECLFR(MMC5_read) {
	uint8 ret = cpu.openbus;
	switch (A) {
	case 0x5204:
		X6502_IRQEnd(FCEU_IQEXT);
		ret = MMC5IRQR;
#ifdef FCEUDEF_DEBUGGER
		if (!fceuindbg)
#endif
			MMC5IRQR &= 0x40;
		return ret;
	case 0x5205:
		return ((uint32)(mul[0] * mul[1]) & 0xFF);
	case 0x5206:
		return ((uint32)(mul[0] * mul[1]) >> 8);
	}
	return ret;
}

static void MMC5Synco(void) {
	uint8 moop;

	MMC5PRG();
	MMC5MIRR();
	MMC5WRAM(0x6000, WRAMPage & 7);
	if (!mmc5ABMode) {
		MMC5CHRB();
		MMC5CHRA();
	} else {
		MMC5CHRA();
		MMC5CHRB();
	}

	/* in case the fill register changed, we need to overwrite the fill buffer */
	FCEU_dwmemset(MMC5fill, NTFill | (NTFill << 8) | (NTFill << 16) | (NTFill << 24), 0x3c0);
	moop = ATFill | (ATFill << 2) | (ATFill << 4) | (ATFill << 6);
	FCEU_dwmemset(MMC5fill + 0x3c0, moop | (moop << 8) | (moop << 16) | (moop << 24), 0x40);

	MMC5HackCHRMode = CHRMode & 3;

	/* zero 17-apr-2013 - why the heck should this happen here? anything in a `synco` should be depending on the state.
	 * im going to leave it commented out to see what happens
	 */
	 /* X6502_IRQEnd(FCEU_IQEXT); */
}

void MMC5_hb(int cur_scanline) {
	/* zero 24-jul-2014 - revised for newer understanding, to fix metal slader glory credits. see r7371 in bizhawk */
	int sl = cur_scanline + 1;
	int ppuon = (PPU[1] & 0x18);

	if (!ppuon || sl >= 241) {
		/* whenever rendering is off for any reason (vblank or forced disable
		 * the irq counter resets, as well as the inframe flag (easily verifiable from software)
		 */
		MMC5IRQR &= ~0x40;
		MMC5IRQR &= ~0x80;
		MMC5LineCounter = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		return;
	}

	if (!(MMC5IRQR & 0x40)) {
		MMC5IRQR |= 0x40;
		MMC5IRQR &= ~0x80;
		MMC5LineCounter = 0;
		X6502_IRQEnd(FCEU_IQEXT);
	} else {
		MMC5LineCounter++;
		if (MMC5LineCounter == IRQScanline) {
			MMC5IRQR |= 0x80;
			if (IRQEnable & 0x80) {
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void MMC5_StateRestore(int version) {
	MMC5Synco();
}

static void GenMMC5Power(void) {
	int x;
	uint8 nval;
	uint8 aval;

	for (x = 0; x < 4; x++) PRGBanks[x] = ~0;
	for (x = 0; x < 8; x++) CHRBanksA[x] = ~0;
	for (x = 0; x < 4; x++) CHRBanksB[x] = ~0;
	for (x = 0; x < 2; x++) WRAMMaskEnable[x] = ~0;
	for (x = 0; x < 4; x++) MMC5ROMWrProtect[x] = 0;
	for (x = 0; x < 5; x++) MMC5MemIn[x] = 0;
	for (x = 0; x < 2; x++) mul[x] = 0;

	WRAMPage = 0;
	CHRMode = 0;
	mmc5ABMode = 0;
	mmc5psize = mmc5vsize = 3;

	IRQScanline = 0;
	IRQEnable = 0;
	MMC5IRQR = 0;
	MMC5LineCounter = 0;

	NTAMirroring = NTFill = ATFill = 0xFF;

	/* MMC5fill is and 8-bit tile index, and a 2-bit attribute implented as a mirrored nametable */
	nval = MMC5fill[0x000];
	aval = MMC5fill[0x3C0] & 3; aval = aval | (aval << 2) | (aval << 4) | (aval << 6);
	FCEU_dwmemset(MMC5fill + 0x000, nval | (nval<<8) | (nval<<16) | (nval<<24), 0x3C0);
	FCEU_dwmemset(MMC5fill + 0x3C0, aval | (aval<<8) | (aval<<16) | (aval<<24), 0x040);

	if (MMC5battery == 0) {
		FCEU_MemoryRand(WRAM, MMC5WRAMsize * 8 * 1024);
		FCEU_MemoryRand(MMC5fill, 1024);
		FCEU_MemoryRand(ExRAM, 1024);
	}

	MMC5Synco();

	SetWriteHandler(0x4020, 0x5bff, Mapper5_write);
	SetReadHandler(0x4020, 0x5bff, MMC5_read);

	SetWriteHandler(0x5c00, 0x5fff, MMC5_ExRAMWr);
	SetReadHandler(0x5c00, 0x5fff, MMC5_ExRAMRd);

	SetWriteHandler(0x6000, 0xFFFF, MMC5_WriteROMRAM);
	SetReadHandler(0x6000, 0xFFFF, MMC5_ReadROMRAM);

	SetWriteHandler(0x5000, 0x5015, MMC5Sound_Write);
	SetWriteHandler(0x5205, 0x5206, Mapper5_write);
	SetReadHandler(0x5205, 0x5206, MMC5_read);

/*	GameHBIRQHook=MMC5_hb; */
/*	FCEU_CheatAddRAM(8, 0x6000, WRAM); */
	FCEU_CheatAddRAM(1, 0x5c00, ExRAM);
}

static void GenMMC5_Init(CartInfo *info, int wsize, int battery) {
	if (wsize) {
		WRAM = (uint8 *)FCEU_gmalloc(wsize * 1024);
		SetupCartPRGMapping(0x10, WRAM, wsize * 1024, 1);
		AddExState(WRAM, wsize * 1024, 0, "WRAM");
	}

	MMC5fill = (uint8 *)FCEU_gmalloc(1024);
	ExRAM = (uint8 *)FCEU_gmalloc(1024);

	FCEU_MemoryRand(MMC5fill, 1024);
	FCEU_MemoryRand(ExRAM, 1024);

	AddExState(ExRAM, 1024, 0, "ERAM");
	AddExState(&MMC5HackSPMode, 1, 0, "SPLM");
	AddExState(&MMC5HackSPScroll, 1, 0, "SPLS");
	AddExState(&MMC5HackSPPage, 1, 0, "SPLP");
	AddExState(&MMC50x5130, 1, 0, "5130");
	AddExState(MMC5_StateRegs, ~0, 0, 0);

	MMC5WRAMsize = wsize ? (wsize / 8) : 0;
	BuildWRAMSizeTable();
	GameStateRestore = MMC5_StateRestore;
	info->Power = GenMMC5Power;

	MMC5battery = battery;
	if (battery) {
		uint32 saveramsize;
		if (info->iNES2) {
			saveramsize = info->PRGRamSaveSize;
		} else {
			if (wsize <= 16) saveramsize = 8 * 1024;
			else if (wsize >= 64) saveramsize = 64 * 1024;
			else saveramsize = 32 * 1024;
		}
		info->SaveGame[0] = WRAM;
		info->SaveGameLen[0] = saveramsize;
	}

	MMC5HackVROMMask = CHRmask4[0];
	MMC5HackExNTARAMPtr = ExRAM;
	MMC5Hack = TRUE;
	MMC5HackVROMPTR = CHRptr[0];
	MMC5HackCHRMode = 0;
	MMC5HackSPMode = MMC5HackSPScroll = MMC5HackSPPage = 0;

	MMC5Sound_ESI();
	MMC5Sound_AddStateInfo();
}

void Mapper005_Init(CartInfo *info) {
	WRAMSIZE = 64;
	if (info->iNES2) {
		WRAMSIZE = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;
		if (WRAMSIZE && WRAMSIZE < 8) WRAMSIZE = 8;
		else if (WRAMSIZE > 64) WRAMSIZE = 64;
	}
	GenMMC5_Init(info, WRAMSIZE, info->battery);
}

/* ELROM seems to have 0KB of WRAM
 * EKROM seems to have 8KB of WRAM, battery-backed
 * ETROM seems to have 16KB of WRAM, battery-backed
 * EWROM seems to have 32KB of WRAM, battery-backed
 */

void ELROM_Init(CartInfo *info) {
	GenMMC5_Init(info, 0, 0);
}

void EKROM_Init(CartInfo *info) {
	GenMMC5_Init(info, 8, info->battery);
}

void ETROM_Init(CartInfo *info) {
	GenMMC5_Init(info, 16, info->battery);
}

void EWROM_Init(CartInfo *info) {
	GenMMC5_Init(info, 32, info->battery);
}
