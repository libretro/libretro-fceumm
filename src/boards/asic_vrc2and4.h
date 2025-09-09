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

#ifndef _VRC2AND4_H
#define _VRC2AND4_H

extern uint8 VRC2_pins;

DECLFR(VRC2_readMicrowire);
DECLFW(VRC2_writeMicrowire);
DECLFR(VRC24_readWRAM);
DECLFW(VRC24_writeWRAM);

void VRC24_syncWRAM (int);
void VRC24_syncPRG (int, int);
void VRC24_syncCHR (int, int);
int  VRC24_getPRGBank (uint8);
int  VRC24_getCHRBank (uint8);
void VRC24_syncMirror ();
DECLFW(VRC24_writeReg);

void FP_FASTAPASS(1) VRC4_cpuCycle(int);
void VRC24_reconfigure (int, int);
void VRC2_activate (uint8, void (*)(), int, int, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));
void VRC4_activate (uint8, void (*)(), int, int, uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)), DECLFW((*)));
void VRC2_addExState ();
void VRC4_addExState ();
void VRC24_addExState ();
void VRC24_restore (int);
void VRC24_power ();
void VRC2_init (CartInfo *, void (*)(), int, int, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));
void VRC4_init (CartInfo *, void (*)(), int, int, uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)), DECLFW((*)));

#endif
