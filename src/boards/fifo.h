#ifndef _FIFO_H
#define _FIFO_H

typedef struct {
	size_t capacity;
	uint8_t *data;
	int16_t front;
	int16_t back;
} FIFO;

size_t FIFO_size (FIFO *);
uint8_t FIFO_halfFull (FIFO *);
int FIFO_retrieve (FIFO *);
void FIFO_add (FIFO *, uint8_t);
void FIFO_reset (FIFO *);
void FIFO_init (FIFO *, size_t);
void FIFO_close (FIFO *);

#endif
