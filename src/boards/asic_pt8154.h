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

#ifndef _ASIC_PT8154_H
#define _ASIC_PT8154_H

void PT8154_syncPRG (int, int);
void PT8154_syncCHR (int, int);
void PT8154_syncMirror ();
DECLFW (PT8154_writeExtra);
DECLFW (PT8154_writeWRAM);
void PT8154_activate (uint8, void (*)());
void PT8154_addExState ();
void PT8154_restore (int);
void PT8154_power ();
void PT8154_init (CartInfo *, void (*)());

#endif
