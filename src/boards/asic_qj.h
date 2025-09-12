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

#ifndef _ASIC_QJ_H
#define _ASIC_QJ_H

void QJ_syncWRAM (int);
void QJ_syncPRG (int, int);
void QJ_syncCHR (int, int);
void QJ_syncMirror ();
DECLFW (QJ_write);
void QJ_clear ();
void QJ_activate (uint8, void (*)());
void QJ_addExState ();
void QJ_restore (int);
void QJ_power ();
void QJ_init (CartInfo *, void (*)());

#endif
