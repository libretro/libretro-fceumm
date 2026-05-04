#ifndef _FCEU_ENDIAN_H
#define _FCEU_ENDIAN_H

#include "fceu-memory.h"
#include <streams/memory_stream.h>

int write32le_mem(uint32_t b, memstream_t *mem);
int read32le_mem(uint32_t *Bufo, memstream_t *mem);

void FlipByteOrder(uint8_t *src, uint32_t count);

/* Byte-swap each `stride`-byte element in `src` in place. Used for
 * savestate fields that contain arrays of multi-byte primitives,
 * where we want each element swapped independently (not the whole
 * buffer reversed end-to-end as FlipByteOrder does). */
void FlipByteOrderStrided(uint8_t *src, uint32_t count, uint32_t stride);

void FCEU_en32lsb(uint8_t *, uint32_t);
uint32_t FCEU_de32lsb(const uint8_t *);

#endif
