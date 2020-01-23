/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2005 CaH4e3
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
#include "mmc3.h"

static uint8 *CHRRAM = NULL;
static int masko8[8] = { 63, 31, 15, 1, 3, 0, 0, 0 };

static uint8 oldversion = 0;
// this mapper is 176 rip mapper.

static void Super24PW(uint32 A, uint8 V) {
	if (oldversion == 1)
	{
		uint32 NV = V & masko8[EXPREGS[0] & 7];
		NV |= (EXPREGS[1] << 1);
		setprg8r((NV >> 6) & 0xF, A, NV);
	}
	else
	{
		setprg8(A, (EXPREGS[1] << 1) | (V & masko8[EXPREGS[0] & 0x7]));
	}

}

static void Super24CW(uint32 A, uint8 V) {
	if (oldversion == 1)
	{
		if (EXPREGS[0] & 0x20)
			setchr1r(0x10, A, V);
		else {
			uint32 NV = V | (EXPREGS[2] << 3);
			setchr1r((NV >> 9) & 0xF, A, NV);
		}
	}
	else
	{
		if (EXPREGS[0] & 0x20)
			setchr1r(0x10, A, (EXPREGS[2] << 3) | V);
		else
			setchr1r(0x00, A, (EXPREGS[2] << 3) | V);
	}
}

static DECLFW(Super24Write) {
	if (oldversion == 1)
	{
		switch (A) {
		case 0x5FF0:
			EXPREGS[0] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
			break;
		case 0x5FF1:
			EXPREGS[1] = V;
			FixMMC3PRG(MMC3_cmd);
			break;
		case 0x5FF2:
			EXPREGS[2] = V;
			FixMMC3CHR(MMC3_cmd);
			break;
		}
	}
	else
	{
		switch (A & 0xF003) {
		case 0x5000:
			EXPREGS[0] = V;
			FixMMC3PRG(MMC3_cmd);
			FixMMC3CHR(MMC3_cmd);
			break;
		case 0x5001:
			EXPREGS[1] = V;
			FixMMC3PRG(MMC3_cmd);
			break;
		case 0x5002:
			EXPREGS[2] = V;
			FixMMC3CHR(MMC3_cmd);
			break;
		}
	}
}

static void Super24Power(void) {
	EXPREGS[0] = 0x24;
	EXPREGS[1] = 159;
	EXPREGS[2] = 0;
	GenMMC3Power();
	SetWriteHandler(0x5000, 0x7FFF, Super24Write);
	SetReadHandler(0x8000, 0xFFFF, CartBR);
}

static void Super24Reset(void) {
	EXPREGS[0] = 0x24;
	EXPREGS[1] = 159;
	EXPREGS[2] = 0;
	MMC3RegReset();
}

static void Super24Close(void) {
	if (CHRRAM)
		FCEU_gfree(CHRRAM);
	CHRRAM = NULL;
}

void Super24_Init(CartInfo *info) {

	if (oldversion == 0)
	{
      uint8* _CHRptr;
      uint8* _PRGptr;
      uint32 _CHROffset = 0;
      uint32 _PRGOffset = 0;
		uint32 _PRGsize = 0;
		uint32 _CHRsize = 0;
		int i = 0;

		for (i = 0;i < 4;i++)
		{
			_PRGsize += PRGsize[i];
			_CHRsize += CHRsize[i];
		}

		_CHRptr = (uint8*)FCEU_gmalloc(_CHRsize);
		for (i = 0;i < 4;i++)
		{
			memcpy(&_CHRptr[_CHROffset], CHRptr[i], CHRsize[i]);
			_CHROffset += CHRsize[i];
		}

		_PRGptr = (uint8*)FCEU_gmalloc(_PRGsize);
		for (i = 0;i < 4;i++)
		{
			memcpy(&_PRGptr[_PRGOffset], PRGptr[i], PRGsize[i]);
			_PRGOffset += PRGsize[i];
		}
		SetupCartCHRMapping(0, _CHRptr, _CHRsize, 0);
		SetupCartPRGMapping(0, _PRGptr, _PRGsize, 0);
	}

	GenMMC3_Init(info, 512, 512, 0, 0);
	info->Power = Super24Power;
	info->Reset = Super24Reset;
	info->Close = Super24Close;
	cwrap = Super24CW;
	pwrap = Super24PW;
	CHRRAM = (uint8*)FCEU_gmalloc(8192);
	SetupCartCHRMapping(0x10, CHRRAM, 8192, 1);
	AddExState(CHRRAM, 8192, 0, "CHRR");
	AddExState(EXPREGS, 3, 0, "BIG2");
}
