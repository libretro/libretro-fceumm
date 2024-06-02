#ifndef _VRCIRQ_H
#define _VRCIRQ_H

void VRCIRQ_Init(void);
void VRCIRQ_Latch(uint8 V);
void VRCIRQ_LatchNibble(uint8 V, uint8 highBit);
void VRCIRQ_Control(uint8 V);
void VRCIRQ_Acknowledge(void);

#endif /* _VRCIRQ_H */