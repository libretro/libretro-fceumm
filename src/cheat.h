#ifndef _FCEU_CHEAT_H
#define _FCEU_CHEAT_H

extern uint8_t *MMapPtrs[64];

void FCEU_CheatResetRAM(void);
void FCEU_CheatAddRAM(int s, uint32_t A, uint8_t *p);

void FCEU_LoadGameCheats(void);
void FCEU_FlushGameCheats(void);
void FCEU_ApplyPeriodicCheats(void);
void FCEU_PowerCheats(void);

void FCEU_ResetCheats(void);

#endif
