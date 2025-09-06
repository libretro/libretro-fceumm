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

#ifndef _ASIC_FME7_H
#define _ASIC_FME7_H

void FME7_syncWRAM (int);
void FME7_syncPRG (int, int);
void FME7_syncCHR (int, int);
void FME7_syncMirror ();
DECLFR(FME7_readWRAM);
DECLFW(MMC3_writeWRAM);
DECLFW(FME7_writeReg);
void FP_FASTAPASS(1) FME7_cpuCycle (int);
void FME7_activate (uint8, void (*)());
void FME7_addExState ();
void FME7_restore (int);
void FME7_power ();
void FME7_init (CartInfo *, void (*)());

#endif