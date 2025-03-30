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

#ifndef _MMC3A_H
#define _MMC3A_H

#include "mapinc.h"

#define MMC3_TYPE_NEC   0
#define MMC3_TYPE_SHARP 1

extern	uint8	MMC3_type;
extern	uint8	MMC3_index;
extern	uint8	MMC3_reg[16];
extern	uint8	MMC3_regMask;
extern	uint8	MMC3_mirroring;
extern	uint8	MMC3_wramEnable;
extern	uint8	MMC3_reloadValue;
extern	uint8	MMC3_reloadRequest;
extern	uint8	MMC3_irqEnable;
extern	uint8	MMC3_counter;

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

extern	void	(*MMC3_Sync)();
extern	uint16	(*MMC3_GetPRGBank)(uint8);
extern	uint16	(*MMC3_GetCHRBank)(uint8);
extern	uint8	FP_FASTAPASS(1) (*MMC3_ReadWRAM) (uint32);
extern	void	FP_FASTAPASS(2) (*MMC3_WriteWRAM)(uint32, uint8);

void	MMC3_syncWRAM ();
uint16	MMC3_getPRGBank (uint8);
uint16	MMC3_getCHRBank (uint8);
void	MMC3_syncPRG (int, int);
void	MMC3_syncCHR (int, int);
void	MMC3_syncMirror ();
void	MMC3_clockCounter ();
DECLFW(MMC3_write);
void	MMC3_power (CartInfo *);
void	MMC3_restore (int);
void	MMC3_init (CartInfo *, void (*)());
void	MMC3_init_enhanced (CartInfo *, void (*)(), uint8, uint8);
void	MMC3_setNumberOfRegs (uint8);
void	MMC3_setBankCallback (uint16 (*)(uint8), uint16 (*)(uint8));
void	MMC3_setWRAMCallback (FP_FASTAPASS(1)(*) (uint32), FP_FASTAPASS(2)(*)(uint32,uint8));

#endif

