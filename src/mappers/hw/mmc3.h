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

#ifndef _MMC3_H
#define _MMC3_H

typedef struct __MMC3 {
    uint8 cmd;
    uint8 opts;
    uint8 mirr;
    uint8 wram;
    uint8 reg[8];
} MMC3;

extern MMC3 mmc3;
extern int isRevB;

uint8 MMC3_GetPRGBank(int V);
uint8 MMC3_GetCHRBank(int V);

DECLFW(MMC3_CMDWrite); /* $ 0x8000 - 0xBFFF */
DECLFW(MMC3_IRQWrite); /* $ 0xC000 - 0xFFFF */
DECLFW(MMC3_Write); /* $ 0x8000 - 0xFFFF */

void MMC3_Power(void);
void MMC3_Reset(void);
void MMC3_Close(void);
void MMC3_IRQHBHook(void);
int  MMC3_WramIsWritable(void);
void MMC3_Init(CartInfo *info, int wram, int battery);

extern void (*MMC3_FixPRG)(void);
extern void (*MMC3_FixCHR)(void);
extern void (*MMC3_FixMIR)(void);

extern void (*MMC3_pwrap)(uint16 A, uint16 V);
extern void (*MMC3_cwrap)(uint16 A, uint16 V);

#endif /* _MMC3_H */
