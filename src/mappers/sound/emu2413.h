#ifndef _EMU2413_H
#define _EMU2413_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPLL_DEBUG 0

enum OPLL_TONE_ENUM { OPLL_2413_TONE = 0, OPLL_VRC7_TONE = 1, OPLL_281B_TONE = 2 };

/* voice data */
typedef struct __OPLL_PATCH {
  uint32 TL, FB, EG, ML, AR, DR, SL, RR, KR, KL, AM, PM, WS;
} OPLL_PATCH;

/* slot */
typedef struct __OPLL_SLOT {
  uint8 number;

  /* type flags:
   * 000000SM
   *       |+-- M: 0:modulator 1:carrier
   *       +--- S: 0:normal 1:single slot mode (sd, tom, hh or cym)
   */
  uint8 type;

  OPLL_PATCH *patch; /* voice parameter */

  /* slot output */
  int32 output[2]; /* output value, latest and previous. */

  /* phase generator (pg) */
  uint16 *wave_table; /* wave table */
  uint32 pg_phase;    /* pg phase */
  uint32 pg_out;      /* pg output, as index of wave table */
  uint8 pg_keep;      /* if 1, pg_phase is preserved when key-on */
  uint16 blk_fnum;    /* (block << 9) | f-number */
  uint16 fnum;        /* f-number (9 bits) */
  uint8 blk;          /* block (3 bits) */

  /* envelope generator (eg) */
  uint8 eg_state;  /* current state */
  int32 volume;    /* current volume */
  uint8 key_flag;  /* key-on flag 1:on 0:off */
  uint8 sus_flag;  /* key-sus option 1:on 0:off */
  uint16 tll;      /* total level + key scale level*/
  uint8 rks;       /* key scale offset (rks) for eg speed */
  uint8 eg_rate_h; /* eg speed rate high 4bits */
  uint8 eg_rate_l; /* eg speed rate low 2bits */
  uint32 eg_shift; /* shift for eg global counter, controls envelope speed */
  uint32 eg_out;   /* eg output */

  uint32 update_requests; /* flags to debounce update */

#if OPLL_DEBUG
  uint8 last_eg_state;
#endif
} OPLL_SLOT;

/* mask */
#define OPLL_MASK_CH(x) (1 << (x))
#define OPLL_MASK_HH (1 << (9))
#define OPLL_MASK_CYM (1 << (10))
#define OPLL_MASK_TOM (1 << (11))
#define OPLL_MASK_SD (1 << (12))
#define OPLL_MASK_BD (1 << (13))
#define OPLL_MASK_RHYTHM (OPLL_MASK_HH | OPLL_MASK_CYM | OPLL_MASK_TOM | OPLL_MASK_SD | OPLL_MASK_BD)

/* rate conveter */
typedef struct __OPLL_RateConv {
  int ch;
  double timer;
  double f_ratio;
  int16 *sinc_table;
  int16 **buf;
} OPLL_RateConv;

OPLL_RateConv *OPLL_RateConv_new(double f_inp, double f_out, int ch);
void OPLL_RateConv_reset(OPLL_RateConv *conv);
void OPLL_RateConv_putData(OPLL_RateConv *conv, int ch, int16 data);
int16 OPLL_RateConv_getData(OPLL_RateConv *conv, int ch);
void OPLL_RateConv_delete(OPLL_RateConv *conv);

typedef struct __OPLL {
  uint32 clk;
  uint32 rate;

  uint8 chip_type;

  uint32 adr;

  double inp_step;
  double out_step;
  double out_time;

  uint8 reg[0x40];
  uint8 test_flag;
  uint32 slot_key_status;
  uint8 rhythm_mode;

  uint32 eg_counter;

  uint32 pm_phase;
  int32 am_phase;

  uint8 lfo_am;

  uint32 noise;
  uint8 short_noise;

  int32 patch_number[9];
  OPLL_SLOT slot[18];
  OPLL_PATCH patch[19 * 2];

  uint8 pan[16];
  float pan_fine[16][2];

  uint32 mask;

  /* channel output */
  /* 0..8:tone 9:bd 10:hh 11:sd 12:tom 13:cym */
  int16 ch_out[14];

  int16 mix_out[2];

  OPLL_RateConv *conv;
} OPLL;

OPLL *OPLL_new(uint32 clk, uint32 rate);
void OPLL_delete(OPLL *);

void OPLL_reset(OPLL *);
void OPLL_resetPatch(OPLL *, uint8_t);

/**
 * Set output wave sampling rate.
 * @param rate sampling rate. If clock / 72 (typically 49716 or 49715 at 3.58MHz) is set, the internal rate converter is
 * disabled.
 */
void OPLL_setRate(OPLL *opll, uint32 rate);

/**
 * Set internal calcuration quality. Currently no effects, just for compatibility.
 * >= v1.0.0 always synthesizes internal output at clock/72 Hz.
 */
void OPLL_setQuality(OPLL *opll, uint8 q);

/**
 * Set pan pot (extra function - not YM2413 chip feature)
 * @param ch 0..8:tone 9:bd 10:hh 11:sd 12:tom 13:cym 14,15:reserved
 * @param pan 0:mute 1:right 2:left 3:center
 * ```
 * pan: 76543210
 *            |+- bit 1: enable Left output
 *            +-- bit 0: enable Right output
 * ```
 */
void OPLL_setPan(OPLL *opll, uint32 ch, uint8 pan);

/**
 * Set fine-grained panning
 * @param ch 0..8:tone 9:bd 10:hh 11:sd 12:tom 13:cym 14,15:reserved
 * @param pan output strength of left/right channel.
 *            pan[0]: left, pan[1]: right. pan[0]=pan[1]=1.0f for center.
 */
void OPLL_setPanFine(OPLL *opll, uint32 ch, float pan[2]);

/**
 * Set chip type. If vrc7 is selected, r#14 is ignored.
 * This method not change the current ROM patch set.
 * To change ROM patch set, use OPLL_resetPatch.
 * @param type 0:YM2413 1:VRC7
 */
void OPLL_setChipType(OPLL *opll, uint8 type);

void OPLL_writeIO(OPLL *opll, uint32 reg, uint8 val);
void OPLL_writeReg(OPLL *opll, uint32 reg, uint8 val);

/**
 * Calculate one sample
 */
int16 OPLL_calc(OPLL *opll);

/**
 * Calulate stereo sample
 */
void OPLL_calcStereo(OPLL *opll, int32 out[2]);

void OPLL_setPatch(OPLL *, const uint8 *dump);
void OPLL_copyPatch(OPLL *, int32_t, OPLL_PATCH *);

/**
 * Force to refresh.
 * External program should call this function after updating patch parameters.
 */
void OPLL_forceRefresh(OPLL *);

void OPLL_dumpToPatch(const uint8 *dump, OPLL_PATCH *patch);
void OPLL_patchToDump(const OPLL_PATCH *patch, uint8 *dump);
void OPLL_getDefaultPatch(int32 type, int32 num, OPLL_PATCH *);

/**
 *  Set channel mask
 *  @param mask mask flag: OPLL_MASK_* can be used.
 *  - bit 0..8: mask for ch 1 to 9 (OPLL_MASK_CH(i))
 *  - bit 9: mask for Hi-Hat (OPLL_MASK_HH)
 *  - bit 10: mask for Top-Cym (OPLL_MASK_CYM)
 *  - bit 11: mask for Tom (OPLL_MASK_TOM)
 *  - bit 12: mask for Snare Drum (OPLL_MASK_SD)
 *  - bit 13: mask for Bass Drum (OPLL_MASK_BD)
 */
uint32 OPLL_setMask(OPLL *, uint32 mask);

/**
 * Toggler channel mask flag
 */
uint32 OPLL_toggleMask(OPLL *, uint32 mask);

/* for compatibility */
#define OPLL_set_rate OPLL_setRate
#define OPLL_set_quality OPLL_setQuality
#define OPLL_set_pan OPLL_setPan
#define OPLL_set_pan_fine OPLL_setPanFine
#define OPLL_calc_stereo OPLL_calcStereo
#define OPLL_reset_patch OPLL_resetPatch
#define OPLL_dump2patch OPLL_dumpToPatch
#define OPLL_patch2dump OPLL_patchToDump
#define OPLL_setChipMode OPLL_setChipType

#ifdef __cplusplus
}
#endif

#endif /* _EMU2413_H */
