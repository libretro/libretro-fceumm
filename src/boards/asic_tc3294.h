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

#ifndef _ASIC_TC3294_H
#define _ASIC_TC3294_H

void TC3294_syncWRAM (int);
void TC3294_syncPRG (int, int);
void TC3294_syncCHR (int, int);
void TC3294_syncMirror ();
DECLFW (TC3294_write);
void TC3294_activate (uint8, void (*)());
void TC3294_addExState ();
void TC3294_restore (int);
void TC3294_power ();
void TC3294_init (CartInfo *, void (*)());

#endif
