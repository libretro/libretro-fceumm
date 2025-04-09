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

#include "mapinc.h"
#include "vrc2and4.h"

static void sync () {
	VRC24_syncPRG(0x0F, VRC2_pins <<4);
	VRC24_syncCHR(0x7F, VRC2_pins <<7);
	VRC24_syncMirror();
}

void Mapper450_reset(void) {
	VRC2_pins =0;
	VRC24_Sync();
}	

void Mapper450_Init (CartInfo *info) {
	VRC24_init(info, sync, 0x01, 0x02, 0, 0, 0);
	info->Reset =Mapper450_reset;
}
