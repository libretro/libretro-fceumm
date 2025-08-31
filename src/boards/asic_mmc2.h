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

#ifndef _ASIC_MMC2_H
#define _ASIC_MMC2_H

void MMC2_syncPRG (int, int);
void MMC2_syncCHR (int, int);
void MMC2_syncMirror ();
DECLFW (MMC2_write);
void MMC2_power ();
void MMC2_restore (int);
void MMC2_addExState ();
void MMC2_activate (uint8, void (*)());
void MMC2_init (CartInfo *, void (*)());

#endif
