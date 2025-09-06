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

#ifndef _ASIC_MMC3_H
#define _ASIC_MMC3_H

#define MMC3_TYPE_NEC     0 /* C000=00 causes single IRQ */
#define MMC3_TYPE_SHARP   1 /* C000=00 causes continuous IRQs */
#define MMC3_TYPE_ACCLAIM 2 /* No difference to SHARP with FCEU*'s PPU implementation */
#define MMC3_TYPE_AX5202P 3 /* WRAM always enabled */
#define MMC3_TYPE_MMC6    4 /* 1 KiB of built-in WRAM */

void MMC3_syncWRAM (int);
int MMC3_getPRGBank (uint8);
int MMC3_getCHRBank (uint8);
int MMC3_getMirroring (void);
void MMC3_syncPRG (int, int);
void MMC3_syncCHR (int, int);
void MMC3_syncMirror ();
void MMC3_clockCounter ();
DECLFW (MMC3_writeReg);
void MMC3_activate (uint8, void (*)(), uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));
void MMC3_addExState();
void MMC3_restore (int);
void MMC3_power ();
void MMC3_init (CartInfo *, void (*)(), uint8, int (*)(uint8), int (*)(uint8), DECLFR((*)), DECLFW((*)));

#endif
