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

#ifndef _ASIC_N118_H
#define _ASIC_N118_H

int N118_getPRGBank (uint8);
int N118_getCHRBank (uint8);
void N118_syncPRG (int, int);
void N118_syncCHR (int, int);
DECLFW (N118_writeReg);
void N118_clear ();
void N118_activate (uint8, void (*)(), int (*)(uint8), int (*)(uint8));
void N118_addExState();
void N118_restore (int);
void N118_power ();
void N118_init (CartInfo *, void (*)(), int (*)(uint8), int (*)(uint8));

#endif
