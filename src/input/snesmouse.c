/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

#include "share.h"

typedef struct {
	bool strobe;
	uint32 latch; /* latched data (read when strobe goes high to low) */
	uint32 sensitivity; /* reading while strobe is high cycles sensitivity 0,1,2 */
	int32 dx, dy; /* relative position to communicate */
	int32 mb; /* current buttons */
} SNES_MOUSE;

static SNES_MOUSE SNESMouse;

static uint8 ReadSNESMouse(int w) {
	uint8 result;

	if (SNESMouse.strobe) {
		SNESMouse.sensitivity += 1;
		if (SNESMouse.sensitivity > 2)
			SNESMouse.sensitivity = 0;
	}

	result = (SNESMouse.latch & 0x80000000) >> 31;
	SNESMouse.latch = (SNESMouse.latch << 1);

	return result;
}

static void WriteSNESMouse(uint8 v) {
	uint8 byte0;
	uint8 byte1;
	uint8 byte2;
	uint8 byte3;
	bool strobing = (v & 1);

	if (SNESMouse.strobe && !strobing) {
		int dx = SNESMouse.dx;
		int dy = SNESMouse.dy;
		/* convert to sign and magnitude */
		bool sx = (dx < 0);
		bool sy = (dy < 0);

		SNESMouse.dx = 0;
		SNESMouse.dy = 0;

		if (dx < 0)
			dx = -dx;
		if (dy < 0)
			dy = -dy;

		/* apply sensitivity */
		dx += dx >> (2 - SNESMouse.sensitivity);
		dx += dx >> (2 - SNESMouse.sensitivity);

		if (dx > 127)
			dx = 127;
		if (dy > 127)
			dy = 127;

		/* FCEU_printf("SNES Mouse: %1d %3d, %1d %3d %1d x%1d\n",sx,dx,sy,dy,SNESMouse.mb,SNESMouse.sensitivity); */

		byte0 = 0x00;
		byte1 = 0x1 | /* signature */
		    ((SNESMouse.sensitivity & 3) << 4) | /* sensitivity */
		    ((SNESMouse.mb & 3) << 6); /* buttons */
		byte2 = (uint8)(dy) | (sy ? 0x80 : 0x00);
		byte3 = (uint8)(dx) | (sx ? 0x80 : 0x00);

		SNESMouse.latch = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | (byte3 << 0);
	}

	SNESMouse.strobe = strobing;
}

static void UpdateSNESMouse(int w, void *data, int arg) {
	int32 *ptr = (int32 *)data;

	SNESMouse.dx += ptr[0];
	ptr[0] = 0;
	SNESMouse.dy += ptr[1];
	ptr[1] = 0;
	SNESMouse.mb = ptr[2] & 3; /* bit 0 = left button, bit 1 = right button */
}

static INPUTC SNES_MOUSEC = { ReadSNESMouse, WriteSNESMouse, 0, UpdateSNESMouse, 0, 0 };

INPUTC *FCEU_InitSNESMouse(int w) {
	SNESMouse.dx = 0;
	SNESMouse.dy = 0;
	SNESMouse.latch = 0;
	SNESMouse.mb = 0;
	SNESMouse.sensitivity = 0;
	SNESMouse.strobe = 0;
	return (&SNES_MOUSEC);
}
