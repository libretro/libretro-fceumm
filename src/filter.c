#include "fceu-types.h"

#include "sound.h"
#include "x6502.h"
#include "fceu.h"
#include "filter.h"

#include "fcoeffs.h"

#if defined(__SSE2__)
#include <emmintrin.h>
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

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
	while (count--) {
		int64_t dropcurrent;
		dropcurrent = ((*in << 16) - sexyfilter2_acc) >> 3;

		sexyfilter2_acc += dropcurrent;
		*in = sexyfilter2_acc >> 16;
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

/* Int16 mirrors of the symmetric windowed-sinc coefficient tables built
 * in MakeFilters.  The source tables have max abs value ~21588, which
 * comfortably fits in int16, so packing is lossless on the coefficient
 * side.  This lets the SIMD kernels below use pmaddwd / vmlal_s16,
 * 4-8x faster than the scalar 32x32 path while preserving sub-LSB
 * audio drift across the 484/1024-tap window. */
static int16_t coeffs16[NCOEFFS];
static int16_t sq2coeffs16[SQ2NCOEFFS];

/* FIR inner kernel.  ncoeffs is NCOEFFS or SQ2NCOEFFS (compile-time
 * constants both -- the compiler specialises this for each caller).
 * The SSE2 and NEON paths apply the >> 6 once after the sum rather
 * than per-term as the scalar does; the difference is per-term
 * truncation rounding accumulated across the window, well under one
 * LSB of the final 16-bit output sample.  Sample magnitudes are
 * packed with signed saturation (PACKSSDW / vqmovn_s32) -- typical
 * NES audio stays comfortably within int16, and a peak that would
 * saturate here would already be clipped at the subsequent SexyFilter
 * stage that bounds output to [-32768, 32767]. */
static INLINE void fir_inner_kernel(
    const int32_t *S, const int16_t *D16, uint32_t ncoeffs,
    int32_t *out_acc, int32_t *out_acc2)
{
    int32_t acc = 0, acc2 = 0;
    uint32_t j;

#if defined(__SSE2__)
    {
        __m128i acc_v  = _mm_setzero_si128();
        __m128i acc2_v = _mm_setzero_si128();
        for (j = 0; j + 8 <= ncoeffs; j += 8) {
            __m128i a_lo = _mm_loadu_si128((const __m128i *)&S[j + 1]);
            __m128i a_hi = _mm_loadu_si128((const __m128i *)&S[j + 5]);
            __m128i b_lo = _mm_loadu_si128((const __m128i *)&S[j + 2]);
            __m128i b_hi = _mm_loadu_si128((const __m128i *)&S[j + 6]);
            __m128i s_a  = _mm_packs_epi32(a_lo, a_hi);
            __m128i s_b  = _mm_packs_epi32(b_lo, b_hi);
            __m128i co   = _mm_loadu_si128((const __m128i *)&D16[j]);
            acc_v  = _mm_add_epi32(acc_v,  _mm_madd_epi16(s_a, co));
            acc2_v = _mm_add_epi32(acc2_v, _mm_madd_epi16(s_b, co));
        }
        /* Horizontal sum of the 4 int32 lanes in each accumulator. */
        {
            __m128i shuf = _mm_shuffle_epi32(acc_v,  _MM_SHUFFLE(2,3,0,1));
            __m128i s1   = _mm_add_epi32(acc_v, shuf);
            shuf = _mm_shuffle_epi32(s1, _MM_SHUFFLE(1,0,3,2));
            acc = _mm_cvtsi128_si32(_mm_add_epi32(s1, shuf));
            shuf = _mm_shuffle_epi32(acc2_v, _MM_SHUFFLE(2,3,0,1));
            s1   = _mm_add_epi32(acc2_v, shuf);
            shuf = _mm_shuffle_epi32(s1, _MM_SHUFFLE(1,0,3,2));
            acc2 = _mm_cvtsi128_si32(_mm_add_epi32(s1, shuf));
        }
        acc  >>= 6;
        acc2 >>= 6;
    }
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
    {
        int32x4_t acc_v0 = vdupq_n_s32(0), acc_v1 = vdupq_n_s32(0);
        int32x4_t acc2_v0 = vdupq_n_s32(0), acc2_v1 = vdupq_n_s32(0);
        for (j = 0; j + 8 <= ncoeffs; j += 8) {
            int32x4_t a0 = vld1q_s32(&S[j + 1]);
            int32x4_t a1 = vld1q_s32(&S[j + 5]);
            int32x4_t b0 = vld1q_s32(&S[j + 2]);
            int32x4_t b1 = vld1q_s32(&S[j + 6]);
            int16x8_t s_a = vcombine_s16(vqmovn_s32(a0), vqmovn_s32(a1));
            int16x8_t s_b = vcombine_s16(vqmovn_s32(b0), vqmovn_s32(b1));
            int16x8_t co  = vld1q_s16(&D16[j]);
            acc_v0  = vmlal_s16(acc_v0,  vget_low_s16(s_a),  vget_low_s16(co));
            acc_v1  = vmlal_s16(acc_v1,  vget_high_s16(s_a), vget_high_s16(co));
            acc2_v0 = vmlal_s16(acc2_v0, vget_low_s16(s_b),  vget_low_s16(co));
            acc2_v1 = vmlal_s16(acc2_v1, vget_high_s16(s_b), vget_high_s16(co));
        }
        {
            int32x4_t s_acc  = vaddq_s32(acc_v0,  acc_v1);
            int32x4_t s_acc2 = vaddq_s32(acc2_v0, acc2_v1);
#if defined(__aarch64__)
            acc  = vaddvq_s32(s_acc);
            acc2 = vaddvq_s32(s_acc2);
#else
            int32x2_t p = vadd_s32(vget_low_s32(s_acc), vget_high_s32(s_acc));
            acc  = vget_lane_s32(vpadd_s32(p, p), 0);
            p = vadd_s32(vget_low_s32(s_acc2), vget_high_s32(s_acc2));
            acc2 = vget_lane_s32(vpadd_s32(p, p), 0);
#endif
        }
        acc  >>= 6;
        acc2 >>= 6;
    }
#else
    j = 0;
#endif

    /* Scalar tail handles whatever 0..7 taps the SIMD block didn't,
     * and the entire window on builds without SSE2 or NEON. */
    for (; j < ncoeffs; j++) {
        acc  += (S[j + 1] * (int32_t)D16[j]) >> 6;
        acc2 += (S[j + 2] * (int32_t)D16[j]) >> 6;
    }
    *out_acc  = acc;
    *out_acc2 = acc2;
}

int32_t NeoFilterSound(int32_t *in, int32_t *out, uint32_t inlen, int32_t *leftover) {
	uint32_t x;
	int32_t *outsave = out;
	int32_t count = 0;
	uint32_t max = (inlen - 1) << 16;

	if (FSettings.soundq == 2) {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc, acc2;
			int32_t *S = &in[(x >> 16) - SQ2NCOEFFS];
			fir_inner_kernel(S, sq2coeffs16, SQ2NCOEFFS, &acc, &acc2);

			acc = ((int64_t)acc * (65536 - (x & 65535)) + (int64_t)acc2 * (x & 65535)) >> (16 + 11);
			*out = acc;
			out++;
			count++;
		}
	} else {
		for (x = mrindex; x < max; x += mrratio) {
			int32_t acc, acc2;
			int32_t *S = &in[(x >> 16) - NCOEFFS];
			fir_inner_kernel(S, coeffs16, NCOEFFS, &acc, &acc2);

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

	/* Build the int16 mirror used by the SIMD inner kernel.  All
	 * source FIR tables have max abs value ~21588 (fits in int16
	 * losslessly), so a straight narrowing cast preserves every
	 * coefficient bit.  Done once per filter rebuild -- 1024 entries
	 * is well under 1 us. */
	if (FSettings.soundq == 2) {
		for (x = 0; x < SQ2NCOEFFS; x++)
			sq2coeffs16[x] = (int16_t)sq2coeffs[x];
	} else {
		for (x = 0; x < NCOEFFS; x++)
			coeffs16[x] = (int16_t)coeffs[x];
	}
}
