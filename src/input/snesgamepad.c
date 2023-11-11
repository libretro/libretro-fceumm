/* FCE Ultra - NES/Famicom Emulator
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

static uint32 rsb[2];
static uint32 rdata[2];

static uint8 Read(int w) {
	uint8 ret = 0;
	ret |= (rdata[w] >> rsb[w]) & 1;
	if (rsb[w] >= 16) {
		ret |= 0x1;
		rsb[w] = 16;
	}
	if (!fceuindbg)
		rsb[w]++;
	return ret;
}

static void Strobe(int w) {
	rsb[w] = 0;
}

static void Update(int w, void *data, int arg) {
	int x;
	int ret = 0;
	for (x = 0; x < 12; x++) {
		ret |= (((*(uint32 *)data) >> x) & 1) << x;
	}
	rdata[w] = ret;
}

static INPUTC SNESGamepad = { Read, 0, Strobe, Update, 0, 0 };

INPUTC *FCEU_InitSNESGamepad(int w) {
	rsb[w] = rdata[w] = 0;
	return (&SNESGamepad);
}
