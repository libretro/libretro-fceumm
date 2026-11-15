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

#ifdef HAVE_HDPACK
	/* HD packs: remember the fine-x scroll active for this emitted
	 * 8-pixel group so HDNes_RecordLine can map screen pixels back to
	 * the two overlapping tile fetches that produced them. */
	if (hdnes_active)
		HDNes_RecordBgGroup(X1 - 2, XOffset);
#endif

	pixdata = ppulut1[(pshift[0] >> (8 - XOffset)) & 0xFF] | ppulut2[(pshift[1] >> (8 - XOffset)) & 0xFF];

	pixdata |= ppulut3[XOffset | (atlatch << 3)];

	{
		/* All eight pens are fetched before any store: P may alias
		 * PALRAM (both uint8_t), so interleaving would serialise each
		 * load behind the previous store. */
		uint8_t p0 = PALRAM[ pixdata        & 0xF];
		uint8_t p1 = PALRAM[(pixdata >>  4) & 0xF];
		uint8_t p2 = PALRAM[(pixdata >>  8) & 0xF];
		uint8_t p3 = PALRAM[(pixdata >> 12) & 0xF];
		uint8_t p4 = PALRAM[(pixdata >> 16) & 0xF];
		uint8_t p5 = PALRAM[(pixdata >> 20) & 0xF];
		uint8_t p6 = PALRAM[(pixdata >> 24) & 0xF];
		uint8_t p7 = PALRAM[ pixdata >> 28       ];
		P[0] = p0;
		P[1] = p1;
		P[2] = p2;
		P[3] = p3;
		P[4] = p4;
		P[5] = p5;
		P[6] = p6;
		P[7] = p7;
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

#if defined(HAVE_HDPACK) && !defined(PPU_BGFETCH)
	/* HD packs: record this tile fetch (CHR tile base pointer,
	 * attribute palette, fine-y).  The low 3 bits of vadr are the fine
	 * scanline within the tile in every fetch variant, so C - (vadr&7)
	 * is the 16-byte tile base.  The PEC586 interleaved layout
	 * (PPU_BGFETCH) is not supported. */
	if (hdnes_active)
		HDNes_RecordBgFetch(X1, C - (vadr & 7), cc, (uint8_t)(vadr & 7));
#endif

if ((RefreshAddr & 0x1f) == 0x1f)
	RefreshAddr ^= 0x41F;
else
	RefreshAddr++;

#ifdef PPUT_HOOK
	PPU_hook(0x2000 | (RefreshAddr & 0xfff));
#endif
