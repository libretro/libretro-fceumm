#ifndef _MSM6585_H
#define _MSM6585_H
typedef struct {
	uint8_t whichNibble;
	uint8_t input;
	int16_t signal;
	int32_t count;
	int32_t hostClock;
	int32_t rate;
	int16_t step;
	int (*getInput)(void);
} MSM6585;

void MSM6585_init (MSM6585*, int32_t, int (*)(void));
void MSM6585_reset (MSM6585*);
void MSM6585_setRate (MSM6585*, uint8_t);
void MSM6585_run (MSM6585*);
int32_t MSM6585_getOutput (MSM6585*);
#endif
