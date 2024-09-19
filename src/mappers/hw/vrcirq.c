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
#include "vrcirq.h"

static int16 IRQPrescaler;
static uint8 IRQCount, IRQLatch, IRQd, IRQa, IRQm, IRQr;

static uint8 IRQScanPos;
static uint8 IRQScanLut[3] = { 113, 113, 112 };

SFORMAT VRCIRQ_StateRegs[] = {
	{ &IRQPrescaler, 2, "PREC" },
	{ &IRQCount, 1, "IRQC" },
	{ &IRQLatch, 1, "IRQL" },
	{ &IRQd, 1, "IRQD" },
	{ &IRQa, 1, "IRQA" },
	{ &IRQm, 1, "IRQM" },
	{ &IRQr, 1, "IRQR" },
	{ &IRQScanPos, 1, "IPOS" },
	{ 0 }
};

static void VRCIRQ_Clock(void) {
	if (IRQCount == 0xFF) {
		X6502_IRQBegin(FCEU_IQEXT);
		IRQCount = IRQLatch;
	} else {
		IRQCount++;
	}
}

void VRCIRQ_CPUHook(int a) {
	if (!IRQa) {
		return;
	}

	while (a--) {
		if (IRQm) { /* cycle mode */
			VRCIRQ_Clock();
		} else {
			if (IRQPrescaler >= IRQScanLut[IRQScanPos]) {
				VRCIRQ_Clock();
				IRQScanPos++;
				if (IRQScanPos >= 3) IRQScanPos = 0;
				IRQPrescaler = 0;
			} else {
				IRQPrescaler++;
			}
		}
	}
}

void VRCIRQ_Latch(uint8 V) {
	IRQLatch = V;
}

void VRCIRQ_LatchNibble(uint8 V, uint8 highBit) {
	if (highBit) {
		IRQLatch &= 0x0F;
		IRQLatch |= V << 4;
	} else {
		IRQLatch &= 0xF0;
		IRQLatch |= V & 0xF;
	}
}

void VRCIRQ_Control(uint8 V) {
	IRQd = (V & 0x01) == 0x01;
	IRQa = (V & 0x02) == 0x02;
	IRQm = (V & 0x04) == 0x04;

	if (IRQa) {
		IRQCount = IRQLatch;
		IRQPrescaler = 0;
		IRQScanPos = 0;
	}

	X6502_IRQEnd(FCEU_IQEXT);
}

void VRCIRQ_Acknowledge(void) {
	if (IRQr == TRUE) {
		IRQa = IRQd;
	}

	X6502_IRQEnd(FCEU_IQEXT);
}

void VRCIRQ_Init(int irqRepeated) {
	IRQPrescaler = 0;
	IRQCount = 0;
	IRQLatch = 0;
	IRQd = 0;
	IRQa = 0;
	IRQm = 0;
	IRQr = irqRepeated ? TRUE : FALSE;
	IRQScanPos = 0;
}
