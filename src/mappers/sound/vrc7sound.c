/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "mapinc.h"
#include "emu2413.h"
#include "vrc7sound.h"

static int32 dwave = 0;
static uint8 vrc7idx;

static OPLL *VRC7Sound = NULL;

static void OPLL_fillbuf(OPLL *opll, int32 *buf, int32 len, int shift) {
	while (len > 0) {
		*buf += GetVolume(APU_VRC7, (OPLL_calc(opll) << shift));
		buf++;
		len--;
	}
}

void DoVRC7Sound(void) {
	int32 z, a;
	if (FSettings.soundq >= 1) {
		return;
    }
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	OPLL_fillbuf(VRC7Sound, &Wave[dwave], a, 1);
	dwave += a;
}

static void UpdateOPLNEO(int32 *Wave, int Count) {
	OPLL_fillbuf(VRC7Sound, Wave, Count, 4);
}

static void UpdateOPL(int Count) {
	int32 z, a;
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	if (VRC7Sound && a) {
		OPLL_fillbuf(VRC7Sound, &Wave[dwave], a, 1);
    }
	dwave = 0;
}

static void VRC7SC(void) {
	if (VRC7Sound) {
		OPLL_set_rate(VRC7Sound, FSettings.SndRate);
    }
}

static void VRC7SKill(void) {
	if (VRC7Sound) {
		OPLL_delete(VRC7Sound);
    }
	VRC7Sound = NULL;
}

DECLFW(VRC7Sound_Write) {
    switch (A & 0xF030) {
    case 0x9010:
        vrc7idx = V;
        break;
	case 0x9030:
		if (FSettings.SndRate) {
			OPLL_writeReg(VRC7Sound, vrc7idx, V);
			GameExpSound.Fill = UpdateOPL;
			GameExpSound.NeoFill = UpdateOPLNEO;
		}
		break;
	}
}

void VRC7Sound_StateRestore(void) {
    OPLL_forceRefresh(VRC7Sound);
}

void VRC7Sound_ESI(void) {
	GameExpSound.RChange = VRC7SC;
	GameExpSound.Kill = VRC7SKill;
	VRC7Sound = OPLL_new(3579545, FSettings.SndRate ? FSettings.SndRate : 44100);
	OPLL_setChipType(VRC7Sound, OPLL_VRC7_TONE);
	OPLL_resetPatch(VRC7Sound, OPLL_VRC7_TONE);

	/* Sound states */
    AddExState(&vrc7idx, sizeof(vrc7idx), 0, "VRCI");
	AddExState(&VRC7Sound->clk, sizeof(VRC7Sound->clk), 0, "CLK7");
	AddExState(&VRC7Sound->rate, sizeof(VRC7Sound->rate), 0, "RATE");

	AddExState(&VRC7Sound->chip_type, sizeof(VRC7Sound->chip_type), 0, "CHIP");

	AddExState(&VRC7Sound->adr, sizeof(VRC7Sound->adr), 0, "ADR7");

	AddExState(&VRC7Sound->inp_step, sizeof(VRC7Sound->inp_step), 0, "ISTP");
	AddExState(&VRC7Sound->out_step, sizeof(VRC7Sound->out_step), 0, "OSTP");
	AddExState(&VRC7Sound->out_time, sizeof(VRC7Sound->out_time), 0, "OTME");

	AddExState(VRC7Sound->reg, sizeof(VRC7Sound->reg), 0, "REG7");
	AddExState(&VRC7Sound->test_flag, sizeof(VRC7Sound->test_flag), 0, "TFLG");
	AddExState(&VRC7Sound->slot_key_status, sizeof(VRC7Sound->slot_key_status), 0, "SKST");
	AddExState(&VRC7Sound->rhythm_mode, sizeof(VRC7Sound->rhythm_mode), 0, "RMOD");

	AddExState(&VRC7Sound->eg_counter, sizeof(VRC7Sound->eg_counter), 0, "ECTR");

	AddExState(&VRC7Sound->pm_phase, sizeof(VRC7Sound->pm_phase), 0, "PMPH");
	AddExState(&VRC7Sound->am_phase, sizeof(VRC7Sound->am_phase), 0, "AMPH");

	AddExState(&VRC7Sound->lfo_am, sizeof(VRC7Sound->lfo_am), 0, "LFO7");

	AddExState(&VRC7Sound->noise, sizeof(VRC7Sound->noise), 0, "NOIS");
	AddExState(&VRC7Sound->short_noise, sizeof(VRC7Sound->short_noise), 0, "SNOS");

	AddExState(VRC7Sound->patch_number, sizeof(VRC7Sound->patch_number), 0, "PTNM");

	/* VRC7 only uses 12 slots */
	AddExState(&VRC7Sound->slot[0], sizeof(VRC7Sound->slot[0]), 0, "SL00");
    AddExState(&VRC7Sound->slot[1], sizeof(VRC7Sound->slot[1]), 0, "SL01");
    AddExState(&VRC7Sound->slot[2], sizeof(VRC7Sound->slot[2]), 0, "SL02");
    AddExState(&VRC7Sound->slot[3], sizeof(VRC7Sound->slot[3]), 0, "SL03");
    AddExState(&VRC7Sound->slot[4], sizeof(VRC7Sound->slot[4]), 0, "SL04");
    AddExState(&VRC7Sound->slot[5], sizeof(VRC7Sound->slot[5]), 0, "SL05");
    AddExState(&VRC7Sound->slot[6], sizeof(VRC7Sound->slot[6]), 0, "SL06");
    AddExState(&VRC7Sound->slot[7], sizeof(VRC7Sound->slot[7]), 0, "SL07");
    AddExState(&VRC7Sound->slot[8], sizeof(VRC7Sound->slot[8]), 0, "SL08");
    AddExState(&VRC7Sound->slot[9], sizeof(VRC7Sound->slot[9]), 0, "SL09");
    AddExState(&VRC7Sound->slot[10], sizeof(VRC7Sound->slot[10]), 0, "SL10");
    AddExState(&VRC7Sound->slot[11], sizeof(VRC7Sound->slot[11]), 0, "SL11");

	AddExState(&VRC7Sound->mask, sizeof(VRC7Sound->mask), 0, "MASK");

	AddExState(VRC7Sound->ch_out, sizeof(VRC7Sound->ch_out), 0, "CHOU");
	AddExState(VRC7Sound->mix_out, sizeof(VRC7Sound->mix_out), 0, "MIXO");

	/* custom patches */
	AddExState(&VRC7Sound->patch[0], sizeof(VRC7Sound->patch[0]), 0, "PAT0");
	AddExState(&VRC7Sound->patch[1], sizeof(VRC7Sound->patch[1]), 0, "PAT1");
}

/* VRC7 Sound */
