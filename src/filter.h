#ifndef _FCEU_FILTER_H
#define _FCEU_FILTER_H

int32_t NeoFilterSound(int32_t *in, int32_t *out, uint32_t inlen, int32_t *leftover);
void MakeFilters(int32_t rate);
void SexyFilter(int32_t *in, int32_t *out, int32_t count);
void SexyFilter2(int32_t *in, int32_t count);

extern int64_t sexyfilter_acc1, sexyfilter_acc2;

#endif
