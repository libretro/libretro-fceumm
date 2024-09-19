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

#ifndef _MMC1_H
#define _MMC1_H

typedef enum {
    MMC1A,
    MMC1B
} MMC1Type;

typedef struct __MMC1 {
    uint8 reg[4];
    uint8 buffer;
    uint8 shift;
} MMC1;

extern MMC1 mmc1;
extern MMC1Type mmc1_type;

uint32 MMC1_GetPRGBank(int index);
uint32 MMC1_GetCHRBank(int index);

DECLFW(MMC1_Write);

void MMC1_Power(void);
void MMC1_Close(void);
void MMC1_Restore(int version);
void MMC1_Reset(void);

void MMC1_Init(CartInfo *info, int wram, int saveram);

void MMC1_FixPRG(void);
void MMC1_FixCHR(void);
void MMC1_FixMIR(void);

extern void (*MMC1_pwrap)(uint16 A, uint16 V);
extern void (*MMC1_cwrap)(uint16 A, uint16 V);
extern void (*MMC1_mwrap)(uint8 V);
extern void (*MMC1_wwrap)(void);

#endif /* _MMC1_H */
