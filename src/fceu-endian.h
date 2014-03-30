#ifndef FCEU_ENDIAN_H_
#define FCEU_ENDIAN_H_

#include "memory.h"

int write16le(uint16 b, MEM_TYPE *fp);
int write32le(uint32 b, MEM_TYPE *fp);
int read32le(uint32 *Bufo, MEM_TYPE *fp);
void FlipByteOrder(uint8 *src, uint32 count);

void FCEU_en32lsb(uint8 *, uint32);
uint32 FCEU_de32lsb(uint8 *);

#endif
