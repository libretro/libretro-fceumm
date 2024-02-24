/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2024 negativeExponent
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

static OPLL *chip = NULL;

static void OPLL_fillbuf(OPLL *opll, int32 *buf, int32 len, int shift) {
	while (len > 0) {
		*buf += GetOutput(SND_VRC7, (OPLL_calc(chip) + 32768)) << shift;
		buf++;
		len--;
	}
}

static void UpdateOPLNEO(int32 *buf, int Count) {
	OPLL_fillbuf(chip, buf, Count, 4);
}

static void UpdateOPL(int Count) {
	int32 z, a;
	z = ((SOUNDTS << 16) / soundtsinc) >> 4;
	a = z - dwave;
	if (a) {
		OPLL_fillbuf(chip, &Wave[dwave], a, 1);
    }
	dwave = 0;
}

DECLFW(VRC7Sound_Write) {
	if (!chip) {
		return;
	}

    switch (A & 0xF030) {
    case 0x9010:
		OPLL_writeIO(chip, 0, V);
        break;
	case 0x9030:
		GameExpSound[SND_VRC7 - 6].Fill = UpdateOPL;
		GameExpSound[SND_VRC7 - 6].NeoFill = UpdateOPLNEO;
		OPLL_writeIO(chip, 1, V);
		break;
	}
}

static void VRC7SC(void) {
	if (chip) {
		OPLL_setChipType(chip, OPLL_VRC7_TONE);
		OPLL_setRate(chip, FSettings.SndRate ? FSettings.SndRate : 44100);
		OPLL_resetPatch(chip, OPLL_VRC7_TONE);
	}
}

static void VRC7SKill(void) {
	if (chip) {
		OPLL_delete(chip);
		chip = NULL;
	}
}

void VRC7Sound_ESI(void) {
	if (!(chip = OPLL_new(3579545, FSettings.SndRate ? FSettings.SndRate : 44100))) {
		return;
	}

	GameExpSound[SND_VRC7 - 6].RChange = VRC7SC;
	GameExpSound[SND_VRC7 - 6].Kill = VRC7SKill;

	VRC7SC();
}

void VRC7Sound_AddStateInfo(void) {
	if (!chip) {
		return;
	}

	/* Sound states */
	AddExState(&chip->clk, sizeof(chip->clk), 0, "CLK7");
	AddExState(&chip->rate, sizeof(chip->rate), 0, "RATE");

	AddExState(&chip->chip_type, sizeof(chip->chip_type), 0, "CHIP");

	AddExState(&chip->adr, sizeof(chip->adr), 0, "ADR7");

	AddExState(&chip->inp_step, sizeof(chip->inp_step), 0, "ISTP");
	AddExState(&chip->out_step, sizeof(chip->out_step), 0, "OSTP");
	AddExState(&chip->out_time, sizeof(chip->out_time), 0, "OTME");

	AddExState(chip->reg, sizeof(chip->reg), 0, "REG7");
	AddExState(&chip->test_flag, sizeof(chip->test_flag), 0, "TFLG");
	AddExState(&chip->slot_key_status, sizeof(chip->slot_key_status), 0, "SKST");
	AddExState(&chip->rhythm_mode, sizeof(chip->rhythm_mode), 0, "RMOD");

	AddExState(&chip->eg_counter, sizeof(chip->eg_counter), 0, "ECTR");

	AddExState(&chip->pm_phase, sizeof(chip->pm_phase), 0, "PMPH");
	AddExState(&chip->am_phase, sizeof(chip->am_phase), 0, "AMPH");

	AddExState(&chip->lfo_am, sizeof(chip->lfo_am), 0, "LFO7");

	AddExState(&chip->noise, sizeof(chip->noise), 0, "NOIS");
	AddExState(&chip->short_noise, sizeof(chip->short_noise), 0, "SNOS");

	AddExState(chip->patch_number, sizeof(chip->patch_number), 0, "PTNM");

	/* VRC7 only uses 12 slots */
	AddExState(&chip->slot[0], sizeof(chip->slot[0]), 0, "SL00");
    AddExState(&chip->slot[1], sizeof(chip->slot[1]), 0, "SL01");
    AddExState(&chip->slot[2], sizeof(chip->slot[2]), 0, "SL02");
    AddExState(&chip->slot[3], sizeof(chip->slot[3]), 0, "SL03");
    AddExState(&chip->slot[4], sizeof(chip->slot[4]), 0, "SL04");
    AddExState(&chip->slot[5], sizeof(chip->slot[5]), 0, "SL05");
    AddExState(&chip->slot[6], sizeof(chip->slot[6]), 0, "SL06");
    AddExState(&chip->slot[7], sizeof(chip->slot[7]), 0, "SL07");
    AddExState(&chip->slot[8], sizeof(chip->slot[8]), 0, "SL08");
    AddExState(&chip->slot[9], sizeof(chip->slot[9]), 0, "SL09");
    AddExState(&chip->slot[10], sizeof(chip->slot[10]), 0, "SL10");
    AddExState(&chip->slot[11], sizeof(chip->slot[11]), 0, "SL11");

	AddExState(&chip->mask, sizeof(chip->mask), 0, "MASK");

	AddExState(chip->ch_out, sizeof(chip->ch_out), 0, "CHOU");
	AddExState(chip->mix_out, sizeof(chip->mix_out), 0, "MIXO");

	/* custom patches */
	AddExState(&chip->patch[0], sizeof(chip->patch[0]), 0, "PAT0");
	AddExState(&chip->patch[1], sizeof(chip->patch[1]), 0, "PAT1");
}
