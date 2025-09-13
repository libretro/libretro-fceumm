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

#ifndef _ASIC_VRC6_H
#define _ASIC_VRC6_H

void VRC6_syncWRAM (int);
void VRC6_syncPRG (int, int);
void VRC6_syncCHR (int, int);
void VRC6_syncMirror ();
int VRC6_getPRGBank (uint8);
int VRC6_getCHRBank (uint8);
DECLFR(VRC6_readWRAM);
DECLFW(MMC3_writeWRAM);
DECLFW(VRC6_writeReg);
void FP_FASTAPASS(1) VRC6_cpuCycle (int);
void VRC6_clear ();
void VRC6_activate (uint8, void (*)(), int, int, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));
void VRC6_addExState ();
void VRC6_restore (int);
void VRC6_power ();
void VRC6_init (CartInfo *, void (*)(), int, int, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));

#endif