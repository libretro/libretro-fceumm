#ifndef _FCEU_INPUT_H
#define _FCEU_INPUT_H

typedef struct {
	uint8_t FP_FASTAPASS(1) (*Read)(int w);
	void FP_FASTAPASS(1) (*Write)(uint8_t v);
	void FP_FASTAPASS(1) (*Strobe)(int w);
	void FP_FASTAPASS(3) (*Update)(int w, void *data, int arg);
	void FP_FASTAPASS(3) (*SLHook)(int w, uint8_t *bg, uint8_t *spr, uint32_t linets, int final);
	void FP_FASTAPASS(3) (*Draw)(int w, uint8_t *buf, int arg);
} INPUTC;

typedef struct {
	uint8_t FP_FASTAPASS(2) (*Read)(int w, uint8_t ret);
	void FP_FASTAPASS(1) (*Write)(uint8_t v);
	void (*Strobe)(void);
	void FP_FASTAPASS(2) (*Update)(void *data, int arg);
	void FP_FASTAPASS(3) (*SLHook)(uint8_t *bg, uint8_t *spr, uint32_t linets, int final);
	void FP_FASTAPASS(2) (*Draw)(uint8_t *buf, int arg);
} INPUTCFC;

void FCEU_DrawInput(uint8_t *buf);
void FCEU_UpdateInput(void);
void InitializeInput(void);
extern void (*PStrobe[2])(void);
extern void (*InputScanlineHook)(uint8_t *bg, uint8_t *spr, uint32_t linets, int final);

void FCEU_DoSimpleCommand(int cmd);

/* Input device initialisers. Defined in src/input/*.c, called from
 * src/input.c. Centralised here so each .c file can include this
 * header and get a matching prototype, satisfying
 * -Wmissing-prototypes. */
INPUTC   *FCEU_InitZapper(int w);
INPUTC   *FCEU_InitMouse(int w);
INPUTC   *FCEU_InitPowerpadA(int w);
INPUTC   *FCEU_InitPowerpadB(int w);
INPUTC   *FCEU_InitArkanoid(int w);
INPUTC   *FCEU_InitVirtualBoy(int w);
INPUTC   *FCEU_InitLCDCompZapper(int w);

INPUTCFC *FCEU_InitArkanoidFC(void);
INPUTCFC *FCEU_InitSpaceShadow(void);
INPUTCFC *FCEU_InitFKB(void);
INPUTCFC *FCEU_InitSuborKB(void);
INPUTCFC *FCEU_InitPEC586KB(void);
INPUTCFC *FCEU_InitHS(void);
INPUTCFC *FCEU_InitMahjong(void);
INPUTCFC *FCEU_InitFamilyTrainerA(void);
INPUTCFC *FCEU_InitFamilyTrainerB(void);
INPUTCFC *FCEU_InitOekaKids(void);
INPUTCFC *FCEU_InitTopRider(void);
INPUTCFC *FCEU_InitBarcodeWorld(void);
INPUTCFC *FCEU_InitBattleBox(void);
INPUTCFC *FCEU_InitQuizKing(void);
INPUTCFC *FCEU_InitFTrainerA(void);
INPUTCFC *FCEU_InitFTrainerB(void);

void FCEU_ZapperSetTolerance(int x);

#endif
