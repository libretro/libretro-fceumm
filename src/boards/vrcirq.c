#include "mapinc.h"
#include "vrcirq.h"

static int16 IRQPrescaler;
static uint8 IRQCount;
static uint8 IRQLatch;
static uint8 IRQd;
static uint8 IRQa;
static uint8 IRQm;

static SFORMAT StateRegs[] =
{
	{ &IRQPrescaler,  2, "PREC" },
	{ &IRQCount,      1, "IRQC" },
	{ &IRQLatch,      1, "IRQL" },
	{ &IRQd,          1, "IRQD" },
	{ &IRQa,          1, "IRQA" },
	{ &IRQm,          1, "IRQM" },

	{ 0 }
};

static void FP_FASTAPASS(1) VRCIRQ_IrqHook(int a)
{
	int count = a;

	if (!IRQa) {
		return;
	}

	while (count) {
		count--;
		IRQPrescaler -= 3;
		if (IRQm || (IRQPrescaler < 0 && !IRQm)) {
			IRQPrescaler += 341;
			if (IRQCount == 0xFF) {
				X6502_IRQBegin(FCEU_IQEXT);
				IRQCount = IRQLatch;
			} else {
				IRQCount++;
			}
		}
	}
}

void VRCIRQ_Init(void)
{
	IRQPrescaler  = 0;
	IRQCount      = 0;
	IRQLatch      = 0;
	IRQd          = 0;
	IRQa          = 0;
	IRQm          = 0;
	MapIRQHook    = VRCIRQ_IrqHook;

	AddExState(&StateRegs, ~0, 0, 0);
}

void VRCIRQ_Latch(uint8 V)
{
	IRQLatch = V;
}

void VRCIRQ_LatchNibble(uint8 V, uint8 highBit)
{
	if (highBit) {
		IRQLatch &= 0x0F;
		IRQLatch |= V << 4;
	} else {
		IRQLatch &= 0xF0;
		IRQLatch |= V & 0xF;
	}
}

void VRCIRQ_Control(uint8 V)
{
	IRQd = (V & 0x01) == 0x01;
	IRQa = (V & 0x02) == 0x02;
	IRQm = (V & 0x04) == 0x04;
	IRQPrescaler = 341;

	if (IRQa) {
		IRQCount = IRQLatch;
	}

	X6502_IRQEnd(FCEU_IQEXT);
}

void VRCIRQ_Acknowledge(void)
{
	IRQa = IRQd;
	X6502_IRQEnd(FCEU_IQEXT);
}
