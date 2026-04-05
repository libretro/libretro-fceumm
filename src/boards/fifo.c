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
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

size_t FIFO_size (FIFO *fifo) {
	fifo->front %= fifo->capacity;
	fifo->back %= fifo->capacity;
	return (fifo->back -fifo->front +fifo->capacity) %fifo->capacity;
}

uint8 FIFO_halfFull (FIFO *fifo) {
	return FIFO_size(fifo) >= fifo->capacity /2;
}

int FIFO_retrieve (FIFO *fifo) {
	fifo->front %= fifo->capacity;
	return FIFO_size(fifo)? fifo->data[fifo->front++]: 0;
}

void FIFO_add (FIFO *fifo, uint8 value) {
	fifo->back %= fifo->capacity;
	if (FIFO_size(fifo) <fifo->capacity) fifo->data[fifo->back++] = value;
}

void FIFO_reset (FIFO *fifo) {
	fifo->front = fifo->back = 0;
}

void FIFO_init (FIFO *fifo, size_t newCapacity) {
	fifo->capacity = newCapacity;
	fifo->data = (uint8*)FCEU_gmalloc(newCapacity);
	AddExState(fifo->data, fifo->capacity, 0, "FIFD");
	AddExState(&fifo->front, 2 | FCEUSTATE_RLSB, 0, "FIFF");
	AddExState(&fifo->back, 2 | FCEUSTATE_RLSB, 0, "FIFB");
}

void FIFO_close (FIFO *fifo) {
	if (fifo->data) {
		FCEU_gfree(fifo->data);
		fifo->data = NULL;
	}
}
