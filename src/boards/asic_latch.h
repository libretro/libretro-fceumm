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

#ifndef _ASIC_LATCH_H
#define _ASIC_LATCH_H

extern uint16 Latch_address;
extern uint8 Latch_data;

DECLFW (Latch_write);
void Latch_addExState ();
void Latch_restore (int);
void Latch_power ();
void Latch_activate (uint8, void (*)(), uint16, uint16, void (*)(uint16*, uint8*, uint8));
void Latch6_activate (uint8, void (*)(), uint16, uint16, void (*)(uint16*, uint8*, uint8));
void Latch_init (CartInfo *, void (*)(), uint16, uint16, void (*)(uint16*, uint8*, uint8));

#endif
