#define MMC1_reg regByte
#define MMC1_shift regByte[4]
#define MMC1_count regByte[5]
#define MMC1_filter regByte[6]

static void MMC1_sync () {
	int AND =prgAND >>1;
	int OR  =prgOR  >>1 | (mapper &0x01? (MMC1_reg[1] &0x10): (mapperFlags &0x06));
	if (MMC1_reg[0] &0x8) { /* 16 KiB mode */
		if (MMC1_reg[0] &0x04) { /* OR logic */
			setprg16(0x8000, MMC1_reg[3] &AND | OR &~AND);
			setprg16(0xC000,        0xFF &AND | OR &~AND);
		} else {                 /* AND logic */
			setprg16(0x8000,        0x00 &AND | OR &~AND);
			setprg16(0xC000, MMC1_reg[3] &AND | OR &~AND);
		}
	} else
		setprg32(0x8000, (MMC1_reg[3] &AND | OR &~AND) >>1);
	
	AND =mapper &0x01? 0x0F: 0x1F;
	if (MMC1_reg[0] &0x10) { /* 4 KiB mode */
		setchr4(0x0000, MMC1_reg[1] &AND);
		setchr4(0x1000, MMC1_reg[2] &AND);
	} else                   /* 8 KiB mode */
		setchr8(MMC1_reg[1] >>1 &(AND >>1));

	switch(MMC1_reg[0] &3) {
		case 0: setmirror(MI_0); break;
		case 1: setmirror(MI_1); break;
		case 2: setmirror(MI_V); break;
		case 3: setmirror(MI_H); break;
	}
}

static DECLFW(MMC1_writeReg) {
	if (V &0x80) {
		MMC1_shift =MMC1_count =0;
		MMC1_reg[0] |=0x0C;
		sync();
	} else
	if (!MMC1_filter) {
		MMC1_shift |=(V &1) <<MMC1_count++;
		if (MMC1_count ==5) {
			MMC1_reg[A >>13 &3] =MMC1_shift;
			MMC1_count =0;
			MMC1_shift =0;
			sync();
		}
	}
	MMC1_filter =2;
}

static void FP_FASTAPASS(1) MMC1_cpuCycle(int a) {
	while (a--) if (MMC1_filter) MMC1_filter--;
}

void MMC1_reset(uint8 clearRegs) {
	sync =MMC1_sync;
	MapIRQHook =MMC1_cpuCycle;
	prgAND =mapperFlags &2? (mapperFlags &8? 0x07: 0x0F): 0x1F;
	SetWriteHandler(0x8000, 0xFFFF, MMC1_writeReg);
	if (clearRegs) MMC1_reg[0] =0x0C;
	sync();
}

#undef MMC1_reg
#undef MMC1_shift
#undef MMC1_count
#undef MMC1_filter

