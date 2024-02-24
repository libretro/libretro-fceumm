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

#ifndef _VRC7_H
#define _VRC7_H

typedef struct __VRC7 {
    uint8 prg[4];
    uint8 chr[8];
    uint8 mirr;
} VRC7;

extern VRC7 vrc7;

DECLFW(VRC7_Write);

void VRC7_Power(void);
void VRC7_Close(void);

void VRC7_Init(CartInfo *info, uint32 A0, uint32 A1);

void VRC7_FixPRG(void);
void VRC7_FixCHR(void);

extern void (*VRC7_pwrap)(uint16 A, uint16 V);
extern void (*VRC7_cwrap)(uint16 A, uint16 V);
extern void (*VRC7_mwrap)(uint8 V);

#endif /* _VRC7_H */
