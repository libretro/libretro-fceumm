#ifndef _FCEU_PPU_H
#define _FCEU_PPU_H

void FCEUPPU_Init(void);
void FCEUPPU_Reset(void);
void FCEUPPU_Power(void);
int FCEUPPU_Loop(int skip);

void FCEUPPU_LineUpdate(void);
void FCEUPPU_SetVideoSystem(int w);

extern void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
extern void FP_FASTAPASS(1) (*PPU_hook)(uint32 A);

/* For cart.c and banksw.h, mostly */
extern uint8 NTARAM[0x800], *vnapage[4];
extern uint8 PPUNTARAM;
extern uint8 PPUCHRRAM;

void FCEUPPU_SaveState(void);
void FCEUPPU_LoadState(int version);

extern int scanline;
extern uint8 PPU[4];

/* MMC5 specifics */
#define Sprite16          (PPU[0] & 0x20) /* Sprites 8x16/8x8 */
#define PPUON             (PPU[1] & 0x18) /* PPU should operate */

#define ABANKS            MMC5SPRVPage
#define BBANKS            MMC5BGVPage
#define MMC5SPRVRAMADR(V) &MMC5SPRVPage[(V) >> 10][(V)]
#define VRAMADR(V)        &VPage[(V) >> 10][(V)]
uint8 *MMC5BGVRAMADR(uint32 A);

/* m547 */
extern uint8 QTAIHack;
extern uint8 qtaintramreg;
extern uint8 QTAINTRAM[0x800];

#endif
