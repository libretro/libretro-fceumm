#ifndef _FCEU_LATCH_H
#define _FCEU_LATCH_H

typedef struct {
	uint16 addr;
    uint8 data;
} LATCH;

extern LATCH latch;

void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func, uint8 wram, uint8 busc);
void LatchPower(void);
void LatchClose(void);
void LatchWrite(uint32 A, uint8 V);
void LatchHardReset();

#endif /* _FCEU_LATCH_H */
