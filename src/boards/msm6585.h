#ifndef _MSM6585_H
#define _MSM6585_H
typedef struct {
	uint8 whichNibble;
	uint8 input;
	int16 signal;
	int32 count;
	int32 hostClock;
	int32 rate;
	int16 step;
	int (*getInput)(void);
} MSM6585;

void MSM6585_init (MSM6585*, int32, int (*)(void));
void MSM6585_reset (MSM6585*);
void MSM6585_setRate (MSM6585*, uint8);
void MSM6585_run (MSM6585*);
int32_t MSM6585_getOutput (MSM6585*);
#endif
