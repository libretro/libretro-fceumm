#ifndef _FCEU_PALETTE_H
#define _FCEU_PALETTE_H

typedef struct {
	uint8_t r, g, b;
} pal;

extern pal *palo;
extern uint8_t palette_game_available;
void FCEU_ResetPalette(void);

void FCEU_ResetPalette(void);
void FCEU_LoadGamePalette(void);

#endif
