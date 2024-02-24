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

#ifndef _BANDAI_H
#define _BANDAI_H

DECLFW(BANDAI_Write);

void BANDAI_Power(void);
void BANDAI_IRQHook(int a);
void BANDAI_Reset(void);

void BANDAI_Init(CartInfo *info, EEPROM_TYPE _eeprom_type, int _isFCG);

void BANDAI_FixPRG(void);
void BANDAI_FixCHR(void);
void BANDAI_FixMIR(void);

extern void (*BANDAI_pwrap)(uint16 A, uint16 V);
extern void (*BANDAI_cwrap)(uint16 A, uint16 V);

#endif /* _BANDAI_H */
