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
 
/* Extracts from early floppy-disk-based RAM Cartridges ("copiers").
   - Front Far East Super Magic Card, which is backwards-compatible to Magic Card 1M, Magic Card 2M, Magic Card 4M. Mapper 6/8/12.1/17
   - Bung Super Game Doctor 4M, which is backwards-compatible to Super Game Doctor 2M and Super Game Doctor 1M. Mapper 561
   - Venus Turbo Game Doctor 6M, which is backwards-comaptible to Turbo Game Doctor 6+, Turbo Game Doctor 4+, and Venus Game Converter. Mapper 562
   Cross-compatibility between the devices is spotty and complicated. Generally, games have checks to lock out competing devices, while devices try to run as many competitor games as possible.
*/
#include "mapinc.h"

#define FFE 0
#define BUNG 1
#define VENUS 2

static uint8 maker;
static uint8 mc1Mode, mc1ModeInitial; /* register 42Fx */
static uint8 mc2Mode, mc2ModeInitial; /* register 43Fx */
static uint8 extMode, extModeInitial; /* register 4500 on FFE, 4411 on Venus, does not exist on Bung */
static uint8 latch;		/* 8000-FFFF register for 1M modes */
static uint8 lockCHR;		/* CHR-RAM write-protected via undocumented protection mechanism */
static uint8 prg8K[4];		/* 8 KiB PRG registers in 2M or 4M mode */
static uint8 chr1K[12];		/* Not used by Bung, 8 registers by Venus, 12 registers by FFE */
static uint8 chr8K;		/* Not used by FFE, only by Bung and Venus */
static uint8 lastCHRBank;	/* Only used by Venus */

/* IRQ counter used by FFE and Bung */
static signed short int irqCounter;

/* IRQ counter used by Venus */
static unsigned short int tgdCounter, tgdTarget;

/* FFE-only registers */
static uint8 latchMMC4[2];	/* For MMC4 emulation mode */
static uint8 smcIRQ;		/* IRQ enable register */

/* FDS data IRQ, used for frame timing purposes */
static uint8 fdsIO;
static signed short int fdsCounter;

static uint8 *WRAM =NULL;	/* 0000-1FFF: CPU 6000-7FFF; 2000-2FFF: CPU 5000-5FFF (FFE only) */
static uint8 *CHRRAM =NULL;	/* up to 32 KiB for Bung, up to 256 KiB for FFE and Venus */

static uint32 trainerSize;
static uint8 *trainerSource, *trainerTarget;
static uint16 trainerInit, resetAddress;

/* For trapping writes to CHR-RAM, which can trigger a protection mechanism in FFE and Venus devices */
extern uint32 RefreshAddr;
static writefunc writePPU;
static DECLFW(interceptPPUWrite) { /* Only called in modes 5-7 with single-screen mirroring. Then, Writing anything to CHR-RAM locks (CIRAM page 1) or unlocks (CIRAM page 0) CHR memory in modes 0-3. Needed for (F4040) Karnov). */
	if (~RefreshAddr &0x2000) lockCHR =!!(mc1Mode &0x10);
	writePPU(A, V);
}

static SFORMAT stateRegsFFE[] ={
	{ &mc1Mode, 1, "MC1M" },
	{ &mc2Mode, 1, "MC2M" },
	{ &extMode, 1, "EXTM" },
	{ &latch, 1, "LATC" },
	{ &lockCHR, 1, "LOCK" },
	{ prg8K, 4, "PRG8" },
	{ chr1K, 12, "CHR1" },
	{ &irqCounter, 2 | FCEUSTATE_RLSB, "IRQC" },
	{ latchMMC4, 2, "MMC4" },
	{ &smcIRQ, 1, "IRQA" },
	{ &fdsIO, 1, "FDSI" },
	{ &fdsCounter, 2 | FCEUSTATE_RLSB, "FDSC" },
	{ 0 }
};
static SFORMAT stateRegsBung[] ={
	{ &mc1Mode, 1, "MC1M" },
	{ &mc2Mode, 1, "MC2M" },
	{ &latch, 1, "LATC" },
	{ prg8K, 4, "PRG8" },
	{ &chr8K, 1, "CHR8" },
	{ &irqCounter, 2 | FCEUSTATE_RLSB, "IRQC" },
	{ &fdsIO, 1, "FDSI" },
	{ &fdsCounter, 2 | FCEUSTATE_RLSB, "FDSC" },
	{ 0 }
};
static SFORMAT stateRegsVenus[] ={
	{ &mc1Mode, 1, "MC1M" },
	{ &mc2Mode, 1, "MC2M" },
	{ &extMode, 1, "EXTM" },
	{ &latch, 1, "LATC" },
	{ &lockCHR, 1, "LOCK" },
	{ prg8K, 4, "PRG8" },
	{ chr1K, 8, "CHR1" },
	{ &chr8K, 1, "CHR8" },
	{ &lastCHRBank, 1, "LACB" },
	{ &tgdCounter, 2 | FCEUSTATE_RLSB, "IRQC" },
	{ &tgdTarget, 2 | FCEUSTATE_RLSB, "IRQT" },
	{ &fdsIO, 1, "FDSI" },
	{ &fdsCounter, 2 | FCEUSTATE_RLSB, "FDSC" },
	{ 0 }
};

static void sync () {
	if (maker ==FFE) setprg4r(0x10, 0x5000, 2); /* Only FFE has further 4 KiB of PRG-RAM at 5000-5FFF */
	setprg8r(0x10, 0x6000, 0); /* Both FFE and Venus actually have 32 KiB of bankswitched PRG-RAM, but the other 24 KiB are not available to games, only to BIOS for various purposes, and are not emulated here */

	/* Write-enable "PRG-ROM" (a.k.a. PRG-DRAM) if the PRG write-protect bit is not set */
	SetupCartPRGMapping(0, PRGptr[0], PRGsize[0], mc1Mode &0x02? 0: 1);
	if (~mc2Mode &0x01 && maker !=VENUS || extMode &0x80 && maker ==VENUS) { /* unrestricted 4M PRG mode on all devices */
		setprg8(0x8000, prg8K[0]);
		setprg8(0xA000, prg8K[1]);
		setprg8(0xC000, prg8K[2]);
		setprg8(0xE000, prg8K[3]);
	} else
	if (~mc2Mode &0x01 && maker ==VENUS) { /* On Venus, what enables 4M mode on Bung is an address-restricted 2M mode */
		setprg8(0x8000, prg8K[0] &0x0F | mc2Mode >>2 &0x10);
		setprg8(0xA000, prg8K[1] &0x0F | mc2Mode >>2 &0x10);
		setprg8(0xC000, prg8K[2] &0x0F);
		setprg8(0xE000, prg8K[3] &0x0F);
	} else /* 1M PRG mode (all devices) */
	switch(mc1Mode >>5) {
		case 0:	setprg16(0x8000, latch &7);
			setprg16(0xC000, 7);
			break;
		case 1:	setprg16(0x8000, latch >>2 &15);
			setprg16(0xC000, 7);
			break;
		case 2:	setprg16(0x8000, latch &15);
			setprg16(0xC000, 15);
			break;
		case 3:	setprg16(0x8000, 15);
			setprg16(0xC000, latch &15);
			break;
		case 4:	setprg32(0x8000, latch >>4 &3);
			break;
		case 5:	setprg32(0x8000, 3);
			break;
		case 6:	if (maker ==FFE) /* Modes 6 and 7 differ between FFE and Bung/enus */
				setprg32(0x8000, 3);
			else {
				setprg8 (0x8000, latch &0xF);
				setprg8 (0xA000, latch >>4);
				setprg16(0xC000, 7);
			}
			break;
		case 7:	if (maker ==FFE)
				setprg32(0x8000, 3);
			else {
				setprg8 (0x8000, latch &0xF &~1);
				setprg8 (0xA000, latch >>4  | 1);
				setprg16(0xC000, 7);
			}
			break;
	}

	/* CHR-RAM is write-protected on FFE ...
	   - ... if 1M mode GNROM with two-screen mirroring is active;
	   - ... if 1M mode (C)NROM is active;
	   - ... if CHR-RAM has been locked before. 
	   On Bung/Venus, it's simpler: GNROM or (C)NROM modes, or CHR-RAM lock on Venus only; Bung has no CHR-RAM lock. */
	if (maker ==FFE)		
		SetupCartCHRMapping(0x10, CHRRAM, CHRsize[0x10], (mc1Mode &0xE1) >=0x81 || lockCHR? 0: 1);
	else
		SetupCartCHRMapping(0x10, CHRRAM, CHRsize[0x10], mc1Mode >=0x80 || lockCHR? 0: 1);
	
	/* CHR-RAM banking */
	if (maker ==BUNG || maker ==VENUS && ~extMode &0x40)
		setchr8r(0x10, chr8K); /* Bung and Venus maintain an independent 8 KiB bank number register that is updated from the latch at specific events */
	else
	if (extMode &0x01 || maker ==VENUS && extMode &0x40) { /* 1 KiB CHR-RAM modes */
		if (extMode &0x04 || maker ==VENUS) { /* For FFE, need to further distinguish between actual 1 KiB and 4 KiB MMC4-like mode */
			setchr1r(0x10, 0x0000, chr1K[0]);
			setchr1r(0x10, 0x0400, chr1K[1]);
			setchr1r(0x10, 0x0800, chr1K[2]);
			setchr1r(0x10, 0x0C00, chr1K[3]);
			setchr1r(0x10, 0x1000, chr1K[4]);
			setchr1r(0x10, 0x1400, chr1K[5]);
			setchr1r(0x10, 0x1800, chr1K[6]);
			setchr1r(0x10, 0x1C00, chr1K[7]);
		} else {
			setchr4r(0x10, 0x0000, chr1K[0 | latchMMC4[0]] >>2);
			setchr4r(0x10, 0x1000, chr1K[4 | latchMMC4[1]] >>2);
		}
	} else
	switch(mc1Mode >>5) { /* 8 KiB CHR-RAM banking mode on FFE only, which applies the latch directly without an extra 8 KiB CHR-RAM bank register */
		case 0: case 2:
			setchr8r(0x10, 0);
			break;
		case 1: case 4: case 5:
			setchr8r(0x10, latch &3);
			break;
		case 3:
			setchr8r(0x10, latch >>4 &3);
			break;
		case 6:
			setchr8r(0x10, latch &1);
			break;
		case 7:
			setchr8r(0x10, 3);
			break;
	}
	
	/* Nametable arrangement. FFE can map CHR-RAM to nametable address space; Bung/Venus only can do normal CIRAM arrangement. */
	if (maker !=FFE || extMode &0x02)  /* CIRAM nametables */
		setmirror(mc1Mode &0x01? (mc1Mode &0x10? MI_H: MI_V): (mc1Mode &0x10? MI_1: MI_0));
	else {              /* CHR-RAM nametables */
		setntamem(CHRRAM +0x0400 *chr1K[8], 0, 0);
		setntamem(CHRRAM +0x0400 *chr1K[9], 0, 1);
		setntamem(CHRRAM +0x0400 *chr1K[10],0, 2);
		setntamem(CHRRAM +0x0400 *chr1K[11],0, 3);
	}
	
	/* Intercept CHR-RAM writes for undocument CHR-RAM locking mechanism, only on FFE and Venus. */
	if (maker !=BUNG) SetWriteHandler(0x2007, 0x2007, mc1Mode >=0xA0 && ~mc1Mode &1? interceptPPUWrite: writePPU);
}


/* Some Venus games check values in BIOS at 4800-4FFF to lock out Bung and FFE devices. Do not need to include the full BIOS, just a selection of values. */
static const uint8 tgdBIOSExtract[32] ={
	0xfe, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x4c, 0xd9, 0x48, 0x4c, 0x18, 0x49, 0x4c,
	0x23, 0x4f, 0x4c, 0x3f, 0x4f, 0x4c, 0x78, 0x4f
};
static DECLFR(readTGDBIOS) {
	return tgdBIOSExtract[A &0x1F];
}

static DECLFR(readReg) {
	if (maker ==VENUS) switch(A) {
		case 0x4400: case 0x4401: case 0x4402: case 0x4403: case 0x4404: case 0x4405: case 0x4406: case 0x4407:
			return chr1K[A &7];
		case 0x4408: case 0x4409: case 0x440A: case 0x440B:
			return prg8K[A &3] <<2 | latch &3;
		case 0x440C:
			return tgdCounter >>8;
		case 0x440D:
			return tgdCounter &0xFF;
		case 0x4411:
			return extMode;
		case 0x4415:
			return mc1Mode;
		case 0x4420: /* Venus allows reading the last accessed CHR-RAM bank register, used by some Venus games to lock out Bung and FFE devices. */
			return chr1K[lastCHRBank &7];
	} else
	if (maker ==FFE) switch(A) {
		case 0x4500:
			return extMode;
		default:
			return A >>8;
	} else
		return A >>8; /* Open Bus */
}

static DECLFW(writeReg) {
	switch (A) { /* All makers */
		case 0x4024:
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 0x4025:
			X6502_IRQEnd(FCEU_IQEXT);
			if (!smcIRQ) {
				fdsIO =V;
				if (V &0x42) fdsCounter =0;
			}
			break;
		case 0x42FC: case 0x42FD: case 0x42FE: case 0x42FF:
			mc1Mode =V &0xF0 | A &0x03;
			if (mc1Mode >=0x80) lockCHR =0; /* Setting modes 4-7 releases the CHR-RAM lock */
			sync();
			break;
		case 0x43FC: case 0x43FD: case 0x43FE: case 0x43FF:
			mc2Mode =V &0xF0 | A &0x03;
			latch =V;
			chr8K =V &3;
			sync();
			break;
	}
	
	if (maker ==BUNG) switch(A) {
		case 0x4100:
			irqCounter =(irqCounter &~0x00FF) | V;
			X6502_IRQEnd(FCEU_IQEXT);
			if (V ==0) irqCounter =0;
			break;
		case 0x4101:
			irqCounter =(irqCounter &~0xFF00) | V <<8;
			X6502_IRQEnd(FCEU_IQEXT);
			break;
	} else
	if (maker ==VENUS) switch(A) {
		case 0x4400: case 0x4401: case 0x4402: case 0x4403: case 0x4404: case 0x4405: case 0x4406: case 0x4407:
			chr1K[A &0x07] =V;
			sync();
			break;		
		case 0x440C:
			X6502_IRQEnd(FCEU_IQEXT);
			if (~V &0x80) tgdCounter =0x8000;
			tgdTarget =tgdTarget &0x00FF | V <<8;
			break;
		case 0x440D:
			X6502_IRQEnd(FCEU_IQEXT);
			tgdTarget =tgdTarget &0xFF00 | V;
			break;
		case 0x4411:
			extMode =V;
			sync();
			break;
	} else
	if (maker ==FFE) {
		switch(A) {
			case 0x4500:
				extMode =V;
				sync();
				break;
			case 0x4501:
				smcIRQ =0;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 0x4502:
				irqCounter =irqCounter &0xFF00 | V;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 0x4503:
				smcIRQ =true;
				irqCounter =irqCounter &0x00FF | V <<8 ;
				X6502_IRQEnd(FCEU_IQEXT);
				break;
			case 0x4504: case 0x4505: case 0x4506: case 0x4507:
				if (mc2Mode &0x02) V >>=2; /* The two bottom bits are unused when 4M mode is not active */
				prg8K[A &3] =V;
				sync();
				break;
			case 0x4510: case 0x4511: case 0x4512: case 0x4513: case 0x4514: case 0x4515: case 0x4516: case 0x4517: case 0x4518: case 0x4519: case 0x451A: case 0x451B:
				chr1K[A &0x0F] =V;
				sync();
				break;
		}
		if (A >=0x4500 && A <=0x451F) WRAM[0x2500 |A &0x1F] =V; /* SMC register content is cached at WRAM $5500-$551F */
	}
}

static DECLFW(writeLatch) {
	if (mc1Mode &0x02) {
		latch =V;
		if (maker !=FFE || mc2Mode &0x03) prg8K[A >>13 &3] =V >>2; /* The two bottom bits are unused when 4M mode is not active */
		if (maker !=FFE) switch(mc1Mode >>5) {
			case 1: case 4: case 5:
				chr8K =latch &3;
				break;
			case 3:
				chr8K =latch >>4 &3;
				break;
		}
		sync();
	} else
		CartBW(A, V);
}

/* Read handler for FFFC and FFFD so that in the presence of a trainer, the trainer's Init address is called before the game's reset handler on power-on reset.
   If the trainer's Init routine requires being called via JSR, resetAddress will be a routine in RAM at 0500 consisting of JSR trainerInit, JMP ($FFFC), so that address is returned from FFFC instead.
   If the trainer's Init routine requires being JMPed to, resetAddress will be trainerInit itself.
   After FFFD has been read, restore the original CartBR read handler, because on a soft reset, trainerInit should not be called. */
static DECLFR(redirectReset) {
	if (A ==0xFFFC)
		return resetAddress &0xFF;
	else {
		SetReadHandler(0xFFFC, 0xFFFD, CartBR);
		return resetAddress >>8;
	}
}

static void power(void) {
	int i;
	writePPU =GetWriteHandler(0x2007);
	GetWriteHandler(0x4017)(0x4017, 0x40); /* Disable Frame IRQ */
	mc1Mode =mc1ModeInitial;
	mc2Mode =mc2ModeInitial;
	extMode =extModeInitial;
	lockCHR =latch =chr8K =lastCHRBank =0;
	for (i =0; i <4; i++) prg8K[3 -i] =(maker ==FFE? ROM_size *2: 0x20) -i -1;
	for (i =0; i <8; i++) chr1K[i] =i;
	for (i =8; i<12; i++) chr1K[i] =i &7;
	smcIRQ =0;
	irqCounter =0;
	fdsIO =0;
	fdsCounter =0;
	tgdCounter =0xFFFF;
	tgdTarget =0;
	sync();

	SetReadHandler(0x4020, 0x47FF, readReg);
	if (maker ==VENUS) SetReadHandler(0x4800, 0x4FFF, readTGDBIOS);
	SetWriteHandler(0x4020, 0x47FF, writeReg);
	
	SetReadHandler(0x5000, 0x7FFF, CartBR);
	SetWriteHandler(0x5000, 0x7FFF, CartBW);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x8000, 0xFFFF, writeLatch);

	resetAddress =0;
	/* Initialize the trainer. Its position varies, which is why iNES_ExecPower's initialization is disabled for FFE, BUNG and VENUS mappers. */
	if (trainerSize) {
		for (i =0; i <trainerSize; i++) trainerTarget[i] =trainerSource[i];
		if (maker ==FFE && extModeInitial ==0x47)
			resetAddress =trainerInit;
		else
		if (trainerInit) {
			X6502_DMW(0x0500, 0x20);
			X6502_DMW(0x0501, trainerInit &0xFF);
			X6502_DMW(0x0502, trainerInit >>8);
			X6502_DMW(0x0503, 0x6C);
			X6502_DMW(0x0504, 0xFC);
			X6502_DMW(0x0505, 0xFF);
			resetAddress =0x0500;
		}
	}
	if (resetAddress) SetReadHandler(0xFFFC, 0xFFFD, redirectReset);	
	if (maker ==FFE) { /* For NARC (SM2006) and Best 競馬: Derby Stallion (SM4064), whose NMI vector point to the FFE BIOS' scratch RAM */
		WRAM[0x2032] =0x4C;
		WRAM[0x2033] =ROM[ROM_size *16384 -6 +0];
		WRAM[0x2034] =ROM[ROM_size *16384 -6 +1];
	}
}

static void clockFDSCounter () {
	fdsCounter +=3;
	while (fdsCounter >=448 && fdsIO &0x80) {
		X6502_IRQBegin(FCEU_IQEXT);
		fdsCounter -=448;
	}
}

static void clockSMCCounter () {
	if (smcIRQ) {
		if (irqCounter ==-1) {
			smcIRQ =0;
			X6502_IRQBegin(FCEU_IQEXT);
		} else
			irqCounter++;
	}
}

static void FP_FASTAPASS(1) cpuCycleFFE(int a) {
	while (a--) {
		clockFDSCounter();
		if (~extMode &0x08) clockSMCCounter();
	}
}

static void FP_FASTAPASS(1) cpuCycleBung(int a) {
	while (a--) {
		clockFDSCounter();
		if (irqCounter <0 && !++irqCounter) X6502_IRQBegin(FCEU_IQEXT);
	}
}

static void FP_FASTAPASS(1) cpuCycleVenus(int a) {
	while (a--) {
		clockFDSCounter();
		if (tgdTarget &0x8000) {
			if (tgdCounter ==tgdTarget && tgdCounter !=0xFFFF)
				X6502_IRQBegin(FCEU_IQEXT);
			else
				tgdCounter++;
		}
	}
}

static void pseudoScanlineFFE () {
	if (extMode &0x08) { /* Eight A12 rises per scanline */
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
		clockSMCCounter();
	}
}

static void FP_FASTAPASS(1) trapPPUAddressChangeFFE (uint32 A) {
	if (extMode &0x01 && ~extMode &0x04 && ((A &0x2FF0) ==0xFD0 || (A &0x2FF0) ==0xFE0))   {
		/* If MMC4 mode is enabled, and CHR mode[0] is 4 KiB, and tile FD or FE is being fetched ... */
		latchMMC4[A >>12 &1] =(A >>10 &4) | (A >>4 &2); /* ... switch the left or right pattern table's latch to 0 (FD) or 2 (FE), being used as an offset for the CHR register index. */
		sync();
	}
}

static void FP_FASTAPASS(1) trapPPUAddressChangeVenus (uint32 A) {
	if (A &~0x2000) lastCHRBank =A >>10;
}

static void close(void) {
	if (CHRRAM) {
		FCEU_gfree(CHRRAM);
		CHRRAM =NULL;
	}
	if (WRAM) {
		FCEU_gfree(WRAM);
		WRAM =NULL;
	}
}

void FFE_Init(CartInfo *info) {
	maker =FFE;
	AddExState(stateRegsFFE, ~0, 0, 0);
	if (!info->iNES2 && info->mapper ==6) info->submapper =1;
	if (info->mapper ==8) {
		info->mapper =6;
		info->submapper =4;
	}
	mc1ModeInitial =(info->mapper ==6? info->submapper: 1) <<5 | (info->mirror &1? 0x01: 0x11) | 0x02;
	mc2ModeInitial =info->mapper ==12 || info->mapper ==17? 0x00: 0x03;
	extModeInitial =info->mapper ==17? 0x47: 0x42;
	
	WRAM =(uint8*)FCEU_gmalloc(16384); /* Just 12 KiB actually, but need power-of-two size */
	SetupCartPRGMapping(0x10, WRAM, 16384, 1);
	memset(WRAM, 0x00, 16384);
	AddExState(WRAM, 16384, 0, "WRAM");

	CHRRAM =(uint8*)FCEU_gmalloc(262144);
	SetupCartCHRMapping(0x10, CHRRAM, 262144, 1);
	AddExState(CHRRAM, 262144, 0, "CRAM");
	memset(CHRRAM, 0xFF, 262144); /* Don't initialize to $00 because 8 Eyes (F4089) uses a sprite 0 hit sprite with uninitialized CHR data */
	if (info->mapper ==12 && info->submapper ==1 && info->CHRRomSize) { /* FFE F6xxx needs CHR data in PRG-ROM. Enlarge to 512 KiB, copy and replace previous PRG-ROM buffer. */
		uint8* newROM =(uint8*)FCEU_gmalloc(524288);
		memset(newROM, 0xFF, 524288);
		memcpy(newROM +     0,  ROM, info->PRGRomSize >524288? 524288: info->PRGRomSize);
		memcpy(newROM +262144, VROM, info->CHRRomSize >262144? 262144: info->CHRRomSize);
		FCEU_gfree(ROM);
		ROM =newROM;
		SetupCartPRGMapping(0, ROM, 524288, 0);
	} else
	if (info->CHRRomSize) /* Need data in CHR-RAM, not CHR-ROM */
		memcpy(info->CHRRomSize ==8192? (CHRRAM +24576): CHRRAM, VROM, info->CHRRomSize >262144? 262144: info->CHRRomSize);

	trainerSize =trainerpoo? 512: 0;
	trainerSource =trainerpoo;
	if (info->mapper ==17) {
		if (info->submapper >0) {
			trainerInit =info->submapper <<8 &0x0300 | 0x5C00;
			trainerTarget =WRAM +0x2000 +(trainerInit &0x0FFF);
		} else {
			trainerInit =0x7000;
			trainerTarget =WRAM +0x1000;
		}
	} else {
		trainerInit =0x7003;
		trainerTarget =WRAM +0x1000;
	}
	
	info->Power =power;
	info->Close =close;
	MapIRQHook =cpuCycleFFE;
	GameHBIRQHook =pseudoScanlineFFE;
	PPU_hook =trapPPUAddressChangeFFE;
}

void Mapper561_562_Init(CartInfo *info) {
	int CHRRAMSize;
	maker =info->mapper ==562? VENUS: BUNG;
	AddExState(maker ==VENUS? stateRegsVenus: stateRegsBung, ~0, 0, 0);
	mc1ModeInitial =info->submapper <<5 | (info->mirror &1? 0x01: 0x11) | 0x02;
	mc2ModeInitial =0x03;
	extModeInitial =0x03;
	
	WRAM =(uint8*)FCEU_gmalloc(8192);
	SetupCartPRGMapping(0x10, WRAM, 8192, 1);
	memset(WRAM, 0x00, 8192);
	AddExState(WRAM, 8192, 0, "WRAM");

	CHRRAMSize =info->CHRRomSize? info->CHRRomSize: (info->CHRRamSize +info->CHRRamSaveSize);
	CHRRAM =(uint8*)FCEU_gmalloc(CHRRAMSize);
	SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSize, 1);
	AddExState(CHRRAM, CHRRAMSize, 0, "CRAM");
	memset(CHRRAM, 0xFF, CHRRAMSize);
	if (info->mapper ==561 && info->submapper ==3 && info->PRGRomSize ==131072) { /* Crazy Climber (G026) is 128 KiB but expects to find PRG-DRAM above it */
		uint8* newROM =(uint8*)FCEU_gmalloc(262144);
		memset(newROM, 0xFF, 262144);
		memcpy(newROM,  ROM, 131072);
		FCEU_gfree(ROM);
		ROM =newROM;
		SetupCartPRGMapping(0, ROM, 262144, 0);
	}
	if (info->CHRRomSize) memcpy(CHRRAM, VROM, CHRRAMSize);

	/* Bung and Venus trainers can come in two forms:
	   - If the load address is CPU $7000 and the size is 512 bytes, then they are identical to FFE trainers and come as normal iNES Trainers.
	   - Otherwise, they come as NES 2.0 Misc. ROM, and the first two words indicate the load and init address. */
	if (trainerpoo) {
		trainerSize =512;
		trainerSource =trainerpoo;
		trainerInit =0x7003;
		trainerTarget =WRAM +0x1000;
	} else
	if (MiscROM && info->miscROMSize >=4) {
		uint16_t addr;
		trainerSize =info->miscROMSize -4;
		trainerSource =MiscROM +4;
		trainerInit =MiscROM[2] | MiscROM[3] <<8;
		addr =MiscROM[0] | MiscROM[1] <<8;
		if (addr <0x2000)
			trainerTarget =RAM +(addr &0x07FF);
		else
		if (addr >=0x6000 && addr <=0x7FFF)
			trainerTarget =WRAM +(addr &0x1FFF);		
		else
			trainerSize =0;
	} else
		trainerSize =0;
	
	GameStateRestore =sync;
	info->Power =power;
	info->Close =close;
	if (maker ==VENUS) {
		MapIRQHook =cpuCycleVenus;
		PPU_hook =trapPPUAddressChangeVenus;
	} else
		MapIRQHook =cpuCycleBung;
}
