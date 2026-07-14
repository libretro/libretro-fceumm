uint8_t *C;
uint8_t cc;
uint32_t vadr;

#ifndef PPUT_MMC5SP
	uint8_t zz;
#else
	uint8_t xs, ys;
	xs = X1;
	ys = ((scanline >> 3) + MMC5HackSPScroll) & 0x1F;
	if (ys >= 0x1E) ys -= 0x1E;
#endif

if (X1 >= 2) {
	uint32_t pixdata;

	pixdata = ppulut1[(pshift[0] >> (8 - XOffset)) & 0xFF] | ppulut2[(pshift[1] >> (8 - XOffset)) & 0xFF];

	pixdata |= ppulut3[XOffset | (atlatch << 3)];

	/* Per-tile palette gather: 8 nibbles of pixdata index PALRAM[0..15].
	 * Equivalent scalar form is
	 *   for (k = 0; k < 8; k++) { P[k] = PALRAM[pixdata & 0xF]; pixdata >>= 4; }
	 * which gcc unrolls into 8 byte loads + a shift-OR-accumulate chain
	 * + one 64-bit store.  The pair-LUT replacement at ppu.c file scope
	 * (fceu_bg_pair_lut[256], rebuilt at the start of RefreshLine)
	 * halves the load count: each 8-bit slice of pixdata fetches a
	 * pre-packed 2-pen pair, so we read four 16-bit values, OR-merge
	 * into a 64-bit tile word, and emit a single store.  may_alias
	 * + aligned(1) lets gcc emit a direct movq without going through
	 * a stack scratch (which the obvious __builtin_memcpy / packed-
	 * struct form would force, costing the optimisation entirely). */
	{
#ifdef MSB_FIRST
		/* Big-endian: bytes are stored most-significant first, so the
		 * first pixel pair (pens 0-1) must occupy the TOP 16 bits of
		 * the tile word to land at P[0]/P[1].  The LUT entries are
		 * built with the even pen in the high byte (see
		 * FCEU_BuildBgPairLUT), completing the mirror of the
		 * little-endian layout below. */
		uint64_t packed =
			((uint64_t)fceu_bg_pair_lut[ pixdata        & 0xFF] << 48) |
			((uint64_t)fceu_bg_pair_lut[(pixdata >>  8) & 0xFF] << 32) |
			((uint64_t)fceu_bg_pair_lut[(pixdata >> 16) & 0xFF] << 16) |
			 (uint64_t)fceu_bg_pair_lut[(pixdata >> 24) & 0xFF];
#else
		uint64_t packed =
			(uint64_t)fceu_bg_pair_lut[ pixdata        & 0xFF]        |
			((uint64_t)fceu_bg_pair_lut[(pixdata >>  8) & 0xFF] << 16) |
			((uint64_t)fceu_bg_pair_lut[(pixdata >> 16) & 0xFF] << 32) |
			((uint64_t)fceu_bg_pair_lut[(pixdata >> 24) & 0xFF] << 48);
#endif
#if defined(__GNUC__) || defined(__clang__)
		/* may_alias + aligned(1) lets gcc emit a direct movq without
		 * routing through a stack scratch. */
		typedef uint64_t fceu_u64_unaligned __attribute__((may_alias, aligned(1)));
		*(fceu_u64_unaligned *)P = packed;
#else
		/* MSVC has no may_alias; memcpy is strict-aliasing-safe and is
		 * lowered to the same unaligned 64-bit store. */
		memcpy(P, &packed, sizeof(packed));
#endif
	}
	P += 8;
}

#ifdef PPUT_MMC5SP
	vadr = (MMC5HackExNTARAMPtr[xs | (ys << 5)] << 4) + (vofs & 7);
#else
	zz = RefreshAddr & 0x1F;
	C = vnapage[(RefreshAddr >> 10) & 3];
	vadr = (C[RefreshAddr & 0x3ff] << 4) + vofs;	/* Fetch name table byte. */
#endif

#ifdef PPUT_HOOK
	PPU_hook(0x2000 | (RefreshAddr & 0xfff));
#endif

#ifdef PPUT_MMC5SP
	cc = MMC5HackExNTARAMPtr[0x3c0 + (xs >> 2) + ((ys & 0x1C) << 1)];
	cc = ((cc >> ((xs & 2) + ((ys & 0x2) << 1))) & 3);
#else
	#ifdef PPUT_MMC5CHR1
		cc = (MMC5HackExNTARAMPtr[RefreshAddr & 0x3ff] & 0xC0) >> 6;
	#else
		cc = C[0x3c0 + (zz >> 2) + ((RefreshAddr & 0x380) >> 4)];	/* Fetch attribute table byte. */
		cc = ((cc >> ((zz & 2) + ((RefreshAddr & 0x40) >> 4))) & 3);
	#endif
#endif

atlatch >>= 2;
atlatch |= cc << 2;

pshift[0] <<= 8;
pshift[1] <<= 8;

#ifdef PPUT_MMC5SP
	C = MMC5HackVROMPTR + vadr;
	C += ((MMC5HackSPPage & 0x3f & MMC5HackVROMMask) << 12);
#else
	#ifdef PPUT_MMC5CHR1
		C = MMC5HackVROMPTR;
		C += (((MMC5HackExNTARAMPtr[RefreshAddr & 0x3ff]) & 0x3f & MMC5HackVROMMask) << 12) + (vadr & 0xfff);
		C += (MMC50x5130 & 0x3) << 18;
	#elif defined(PPUT_MMC5)
		C = MMC5BGVRAMADR(vadr);
	#else
		C = VRAMADR(vadr);
	#endif
#endif

#ifdef PPUT_HOOK
	PPU_hook(vadr);
#endif

#ifdef PPU_BGFETCH
	if (RefreshAddr & 1) {
		pshift[0] |= C[8];
		pshift[1] |= C[8];
	} else {
		pshift[0] |= C[0];
		pshift[1] |= C[0];
	}
#else
	pshift[0] |= C[0];
	pshift[1] |= C[8];
#endif

if ((RefreshAddr & 0x1f) == 0x1f)
	RefreshAddr ^= 0x41F;
else
	RefreshAddr++;

#ifdef PPUT_HOOK
	PPU_hook(0x2000 | (RefreshAddr & 0xfff));
#endif
