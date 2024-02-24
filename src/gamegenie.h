#ifndef _FCEU_GAMEGENIE_H
#define _FCEU_GAMEGENIE_H

extern int geniestage;
extern readfunc *AReadG;
extern writefunc *BWriteG;
extern int RWWrap;

void FCEU_OpenGenie(void);
void FCEU_GeniePower(void);
void FCEU_CloseGenie(void);
void FCEU_KillGenie(void);

#endif /* _FCEU_GAMEGENIE_H */