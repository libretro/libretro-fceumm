#ifndef _FCEU_LATCH_H
#define _FCEU_LATCH_H

typedef struct {
	uint16_t addr;
    uint8_t data;
} LATCH;

extern LATCH latch;

void Latch_Init(CartInfo *info, void (*proc)(void), readfunc func, uint8_t wram, uint8_t busc);
void LatchPower(void);
void LatchClose(void);
void LatchWrite(uint32_t A, uint8_t V);
void LatchHardReset(void);

#endif /* _FCEU_LATCH_H */
