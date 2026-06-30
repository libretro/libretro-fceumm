#include "fceu-types.h"

#include "sound.h"
#include "x6502.h"
#include "fceu.h"
#include "filter.h"

#include "fcoeffs.h"

/* Reference rate at which SexyFilter2's one-pole reproduces the legacy
 * hardcoded ">>3" coefficient (1/8 == 8192 in Q16).  The coefficient is
 * scaled by SEXYFILTER2_REF_RATE / SndRate so the corner frequency stays
 * fixed in Hz across every output rate the core selects, instead of
 * drifting with the rate as the old fixed shift did.  Changing this value
 * retunes the corner (it sits at ~0.0199 * SEXYFILTER2_REF_RATE Hz, i.e.
 * ~955 Hz at 48000). */
#define SEXYFILTER2_REF_RATE 48000

static uint32_t mrindex;
static uint32_t mrratio;

int64_t sexyfilter_acc1 = 0, sexyfilter_acc2 = 0;

/* SexyFilter2's separate accumulator. Like sexyfilter_acc1/_acc2 it
 * was a function-local static, which meant FCEUSND_Power couldn't
 * reset it - cart B in a long-running process would inherit cart A's
 * lowpass state. Lifted to file scope alongside the others so
 * SexyFilter_Reset can zero it too. */
int64_t sexyfilter2_acc = 0;

void SexyFilter_Reset(void)
{
   sexyfilter_acc1 = 0;
   sexyfilter_acc2 = 0;
   sexyfilter2_acc = 0;
   /* mrindex is reset in MakeFilters, which FCEUI_Sound calls on
    * every cart load, so it doesn't need to be reset here. */
}

void SexyFilter2(int32_t *in, int32_t count) {
	int64_t  coeff;
	uint32_t rate = FSettings.SndRate ? FSettings.SndRate
	                                  : SEXYFILTER2_REF_RATE;

	/* Derive the one-pole coefficient from the output rate so the corner
	 * frequency is rate-invariant.  coeff == 8192 (the legacy 1/8) exactly
	 * at rate == SEXYFILTER2_REF_RATE. */
	coeff = ((int64_t)8192 * SEXYFILTER2_REF_RATE + (rate >> 1)) / rate;

	/* A stable one-pole requires 0 < coeff <= 65536 (65536 == passthrough).
	 * The rates this core offers (>= 32 kHz) keep coeff in ~4096..12288;
	 * clamp defensively so an unusual rate can never push the pole onto or
	 * outside the unit circle. */
	if (coeff < 1)     coeff = 1;
	if (coeff > 65536) coeff = 65536;

	while (count--) {
		int64_t diff;
		int64_t dropcurrent;

		diff        = ((int64_t)*in << 16) - sexyfilter2_acc;
		/* Round to nearest instead of truncating toward -inf, so the
		 * feedback path neither accumulates a sub-LSB downward bias nor
		 * settles to a stuck negative offset on silence. */
		dropcurrent = (diff * coeff + (1 << 15)) >> 16;

		sexyfilter2_acc += dropcurrent;
		*in = (int32_t)(sexyfilter2_acc >> 16);
		in++;
	}
}

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
		/* Round to nearest in the recursive updates and in the output
		 * quantisation rather than truncating toward -inf.  This removes
		 * the systematic sub-LSB downward bias the floor shifts impart to
		 * the feedback state.  mul1/mul2 (the pole placement) are
		 * untouched, so the filter's frequency response/voicing is
		 * identical; only the quantisation noise floor is tightened. */
		sexyfilter_acc1 += ((ino - sexyfilter_acc1) * mul1 + (1 << 15)) >> 16;
		sexyfilter_acc2 += ((ino - sexyfilter_acc1 - sexyfilter_acc2) * mul2 + (1 << 15)) >> 16;
		/* Clearing the source sample is load-bearing only in the LQ call
		 * SexyFilter(Wave, WaveFinal, ...): it resets Wave for the next
		 * frame's accumulation.  In the HQ in-place call (out == in) it is
		 * a dead store immediately overwritten by *out below.  Do NOT
		 * remove it - the LQ accumulation path depends on it. */
		*in = 0;
		{
			int32_t t = (int32_t)((sexyfilter_acc1 - ino + sexyfilter_acc2
			            + (1 << 15)) >> 16);
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
	int32_t *tabs[8] = { C44100NTSC, C44100PAL, C48000NTSC, C48000PAL, C96000NTSC,
					   C96000PAL, C32000NTSC, C32000PAL };
	int32_t *sq2tabs[8] = { SQ2C44100NTSC, SQ2C44100PAL, SQ2C48000NTSC, SQ2C48000PAL,
						  SQ2C96000NTSC, SQ2C96000PAL, SQ2C32000NTSC, SQ2C32000PAL };

	int32_t *tmp;
	int32_t x;
	uint32_t nco;
	uint32_t idx;

	if (FSettings.soundq == 2)
		nco = SQ2NCOEFFS;
	else
		nco = NCOEFFS;

	mrindex = (nco + 1) << 16;
	mrratio = (PAL ? (int64_t)(PAL_CPU * 65536) : (int64_t)(NTSC_CPU * 65536)) / rate;

	/* Select the coefficient table matched to the output rate. Bit 0 is
	 * the PAL/NTSC region; the rate selects the base index. Rates without
	 * a dedicated table fall back to the 44100 design (index 0/1). */
	if (rate == 48000)
		idx = 2;
	else if (rate == 96000)
		idx = 4;
	else if (rate == 32000)
		idx = 6;
	else /* 44100 and any other rate */
		idx = 0;
	idx |= (PAL ? 1 : 0);

	if (FSettings.soundq == 2)
		tmp = sq2tabs[idx];
	else
		tmp = tabs[idx];

	if (FSettings.soundq == 2)
		for (x = 0; x < (SQ2NCOEFFS >> 1); x++)
			sq2coeffs[x] = sq2coeffs[SQ2NCOEFFS - 1 - x] = tmp[x];
	else
		for (x = 0; x < (NCOEFFS >> 1); x++)
			coeffs[x] = coeffs[NCOEFFS - 1 - x] = tmp[x];
}
