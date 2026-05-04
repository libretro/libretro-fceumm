#ifndef _MMC3_H
#define _MMC3_H

extern uint8_t MMC3_cmd;
extern uint8_t mmc3opts;
extern uint8_t A000B;
extern uint8_t A001B;
extern uint8_t EXPREGS[8];
extern uint8_t DRegBuf[8];

extern void (*pwrap)(uint32_t A, uint8_t V);
extern void (*cwrap)(uint32_t A, uint8_t V);
extern void (*mwrap)(uint8_t V);

void GenMMC3Power(void);
void GenMMC3Restore(int version);
void MMC3RegReset(void);
void GenMMC3Close(void);
void FixMMC3PRG(int V);
void FixMMC3CHR(int V);
DECLFW(MMC3_CMDWrite);
DECLFW(MMC3_IRQWrite);
int MMC3CanWriteToWRAM(void);

void GenMMC3_Init(CartInfo *info, int prg, int chr, int wram, int battery);

#endif /* _MMC3_H */
