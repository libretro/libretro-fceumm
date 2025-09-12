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
	AddExState(&fifo->front, 2, 0, "FIFF");
	AddExState(&fifo->back, 2, 0, "FIFB");
}

void FIFO_close (FIFO *fifo) {
	if (fifo->data) {
		free(fifo->data);
		fifo->data = NULL;
	}
}
