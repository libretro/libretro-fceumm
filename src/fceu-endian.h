#ifndef _FCEU_ENDIAN_H
#define _FCEU_ENDIAN_H

#include "fceu-memory.h"
#include <streams/memory_stream.h>

int write32le_mem(uint32_t b, memstream_t *mem);
int read32le_mem(uint32_t *Bufo, memstream_t *mem);

void FlipByteOrder(uint8_t *src, uint32_t count);

void FCEU_en32lsb(uint8_t *, uint32_t);
uint32_t FCEU_de32lsb(const uint8_t *);

#endif
