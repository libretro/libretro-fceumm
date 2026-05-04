#ifndef _VRCIRQ_H
#define _VRCIRQ_H

void VRCIRQ_Init(void);
void VRCIRQ_Latch(uint8_t V);
void VRCIRQ_LatchNibble(uint8_t V, uint8_t highBit);
void VRCIRQ_Control(uint8_t V);
void VRCIRQ_Acknowledge(void);

#endif /* _VRCIRQ_H */