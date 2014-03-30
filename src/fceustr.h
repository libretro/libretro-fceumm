#ifndef _FCEU_STR_H
#define _FCEU_STR_H

typedef struct {
	uint8 *data;
	uint32 len;	/* Not including extra NULL character. */
} fceustr;

#endif
