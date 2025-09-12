#ifndef _FIFO_H
#define _FIFO_H

typedef struct {
	size_t capacity;
	uint8 *data;
	int16 front;
	int16 back;
} FIFO;

size_t FIFO_size (FIFO *);
uint8 FIFO_halfFull (FIFO *);
int FIFO_retrieve (FIFO *);
void FIFO_add (FIFO *, uint8);
void FIFO_reset (FIFO *);
void FIFO_init (FIFO *, size_t);
void FIFO_close (FIFO *);

#endif
