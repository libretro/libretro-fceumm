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

#ifndef _VRCIRQ_H
#define _VRCIRQ_H

void VRCIRQ_Init(int irqRepeated);
void VRCIRQ_Latch(uint8 V);
void VRCIRQ_LatchNibble(uint8 V, uint8 highBit);
void VRCIRQ_Control(uint8 V);
void VRCIRQ_Acknowledge(void);
void VRCIRQ_CPUHook(int a);

extern SFORMAT VRCIRQ_StateRegs[];

#endif /* _VRCIRQ_H */
