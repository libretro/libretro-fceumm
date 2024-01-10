#ifndef _FCEU_ENDIAN_H
#define _FCEU_ENDIAN_H

#include "fceu-memory.h"
#include <streams/memory_stream.h>

int write32le_mem(uint32 b, memstream_t *mem);
int read32le_mem(uint32 *Bufo, memstream_t *mem);

void FCEU_en32lsb(uint8 *, uint32);
uint32 FCEU_de32lsb(const uint8 *);

#endif
