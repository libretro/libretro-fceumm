/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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
#include "pic16c5x.h"

static uint32 address;
static uint8 *picrom = NULL;

static uint8_t pci16c5x_read(int port) {
	if (port == 0) {
		return (1 |
			(address & 0x0040 ? 0x02 : 0) | /*  A6 -> RA1 */
			(address & 0x0020 ? 0x04 : 0) | /*  A5 -> RA2 */
			(address & 0x0010 ? 0x08 : 0)); /*  A4 -> RA3 */
	} else if (port == 1) {
		return (
			(address & 0x1000 ? 0x01 : 0) | /* A12 -> RB0 */
			(address & 0x0080 ? 0x02 : 0) | /*  A7 -> RB1 */
			(address & 0x0400 ? 0x04 : 0) | /* A10 -> RB2 */
			(address & 0x0800 ? 0x08 : 0) | /* A11 -> RB3 */
			(address & 0x0200 ? 0x10 : 0) | /*  A9 -> RB4 */
			(address & 0x0100 ? 0x20 : 0) | /*  A8 -> RB5 */
			(address & 0x2000 ? 0x40 : 0) | /* A13 -> RB6 */
			(address & 0x4000 ? 0x80 : 0)); /* A14 -> RB7 */
	}
	return (0xFF);
}

static void pci16c5x_write(int port, int val) {
	if (port == 0) {
		if (val & 0x1001) {
			X6502_IRQEnd(FCEU_IQEXT);
		} else {
			X6502_IRQBegin(FCEU_IQEXT);
		}
	}
}

static void M355CPUIRQHook(int a) {
	while (a--) {
		pic16c5x_run();
	}
}

static readfunc cpuRead[0x10000];
static writefunc cpuWrite[0x10000];

static DECLFR(M355Read) {
	address = A;
	if (A >= 0x8000) {
		return CartBR(A);
	}
	return cpuRead[A](A);
}

static DECLFW(M355Write) {
	address = A;
	if (cpuWrite[A]) {
		cpuWrite[A](A, V);
	}
}

static void M355Power(void) {
	int x;

	address = 0;

	pic16c5x_reset(1);

	setprg32(0x8000, 0);
	setchr8(0);

	for (x = 0; x < 0x10000; x++) {
		cpuRead[x] = GetReadHandler(x);
		cpuWrite[x] = GetWriteHandler(x);
	}

	SetReadHandler(0, 0xFFFF, M355Read);
	SetWriteHandler(0, 0xFFFF, M355Write);
}

static void M355Reset(void) {
	address = 0;
	pic16c5x_reset(0);
}

static void M355Close(void) {
	picrom = NULL;
}

void UNL3DBlock_Init(CartInfo *info) {
	if (MiscROM && info->miscROMSize) picrom = MiscROM;
	if (picrom) {
		pic16c54_init(picrom, pci16c5x_read, pci16c5x_write);
		pic16c5x_add_statesinfo();
	}
	info->Power = M355Power;
	info->Reset = M355Reset;
	info->Close = M355Close;
	MapIRQHook = M355CPUIRQHook;
	AddExState(&address, sizeof(address), 0, "ADDR");
}
