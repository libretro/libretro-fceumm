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

#ifndef _ASIC_VRC3_H
#define _ASIC_VRC3_H

void VRC3_syncWRAM (int);
void VRC3_syncPRG (int, int);
void VRC3_syncCHR (int, int);
DECLFW (VRC3_write);
void VRC3_clear ();
void VRC3_activate (uint8, void (*)());
void VRC3_addExState ();
void VRC3_restore (int);
void VRC3_power ();
void VRC3_init (CartInfo *, void (*)());

#endif
