/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
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

/* NES 2.0 Mapper 556
 * Used for the for the è¶…å¼ºå°æ–°2+ç‘ªèŽ‰å®¶æ— 7-in-1 (JY-215) multicart.
 */

#include "mapinc.h"

static uint8 reg[5];
static uint8 mmc3Reg[8];
static uint8 mmc3Cmd;
static uint8 mmc3Mirr;
static uint8 mmc3Wram;
static uint8 mmc3IRQLatch;
static uint8 mmc3IRQCount;
static uint8 mmc3IRQa;
static uint8 mmc3IRQReload;
static uint8 vrc4Prg[2];
static uint8 vrc4Mirr;
static uint8 vrc4Misc;
static uint16 vrc4Chr[8];
static uint8 vrc4IRQLatch;
static uint8 vrc4IRQa;
static uint8 vrc4IRQCount;
static int16 vrc4IRQCycles;
static uint8 *WRAM = NULL;
static uint32 WRAMSIZE = 0;

static SFORMAT StateRegs[] = {
	{ reg,            5, "REGS" },
	{ mmc3Reg,        8, "MMC3" },
	{ &mmc3Cmd,       1, "M3IX" },
	{ &mmc3Mirr,      1, "M3MI" },
	{ &mmc3Wram,      1, "M3WR" },
	{ &mmc3IRQLatch,  1, "M3RL" },
	{ &mmc3IRQCount,  1, "M3CN" },
	{ &mmc3IRQa,      1, "M3IQ" },
	{ &mmc3IRQReload, 1, "M3IR" },
	{ vrc4Prg,        2, "V4PR" },
	{ &vrc4Mirr,      1, "V4MI" },
	{ &vrc4Misc,      1, "V4MS" },
	{ vrc4Chr,       16, "V4CH" },
	{ &vrc4IRQLatch,  1, "VILA" },
	{ &vrc4IRQa,      1, "VIMO" },
	{ &vrc4IRQCount,  1, "VICO" },
	{ &vrc4IRQCycles, 2, "VICY" },
	{ 0 },
};

static void Sync(void) {
	uint32 prgmask = ~reg[3] & 0x3F;
	uint32 prgbase = ((reg[3] & 0x40) << 2) | reg[1];
	uint32 chrmask = 0xFF >> (~reg[2] & 0x0F);
	uint32 chrbase = ((reg[3] & 0x40) << 6) | ((reg[2] & 0xF0) << 4) | reg[0];
	uint32 cbase = 0; /* prg/chr bank flip flag */

	if (~reg[2] & 0x80) {
		/* MMC3 */
		cbase = (mmc3Cmd << 8) & 0x4000;
		setprg8(0x8000 ^ cbase, (prgbase & ~prgmask) | (mmc3Reg[6] & prgmask));
		setprg8(0xA000,         (prgbase & ~prgmask) | (mmc3Reg[7] & prgmask));
		setprg8(0xC000 ^ cbase, (prgbase & ~prgmask) | (0xFE & prgmask));
		setprg8(0xE000,         (prgbase & ~prgmask) | (0xFF & prgmask));

		cbase = (mmc3Cmd << 5) & 0x1000;
		setchr1(0x0000 ^ cbase, (chrbase & ~chrmask) | ((mmc3Reg[0] & 0xFE) & chrmask));
		setchr1(0x0400 ^ cbase, (chrbase & ~chrmask) | ((mmc3Reg[0] | 0x01) & chrmask));
		setchr1(0x0800 ^ cbase, (chrbase & ~chrmask) | ((mmc3Reg[1] & 0xFE) & chrmask));
		setchr1(0x0C00 ^ cbase, (chrbase & ~chrmask) | ((mmc3Reg[1] | 0x01) & chrmask));
		setchr1(0x1000 ^ cbase, (chrbase & ~chrmask) | (mmc3Reg[2] & chrmask));
		setchr1(0x1400 ^ cbase, (chrbase & ~chrmask) | (mmc3Reg[3] & chrmask));
		setchr1(0x1800 ^ cbase, (chrbase & ~chrmask) | (mmc3Reg[4] & chrmask));
		setchr1(0x1C00 ^ cbase, (chrbase & ~chrmask) | (mmc3Reg[5] & chrmask));

		setmirror((mmc3Mirr & 0x01) ^ 1);
	} else {
		/* VRC4 mode */
		cbase = (vrc4Misc << 13) & 0x4000;
		setprg8(0x8000 ^ cbase, (prgbase & ~prgmask) | (vrc4Prg[0] & prgmask));
		setprg8(0xA000,         (prgbase & ~prgmask) | (vrc4Prg[1] & prgmask));
		setprg8(0xC000 ^ cbase, (prgbase & ~prgmask) | (0xFE & prgmask));
		setprg8(0xE000,         (prgbase & ~prgmask) | (0xFF & prgmask));

		setchr1(0x0000,         (chrbase & ~chrmask) | (vrc4Chr[0] & chrmask));
		setchr1(0x0400,         (chrbase & ~chrmask) | (vrc4Chr[1] & chrmask));
		setchr1(0x0800,         (chrbase & ~chrmask) | (vrc4Chr[2] & chrmask));
		setchr1(0x0C00,         (chrbase & ~chrmask) | (vrc4Chr[3] & chrmask));
		setchr1(0x1000,         (chrbase & ~chrmask) | (vrc4Chr[4] & chrmask));
		setchr1(0x1400,         (chrbase & ~chrmask) | (vrc4Chr[5] & chrmask));
		setchr1(0x1800,         (chrbase & ~chrmask) | (vrc4Chr[6] & chrmask));
		setchr1(0x1C00,         (chrbase & ~chrmask) | (vrc4Chr[7] & chrmask));

		switch (vrc4Mirr & 0x03) {
		case 0: setmirror(MI_V); break;
		case 1: setmirror(MI_H); break;
		case 2: setmirror(MI_0); break;
		case 3: setmirror(MI_1); break;
		}
	}
}

static DECLFW(writeMMC3) {
	switch (A & 0xE001) {
	case 0x8000:
		mmc3Cmd = V;
		Sync();
		break;
	case 0x8001:
		mmc3Reg[mmc3Cmd & 7] = V;
		Sync();
		break;
	case 0xA000:
		mmc3Mirr = V;
		Sync();
		break;
	case 0xA001:
		mmc3Wram = V;
		Sync();
		break;
	case 0xC000:
		mmc3IRQLatch = V;
		break;
	case 0xC001:
		mmc3IRQReload = 1;
		break;
	case 0xE000:
		mmc3IRQa = 0;
		X6502_IRQEnd(FCEU_IQEXT);
		break;
	case 0xE001:
		mmc3IRQa = 1;
		break;
	}
}

static DECLFW(writeVRC4) {
	uint8 index;
	A = (A & 0xF000) | ((A >> 2) & 3) | (A & 3);
	switch (A & 0xF000) {
	case 0x8000:
	case 0xA000:
		vrc4Prg[(A >> 13) & 1] = V;
		Sync();
		break;
	case 0x9000:
		if (~A & 2) {
			vrc4Mirr = V;
		} else if (~A & 1) {
			vrc4Misc = V;
		}
		Sync();
		break;
	case 0xF000:
		switch (A & 3) {
		case 0:
			vrc4IRQLatch = (vrc4IRQLatch & 0xF0) | (V & 0x0F);
			break;
		case 1:
			vrc4IRQLatch = (vrc4IRQLatch & 0x0F) | (V << 4);
			break;
		case 2:
			vrc4IRQa = V;
			if (vrc4IRQa & 0x02) {
				vrc4IRQCount = vrc4IRQLatch;
				vrc4IRQCycles = 341;
			}
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		case 3:
			vrc4IRQa = (vrc4IRQa & ~0x02) | ((vrc4IRQa << 1) & 0x02);
			X6502_IRQEnd(FCEU_IQEXT);
			break;
		}
		break;
	default:
		index = ((A - 0xB000) >> 11) | ((A >> 1) & 1);
		if (A & 1) {
			vrc4Chr[index] = (vrc4Chr[index] & 0x0F) | (V << 4);
		} else {
			vrc4Chr[index] = (vrc4Chr[index] & ~0x0F) | (V & 0x0F);
		}
		Sync();
		break;
	}
}

static DECLFW(M556WriteReg) {
	if (~reg[3] & 0x80) {
		reg[reg[4] & 3] = V;
		reg[4]++;
		Sync();
	}
}

static DECLFW(M556Write) {
	if (~reg[2] & 0x80) {
		writeMMC3(A, V);
	} else {
		writeVRC4(A, V);
	}
}

static void FP_FASTAPASS(1) M556CPUHook(int a) {
	int count = a;

	if (~reg[2] & 0x80) {
		return;
	}

	/* VRC4 IRQ mode */
	while (count--) {
		if ((vrc4IRQa & 0x02) && ((vrc4IRQa & 0x04) || ((vrc4IRQCycles -= 3) <= 0))) {
			if (~vrc4IRQa & 0x04) {
				vrc4IRQCycles += 341;
			}
			if (!++vrc4IRQCount) {
				vrc4IRQCount = vrc4IRQLatch;
				X6502_IRQBegin(FCEU_IQEXT);
			}
		}
	}
}

static void M556HBHook(void) {
	int count = mmc3IRQCount;

	if (reg[2] & 0x80) {
		return;
	}

	/* MMC3 IRQ mode */
	if (!count || mmc3IRQReload) {
		mmc3IRQCount = mmc3IRQLatch;
	} else {
		mmc3IRQCount--;
	}
	if (count && !mmc3IRQCount && mmc3IRQa) {
		X6502_IRQBegin(FCEU_IQEXT);
	}
	mmc3IRQReload = 0;
}

static void M556Reset(void) {
	int i;
	for (i = 0; i < 5; i++) {
		reg[i] = 0;
    }
	reg[2] = 0x0F;
	Sync();
}

static void M556Power(void) {
	int i;
	for (i = 0; i < 5; i++) {
		reg[i] = 0;
    }
	for (i = 0; i < 8; i++) {
		mmc3Reg[i] = 0;
    }
	for (i = 0; i < 2; i++) {
		vrc4Prg[i] = 0;
    }
	for (i = 0; i < 8; i++) {
		vrc4Chr[i] = 0;
    }
	mmc3Cmd = mmc3Mirr = mmc3Wram = mmc3IRQLatch = mmc3IRQCount = mmc3IRQa = mmc3IRQReload = 0;
	vrc4Mirr = vrc4Misc = vrc4IRQLatch = vrc4IRQa = vrc4IRQCount = vrc4IRQCycles = 0;
	reg[2] = 0x0F;
	Sync();

	SetReadHandler(0x8000, 0xFFFF, CartBR);
	SetWriteHandler(0x5000, 0x5FFF, M556WriteReg);
	SetWriteHandler(0x8000, 0xFFFF, M556Write);

	if (WRAM) {
		setprg8r(0x10, 0x6000, 0);
		SetReadHandler(0x6000, 0x7FFF, CartBR);
		SetWriteHandler(0x6000, 0x7FFF, CartBW);
	}
}

static void M556Close(void) {
	if (WRAM) {
		FCEU_gfree(WRAM);
	}
	WRAM = NULL;
}

static void StateRestore(int version) {
	Sync();
}

void Mapper556_Init(CartInfo *info) {
	info->Reset = M556Reset;
	info->Power = M556Power;
	info->Close = M556Close;
	MapIRQHook = M556CPUHook;
	GameHBIRQHook = M556HBHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, 0);

	WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
	if (WRAMSIZE) {
		WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (info->battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}
}
