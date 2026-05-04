#ifndef _FCEU_FILTER_H
#define _FCEU_FILTER_H

int32_t NeoFilterSound(int32_t *in, int32_t *out, uint32_t inlen, int32_t *leftover);
void MakeFilters(int32_t rate);
void SexyFilter(int32_t *in, int32_t *out, int32_t count);
void SexyFilter2(int32_t *in, int32_t count);

/* Zero the IIR filter state. The accumulators are saved/loaded with
 * the rest of the sound state (sound.c FAC1/FAC2 entries), but they
 * are not reinitialised by FCEUSND_Power on cart load. Without this,
 * starting cart B in a process that previously ran cart A inherits
 * cart A's IIR state - audibly minor but breaks frame-determinism for
 * the first samples of cart B. */
void SexyFilter_Reset(void);

extern int64_t sexyfilter_acc1, sexyfilter_acc2;
extern int64_t sexyfilter2_acc;

#endif
