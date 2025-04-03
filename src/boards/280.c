/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 * Copyright (C) 2025 NewRisingSun
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "mapinc.h"

static uint16 latchAddr;
static uint8  latchData;
static uint8  mode;

static SFORMAT StateRegs[] = {
   { &latchAddr, 2 | FCEUSTATE_RLSB, "LATC" },
   { &latchData, 1, "DATA" },
   { &mode, 1, "MODE" },
   { 0 }
};

static void Sync(void) {
   if (mode &1) {
      setprg16(0x8000, 0x20 | latchData &0x07);
      setprg16(0xC000, 0x27);
   } else {
      if (latchAddr &0x01)
	      setprg32(0x8000, latchAddr >>3 &0x0F);
      else {
	      setprg16(0x8000, latchAddr >>2 &0x1F);
	      setprg16(0xC000, latchAddr >>2 &0x1F);
      }
      if (~latchAddr &0x80) setprg16(0xC000, 0);
   }
   SetupCartCHRMapping(0, CHRptr[0], 0x2000, ~mode &0x01 && latchAddr &0x80? 0: 1);
   setchr8(0);
   setmirror(latchAddr &0x02? MI_H: MI_V);
}

static DECLFW(M280Write) {
   latchAddr =A;
   latchData =V;
   Sync();
}

static void M280Reset(void) {
   mode ^=1;
   latchAddr = 0;
   latchData = 0;
   Sync();
}

static void M280Power(void) {
   mode =0;
   latchAddr = 0;
   latchData = 0;
   Sync();
   SetReadHandler(0x8000, 0xFFFF, CartBR);
   SetWriteHandler(0x8000, 0xFFFF, M280Write);
}

static void StateRestore(int version) {
   Sync();
}

void Mapper280_Init(CartInfo *info) {
   info->Power = M280Power;
   info->Reset = M280Reset;
   GameStateRestore = StateRestore;
   AddExState(&StateRegs, ~0, 0, 0);
}
