#ifndef _FCEU_PPU_H
#define _FCEU_PPU_H

typedef struct {
	/* overclock the console by adding dummy scanlines to PPU loop
	 * disables DMC DMA and WaveHi filling for these dummies
	 * doesn't work with new PPU */
	struct {
		uint8 overclocked_state; /* ppu in overclock state? stops sound from running in overclocked state */
		uint8 DMC_7bit_in_use; /* DMC in use */
		uint16 vblank_scanlines;
		uint16 postrender_scanlines;
	} overclock;
	uint16 totalscanlines;
	uint16 normal_scanlines;
} PPU_T;

extern PPU_T ppu;

void FCEUPPU_Init(void);
void FCEUPPU_Reset(void);
void FCEUPPU_Power(void);
int FCEUPPU_Loop(int skip);

void FCEUPPU_LineUpdate(void);
void FCEUPPU_SetVideoSystem(int w);

extern void (*GameHBIRQHook)(void), (*GameHBIRQHook2)(void);
extern void (*PPU_hook)(uint32 A);

/* For cart.c and banksw.h, mostly */
extern uint8 NTARAM[0x1000], *vnapage[4];
extern uint8 PPUNTARAM;
extern uint8 PPUCHRRAM;

void FCEUPPU_SaveState(void);
void FCEUPPU_LoadState(int version);

extern int scanline;
extern uint8 PPU[4];

#endif
