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

#ifndef _N118_H
#define _N118_H

typedef struct __N118 {
    uint8 reg[8];
    uint8 cmd;
} N118;

extern N118 n118;

DECLFW(N118_Write);

void N118_Close(void);
void N118_Power(void);

void N118_Init(CartInfo *info, int wsize, int battery);

extern void (*N118_FixPRG)(void);
extern void (*N118_FixCHR)(void);

extern void (*N118_pwrap)(uint16 A, uint16 V);
extern void (*N118_cwrap)(uint16 A, uint16 V);

#endif /* _N118_H */
