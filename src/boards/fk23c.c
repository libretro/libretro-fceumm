/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2006 CaH4e3
 *  Copyright (C) 2020 negativeExponent
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

/* BMC-FK23C (no WRAM, no DIP switch)
 * BMC-FK23CA (no WRAM, with DIP switch)
 * BMC-Super24in1SC03 (functional duplicate of BMC-FK23C)
 * WAIXING-FS005 (alternative name: Bensheng BS-001) (32 KiB battery-backed WRAM, 8 KiB of CHR-RAM, no DIP switch)
 * WAIXING-FS006 (optional 8 KiB battery-backed WRAM, optional 8 KiB of CHR-RAM, no DIP switch)

 * Three incompatible subtypes exist that do not correspond to these UNIF board names. No submappers have been proposed, as the subtypes can be easily discerned heuristically by looking at ROM sizes:

 * Subtype 0, ROM size other than specified below: boot with Extended MMC3 mode disabled (boots in first 512 KiB of PRG-ROM regardless of ROM size)
 * Subtype 1, 1024 KiB PRG-ROM, 1024 KiB CHR-ROM: boot with Extended MMC3 mode enabled (boots in last 512 KiB of the first 2 MiB of PRG-ROM)
 * Subtype 2, 8192 or more KiB PRG-ROM, no CHR-ROM: Like Subtype 0, but MMC3 registers $46 and $47 swapped.
 */

/* 2020-3-14 - Refactoring based on latest sources */
/* TODO: Add database for ines 1.0 headers */

#include "mapinc.h"
#include "../ines.h"

static uint8 *WRAM            = NULL;
static uint8 *CHRRAM          = NULL;
static uint32 WRAMSIZE        = 0;
static uint32 CHRRAMSIZE      = 0;

static uint8 fk23_regs[4]     = { 0 };
static uint8 mmc3_regs[12]    = { 0 };
static uint8 mmc3_ctrl        = 0;
static uint8 mmc3_mirr        = 0;
static uint8 mmc3_wram        = 0;
static uint8 irq_count        = 0;
static uint8 irq_latch        = 0;
static uint8 irq_enabled      = 0;
static uint8 irq_reload       = 0;
static uint8 cnrom_chr        = 0;
static uint8 dipswitch        = 0;
static uint8 subType          = 0;
static uint8 is_bmcfk23ca     = 0;

static SFORMAT StateRegs[] = {
   { fk23_regs,               4, "EXPR" },
   { mmc3_regs,              12, "M3RG" },
   { &cnrom_chr,              1, "CCHR" },
   { &dipswitch,              1, "DPSW" },
   { &mmc3_ctrl,              1, "M3CT" },
   { &mmc3_mirr,              1, "M3MR" },
   { &mmc3_wram,              1, "M3WR" },
   { &irq_reload,             1, "IRQR" },
   { &irq_count,              1, "IRQC" },
   { &irq_latch,              1, "IRQL" },
   { &irq_enabled,            1, "IRQA" },
   { &subType,                1, "SUBT" },
   { 0 }
};

#define INVERT_PRG            (mmc3_ctrl & 0x40)
#define INVERT_CHR            (mmc3_ctrl & 0x80)
#define WRAM_ENABLED          (mmc3_wram & 0x80)
#define WRAM_EXTENDED         (mmc3_wram & 0x20)
#define FK23_ENABLED          (mmc3_wram & 0x40)
#define MMC3_EXTENDED         (fk23_regs[3] & 0x02)
#define CHR_CNROM_MODE        (fk23_regs[0] & 0x40)
#define CHR_OUTER_BANK_SIZE   (fk23_regs[0] & 0x10)

static void cwrap(uint16 A, uint16 V)
{
   int bank = 0;

   /* some workaround for chr rom / ram access */
   if (!VROM_size)
      bank = 0;
   else if (CHRRAMSIZE && fk23_regs[0] & 0x20)
      bank = 0x10;

   if (WRAM_EXTENDED) {
      if ((mmc3_wram & 0x04) && V < 8) bank = 0x10; /* first 8K of chr bank is ram */
      else bank = 0;
   }

   setchr1r(bank, A, V);
}

static void SyncCHR(void)
{
   if (CHR_CNROM_MODE)
   {
      uint8 mask              = (fk23_regs[3] & 0x46) ? (CHR_OUTER_BANK_SIZE ? 0x01 : 0x03) : 0;
      uint16 bank             = (fk23_regs[2] | (cnrom_chr & mask)) << 3;

      cwrap(0x0000, bank);
      cwrap(0x0400, bank + 1);
      cwrap(0x0800, bank + 2);
      cwrap(0x0C00, bank + 3);

      cwrap(0x1000, bank + 4);
      cwrap(0x1400, bank + 5);
      cwrap(0x1800, bank + 6);
      cwrap(0x1C00, bank + 7);
   }
   else
   {
      if (MMC3_EXTENDED)
      {
         uint16 cbase         = INVERT_CHR ? 0x1000 : 0;
         uint16 outer         = fk23_regs[2] << 3;

         cwrap(cbase ^ 0x0000, mmc3_regs[0]  | outer);
         cwrap(cbase ^ 0x0400, mmc3_regs[10] | outer);
         cwrap(cbase ^ 0x0800, mmc3_regs[1]  | outer);
         cwrap(cbase ^ 0x0c00, mmc3_regs[11] | outer);

         cwrap(cbase ^ 0x1000, mmc3_regs[2] | outer);
         cwrap(cbase ^ 0x1400, mmc3_regs[3] | outer);
         cwrap(cbase ^ 0x1800, mmc3_regs[4] | outer);
         cwrap(cbase ^ 0x1c00, mmc3_regs[5] | outer);
      }
      else
      {
         uint16 cbase         = INVERT_CHR ? 0x1000 : 0;
         uint8 mask           = CHR_OUTER_BANK_SIZE ? 0x7F : 0xFF;
         uint16 outer         = (fk23_regs[2] << 3) & ~mask;

         cwrap(cbase ^ 0x0000, ((mmc3_regs[0] & 0xFE) & mask) | outer);
         cwrap(cbase ^ 0x0400, ((mmc3_regs[0] | 0x01) & mask) | outer);
         cwrap(cbase ^ 0x0800, ((mmc3_regs[1] & 0xFE) & mask) | outer);
         cwrap(cbase ^ 0x0C00, ((mmc3_regs[1] | 0x01) & mask) | outer);

         cwrap(cbase ^ 0x1000, (mmc3_regs[2] & mask) | outer);
         cwrap(cbase ^ 0x1400, (mmc3_regs[3] & mask) | outer);
         cwrap(cbase ^ 0x1800, (mmc3_regs[4] & mask) | outer);
         cwrap(cbase ^ 0x1c00, (mmc3_regs[5] & mask) | outer);
      }
   }
}

static void SyncPRG(void)
{
   uint8 prg_mode             = fk23_regs[0] & 7;
   uint16 prg_base            = (fk23_regs[1] & 0x07F) | ((fk23_regs[0] << 4) & 0x080) |
         ((fk23_regs[0] << 1) & 0x100) | ((fk23_regs[2] << 3) & 0x600) |
         ((fk23_regs[2] << 6) & 0x800);

   switch (prg_mode)
   {
   case 4:
      setprg32(0x8000, prg_base >> 1);
      break;
   case 3:
      setprg16(0x8000, prg_base);
      setprg16(0xC000, prg_base);
      break;
   case 0:
   case 1:
   case 2:
      if (MMC3_EXTENDED)
      {
         uint16 cbase         = INVERT_PRG ? 0x4000 : 0;
         uint16 outer         = prg_base << 1;

         setprg8(0x8000 ^ cbase, mmc3_regs[6] | outer);
         setprg8(0xA000,         mmc3_regs[7] | outer);
         setprg8(0xC000 ^ cbase, mmc3_regs[8] | outer);
         setprg8(0xE000,         mmc3_regs[9] | outer);
      }
      else
      {
         uint16 cbase         = INVERT_PRG ? 0x4000 : 0;
         uint8 mask           = 0x3F >> prg_mode;
         uint16 outer         = (prg_base << 1) & ~mask;

         setprg8(0x8000 ^ cbase, (mmc3_regs[6] & mask) | outer);
         setprg8(0xA000,         (mmc3_regs[7] & mask) | outer);
         setprg8(0xC000 ^ cbase, (0xFE & mask) | outer);
         setprg8(0xE000,         (0xFF & mask) | outer);
      }
      break;
   }
}

static void SyncWRAM(void)
{
   /* TODO: WRAM Protected  mode when not in extended mode */
   if (WRAM_ENABLED || WRAM_EXTENDED)
   {
      if (WRAM_EXTENDED)
      {
         /* FIXME:this does not look normal, but it works, $5000-$5fff */
         setprg8r(0x10, 0x4000, (mmc3_wram & 0x03) + 1);
         setprg8r(0x10, 0x6000, mmc3_wram & 0x03);
      }
      else
         setprg8r(0x10, 0x6000, 0);
   }
}

static void SyncMIR(void)
{
   switch (mmc3_mirr & (WRAM_EXTENDED ? 0x03 : 0x01))
   {
   case 0: setmirror(MI_V); break;
   case 1: setmirror(MI_H); break;
   case 2: setmirror(MI_0); break;
   case 3: setmirror(MI_1); break;
   }
}

static void Sync(void)
{
   SyncPRG();
   SyncCHR();
   SyncWRAM();
   SyncMIR();
}

static DECLFW(WriteLo)
{
   if (((WRAM_EXTENDED == 0) || FK23_ENABLED) && (A & (0x10 << dipswitch)))
   {
      switch (A & 3)
      {
      case 0:
         if (fk23_regs[0] != V)
         {
            fk23_regs[0] = V;
            SyncPRG();
            SyncCHR();
         }
         break;
      case 1:
         if (fk23_regs[1] != V)
         {
            fk23_regs[1] = V;
            SyncPRG();
         }
         break;
      case 2:
         cnrom_chr = 0;
         if (fk23_regs[2] != V)
         {
            fk23_regs[2] = V;
            SyncPRG();
            SyncCHR();
         }
         break;
      case 3:
         if (fk23_regs[3] != V)
         {
            fk23_regs[3] = V;
            SyncPRG();
            SyncCHR();
         }
         break;
      }
   }
   else
      /* FK23C Registers disabled, $5000-$5FFF maps to the second 4 KiB of the 8 KiB WRAM bank 2 */
      CartBW(A, V);
}

static DECLFW(WriteHi)
{
   switch (A & 0xE000)
   {
   case 0x8000:
   case 0x9000:
   case 0xC000:
   case 0xD000:
   case 0xE000:
   case 0xF000:
      if (CHR_CNROM_MODE)
      {
         cnrom_chr = V & 0x03;

         if ((fk23_regs[0] & 0x07) == 0x03)
            cnrom_chr = 0;

         SyncCHR();
      }
      break;
   }

   switch (A & 0xE001)
   {
   case 0x8000:
   {
      uint8 old_ctrl = mmc3_ctrl;

      /* Subtype 2, 8192 or more KiB PRG-ROM, no CHR-ROM: Like Subtype 0,
       * but MMC3 registers $46 and $47 swapped. */
      if (subType == 2)
      {
         if (V == 0x46)
            V = 0x47;
         else if (V == 0x47)
            V = 0x46;
      }

      mmc3_ctrl = V;

      if (INVERT_PRG != (old_ctrl & 0x40))
         SyncPRG();

      if (INVERT_CHR != (old_ctrl & 0x80))
         SyncCHR();

      break;
   }
   case 0x8001:
   {
      uint8 ctrl_mask = MMC3_EXTENDED ? 0x0F : 0x07;

      if ((mmc3_ctrl & ctrl_mask) < 12)
      {
         mmc3_regs[mmc3_ctrl & ctrl_mask] = V;

         if (((mmc3_ctrl & ctrl_mask) < 6) || ((mmc3_ctrl & ctrl_mask) >= 10))
            SyncCHR();
         else
            SyncPRG();
      }
      break;
   }
   case 0xA000:
      mmc3_mirr = V;
      SyncMIR();
      break;
   case 0xA001:
      /* ignore bits when ram config register is disabled */
      if ((V & 0x20) == 0)
         V &= 0xC0;
      mmc3_wram = V;
      Sync();
      break;
   case 0xC000:
      irq_latch = V;
      break;
   case 0xC001:
      irq_reload = 1;
      break;
   case 0xE000:
      X6502_IRQEnd(FCEU_IQEXT);
      irq_enabled = 0;
      break;
   case 0xE001:
      irq_enabled = 1;
      break;
   }
}

static void IRQHook(void)
{
   if (!irq_count || irq_reload)
      irq_count = irq_latch;
   else
      irq_count--;

   if (!irq_count && irq_enabled)
      X6502_IRQBegin(FCEU_IQEXT);

   irq_reload = 0;
}

static void Reset(void)
{
   /* this little hack makes sure that we try all the dip switch settings eventually, if we reset enough */
   /*if (is_bmcfk23ca) {
      dipswitch = (dipswitch + 1) & 7;
      printf("BMCFK23C dipswitch set to $%04x\n",0x5000|0x10 << dipswitch);
   }*/

   fk23_regs[0]   = fk23_regs[1] = fk23_regs[2] = fk23_regs[3] = 0;
   mmc3_regs[0]   = 0;
   mmc3_regs[1]   = 2;
   mmc3_regs[2]   = 4;
   mmc3_regs[3]   = 5;
   mmc3_regs[4]   = 6;
   mmc3_regs[5]   = 7;
   mmc3_regs[6]   = 0;
   mmc3_regs[7]   = 1;
   mmc3_regs[8]   = ~1;
   mmc3_regs[9]   = ~0;
   mmc3_regs[10]  = ~0;
   mmc3_regs[11]  = ~0;
   mmc3_wram      = 0x80;
   mmc3_ctrl      = mmc3_mirr = irq_count = irq_latch = irq_enabled = 0;

   if (subType == 1)
      fk23_regs[1] = 0x20;

   Sync();
}

static void Power(void)
{
   fk23_regs[0]   = fk23_regs[1] = fk23_regs[2] = fk23_regs[3] = 0;
   mmc3_regs[0]   = 0;
   mmc3_regs[1]   = 2;
   mmc3_regs[2]   = 4;
   mmc3_regs[3]   = 5;
   mmc3_regs[4]   = 6;
   mmc3_regs[5]   = 7;
   mmc3_regs[6]   = 0;
   mmc3_regs[7]   = 1;
   mmc3_regs[8]   = ~1;
   mmc3_regs[9]   = ~0;
   mmc3_regs[10]  = ~0;
   mmc3_regs[11]  = ~0;
   mmc3_wram      = 0x80;
   mmc3_ctrl      = mmc3_mirr = irq_count = irq_latch = irq_enabled = 0;

   if (subType == 1)
      fk23_regs[1] = 0x20;

   Sync();

   SetReadHandler(0x8000, 0xFFFF, CartBR);
   SetWriteHandler(0x5000, 0x5fff, WriteLo);
   SetWriteHandler(0x8000, 0xFFFF, WriteHi);

   if (WRAMSIZE)
   {
      SetReadHandler(0x6000, 0x7FFF, CartBR);
      SetWriteHandler(0x6000, 0x7FFF, CartBW);
      FCEU_CheatAddRAM(WRAMSIZE >> 10, 0x6000, WRAM);
   }
}

static void Close(void)
{
   if (WRAM)
      FCEU_gfree(WRAM);
   WRAM = NULL;

   if (CHRRAM)
      FCEU_gfree(CHRRAM);
   CHRRAM = NULL;
}

static void StateRestore(int version)
{
   Sync();
}

void GenBMCFK23C_Init(CartInfo *info)
{
   is_bmcfk23ca = 0;

   info->Power       = Power;
   info->Reset       = Reset;
   info->Close       = Close;
   GameHBIRQHook     = IRQHook;
   GameStateRestore  = StateRestore;
   AddExState(StateRegs, ~0, 0, 0);

   if (CHRRAMSIZE)
   {
      CHRRAM = (uint8 *)FCEU_gmalloc(CHRRAMSIZE);
      SetupCartCHRMapping(0x10, CHRRAM, CHRRAMSIZE, 1);
      AddExState(CHRRAM, CHRRAMSIZE, 0, "CRAM");
   }

   if (WRAMSIZE)
   {
      WRAM = (uint8 *)FCEU_gmalloc(WRAMSIZE);
      SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
      AddExState(WRAM, WRAMSIZE, 0, "WRAM");

      if (info->battery)
      {
         info->SaveGame[0] = WRAM;
         info->SaveGameLen[0] = info->PRGRamSaveSize ? info->PRGRamSaveSize : WRAMSIZE;
      }
   }

   subType = 0;
   if (((ROM_size * 16) == 1024) && ((VROM_size * 8) == 1024))
      subType = 1;
   else if (UNIFchrrama && ((ROM_size << 4) >= 8192))
      subType = 2;
}

/* generic entry point for mapper 176 / bmcfk23c carts */
void BMCFK23C_Init(CartInfo *info) {
   /* prepare ROM params before loading... */
   if (info->iNES2)
   {
      if (!UNIFchrrama)
         CHRRAMSIZE = info->CHRRamSize + info->CHRRamSaveSize;
      WRAMSIZE = info->PRGRamSize + info->PRGRamSaveSize;
   }
   else
   {
      if (!UNIFchrrama)
      {
         /*  Rockman I - VI uses mixed chr rom/ram */
         if ((ROM_size * 16) == 2048 && (VROM_size * 8) == 512)
            CHRRAMSIZE = 8 * 1024;
      }

      /* Waixing boards has 32K battery backed wram */
      if (info->battery)
         WRAMSIZE = 32 * 1024;
      else
         /* Always enable WRAM for ines-headered roms, lets see who complains */
         WRAMSIZE = 8 * 1024;
   }

   GenBMCFK23C_Init(info);
}

/* UNIF Boards, declares so we can for chr mixed mode size and wram if any */

void BMCFK23CA_Init(CartInfo *info)
{
   /* can use mixed chr rom/ram */
   if (!UNIFchrrama)
      CHRRAMSIZE = 8 * 1024;

   WRAMSIZE = 8 * 1024;

   GenBMCFK23C_Init(info);
   is_bmcfk23ca = 1;
}

/* BMC-Super24in1SC03 */
void Super24_Init(CartInfo *info) {
   /* can use mixed chr rom/ram */
   if (!UNIFchrrama)
      CHRRAMSIZE = 8 * 1024;

   GenBMCFK23C_Init(info);
}

void WAIXINGFS005_Init(CartInfo *info)
{
   /* can have 8 or 32 KB battery-backed prg ram
    * plus 8 KB chr for these boards */
   if (!UNIFchrrama)
      CHRRAMSIZE = 8 * 1024;

   WRAMSIZE = 32 * 1024;

   GenBMCFK23C_Init(info);
}
