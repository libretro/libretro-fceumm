#include <math.h>
#include "fceu-types.h"

#include "sound.h"
#include "x6502.h"
#include "fceu.h"
#include "filter.h"

#include "fcoeffs.h"

static uint32_t mrindex;
static uint32_t mrratio;

void SexyFilter2(int32_t *in, int32_t count) {
 #ifdef moo
	static int64_t acc = 0;
	double x, p;
	int64_t c;

	x = 2 * M_PI * 6000 / FSettings.SndRate;
	p = ((double)2 - cos(x)) - sqrt(pow((double)2 - cos(x), 2) - 1);

	c = p * 0x100000;
 #endif
	static int64_t acc = 0;

	while (count--) {
		int64_t dropcurrent;
		dropcurrent = ((*in << 16) - acc) >> 3;

		acc += dropcurrent;
		*in = acc >> 16;
		in++;
#if 0
		 acc=((int64_t)0x100000-c)* *in + ((c*acc)>>20);
		*in=acc>>20;
		in++;
#endif
	}
}

int64_t sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;

void SexyFilter(int32_t *in, int32_t *out, int32_t count) {
	int32_t mul1, mul2, vmul;

	mul1 = (94 << 16) / FSettings.SndRate;
	mul2 = (24 << 16) / FSettings.SndRate;
	vmul = (FSettings.SoundVolume << 16) * 3 / 4 / 100;

	if (FSettings.soundq)
		vmul /= 4;
	else
		vmul *= 2;	/* TODO:  Increase volume in low quality sound rendering code itself */

	while (count) {
		int64_t ino = (int64_t) * in * vmul;
		sexyfilter_acc1 += ((ino - sexyfilter_acc1) * mul1) >> 16;
		sexyfilter_acc2 += ((ino - sexyfilter_acc1 - sexyfilter_acc2) * mul2) >> 16;
		*in = 0;
		{
			int32_t t = (sexyfilter_acc1 - ino + sexyfilter_acc2) >> 16;
			if (t > 32767) t = 32767;
			if (t < -32768) t = -32768;
			*out = t;
		}
		in++;
		out++;
		count--;
	}
}

/* Returns number of samples written to out. */
/* leftover is set to the number of samples that need to be copied
	from the end of in to the beginning of in.
*/

/* static uint32_t mva=1000; */

/* This filtering code assumes that almost all input values stay below 32767.
	Do not adjust the volume in the wlookup tables and the expansion sound
	code to be higher, or you *might* overflow the FIR code.
*/

int32_t NeoFilterSound(int32_t *in, int32_t *out, uint32_t inlen, int32_t *leftover) {
	uint32_t x;
	int32_t *outsave = out;
	int32_t count = 0;
	uint32_t max = (inlen - 1) << 16;

	if (FSettings.soundq == 2) {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc = 0, acc2 = 0;
			uint32_t c;
			int32_t *S, *D;

			for (c = SQ2NCOEFFS, S = &in[(x >> 16) - SQ2NCOEFFS], D = sq2coeffs; c; c--, D++) {
				acc += (S[c] * *D) >> 6;
				acc2 += (S[1 + c] * *D) >> 6;
			}

			acc = ((int64_t)acc * (65536 - (x & 65535)) + (int64_t)acc2 * (x & 65535)) >> (16 + 11);
			*out = acc;
			out++;
			count++;
		}
	} else {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc = 0, acc2 = 0;
			uint32_t c;
			int32_t *S, *D;

			for (c = NCOEFFS, S = &in[(x >> 16) - NCOEFFS], D = coeffs; c; c--, D++) {
				acc += (S[c] * *D) >> 6;
				acc2 += (S[1 + c] * *D) >> 6;
			}

			acc = ((int64_t)acc * (65536 - (x & 65535)) + (int64_t)acc2 * (x & 65535)) >> (16 + 11);
			*out = acc;
			out++;
			count++;
		}
	}

	mrindex = x - max;

	if (FSettings.soundq == 2) {
		mrindex += SQ2NCOEFFS * 65536;
		*leftover = SQ2NCOEFFS + 1;
	} else {
		mrindex += NCOEFFS * 65536;
		*leftover = NCOEFFS + 1;
	}

	if (GameExpSound.NeoFill)
		GameExpSound.NeoFill(outsave, count);

	SexyFilter(outsave, outsave, count);
	if (FSettings.lowpass)
		SexyFilter2(outsave, count);
	return(count);
}

void MakeFilters(int32_t rate) {
	int32_t *tabs[6] = { C44100NTSC, C44100PAL, C48000NTSC, C48000PAL, C96000NTSC,
					   C96000PAL };
	int32_t *sq2tabs[6] = { SQ2C44100NTSC, SQ2C44100PAL, SQ2C48000NTSC, SQ2C48000PAL,
						  SQ2C96000NTSC, SQ2C96000PAL };

	int32_t *tmp;
	int32_t x;
	uint32_t nco;

	if (FSettings.soundq == 2)
		nco = SQ2NCOEFFS;
	else
		nco = NCOEFFS;

	mrindex = (nco + 1) << 16;
	mrratio = (PAL ? (int64_t)(PAL_CPU * 65536) : (int64_t)(NTSC_CPU * 65536)) / rate;

	if (FSettings.soundq == 2)
		tmp = sq2tabs[(PAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate == 96000 ? 4 : 0)];
	else
		tmp = tabs[(PAL ? 1 : 0) | (rate == 48000 ? 2 : 0) | (rate == 96000 ? 4 : 0)];

	if (FSettings.soundq == 2)
		for (x = 0; x < (SQ2NCOEFFS >> 1); x++)
			sq2coeffs[x] = sq2coeffs[SQ2NCOEFFS - 1 - x] = tmp[x];
	else
		for (x = 0; x < (NCOEFFS >> 1); x++)
			coeffs[x] = coeffs[NCOEFFS - 1 - x] = tmp[x];
}
