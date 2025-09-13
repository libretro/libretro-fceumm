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

#ifndef _ASIC_MMC1_H
#define _ASIC_MMC1_H

#define MMC1_TYPE_MMC1A 0
#define MMC1_TYPE_MMC1B 1

void MMC1_syncWRAM (int);
int MMC1_getPRGBank (uint8);
int MMC1_getCHRBank (uint8);
void MMC1_syncPRG (int, int);
void MMC1_syncCHR (int, int);
void MMC1_syncMirror ();
void FP_FASTAPASS(1) MMC1_cpuCycle(int);
DECLFW (MMC1_writeReg);
void MMC1_clear ();
void MMC1_addExState ();
void MMC1_restore (int);
void MMC1_power ();
void MMC1_activate (uint8, void (*)(), uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));
void MMC1_init (CartInfo *, void (*)(), uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));

#endif
