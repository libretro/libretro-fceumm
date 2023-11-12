/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2002 Xodnizel
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fceu-types.h"
#include "x6502.h"
#include "cheat.h"
#include "fceu.h"
#include "general.h"
#include "cart.h"
#include "fceu-memory.h"

static uint8 *CheatRPtrs[64];
uint8 *MMapPtrs[64];

void FCEU_CheatResetRAM(void) {
	int x;

	for (x = 0; x < 64; x++)
		CheatRPtrs[x] = 0;
}

void FCEU_CheatAddRAM(int s, uint32 A, uint8 *p) {
	uint32 AB = A >> 10;
	int x;

	for (x = s - 1; x >= 0; x--) {
		CheatRPtrs[AB + x] = p - A;
		MMapPtrs[AB + x] = p + 1024 * x;
	}
}


struct CHEATF {
	struct CHEATF *next;
	char *name;
	uint16 addr;
	uint8 val;
	int compare;		/* -1 for no compare. */
	int type;			/* 0 for replace, 1 for substitute(GG). */
	int status;
};

typedef struct {
	uint16 addr;
	uint8 val;
	int compare;
	readfunc PrevRead;
} CHEATF_SUBFAST;


static CHEATF_SUBFAST SubCheats[256];
static int numsubcheats = 0;
struct CHEATF *cheats = 0, *cheatsl = 0;


#define CHEATC_NONE     0x8000
#define CHEATC_EXCLUDED 0x4000
#define CHEATC_NOSHOW   0xC000

static uint16 *CheatComp = 0;

static uint8 SubCheatsRead(uint32 A) {
	CHEATF_SUBFAST *s = SubCheats;
	int x = numsubcheats;

	do {
		if (s->addr == A) {
			if (s->compare >= 0) {
				uint8 pv = s->PrevRead(A);

				if (pv == s->compare)
					return(s->val);
				else
					return(pv);
			} else return(s->val);
		}
		s++;
	} while (--x);
	return(0);	/* We should never get here. */
}

static void RebuildSubCheats(void) {
	int x;
	struct CHEATF *c = cheats;

	for (x = 0; x < numsubcheats; x++)
		SetReadHandler(SubCheats[x].addr, SubCheats[x].addr, SubCheats[x].PrevRead);

	numsubcheats = 0;
	while (c) {
		if (c->type == 1 && c->status) {
			if (GetReadHandler(c->addr) == SubCheatsRead) {
				/* Prevent a catastrophe by this check. */
			} else {
				SubCheats[numsubcheats].PrevRead = GetReadHandler(c->addr);
				SubCheats[numsubcheats].addr = c->addr;
				SubCheats[numsubcheats].val = c->val;
				SubCheats[numsubcheats].compare = c->compare;
				SetReadHandler(c->addr, c->addr, SubCheatsRead);
				numsubcheats++;
			}
		}
		c = c->next;
	}
}

void FCEU_PowerCheats() {
	numsubcheats = 0;	/* Quick hack to prevent setting of ancient read addresses. */
	RebuildSubCheats();
}

/* This function doesn't allocate any memory for "name" */
static int AddCheatEntry(char *name, uint32 addr, uint8 val, int compare, int status, int type) {
	struct CHEATF *temp;
	if (!(temp = (struct CHEATF*)malloc(sizeof(struct CHEATF)))) {
		FCEUD_PrintError("Error allocating memory for cheat data.");
		return(0);
	}
	temp->name = name;
	temp->addr = addr;
	temp->val = val;
	temp->status = status;
	temp->compare = compare;
	temp->type = type;
	temp->next = 0;

	if (cheats) {
		cheatsl->next = temp;
		cheatsl = temp;
	} else
		cheats = cheatsl = temp;

	return(1);
}

void FCEU_LoadGameCheats(void)
{
   numsubcheats = 0;
   RebuildSubCheats();
}

void FCEU_FlushGameCheats(void) {
	if (CheatComp)
   {
		free(CheatComp);
		CheatComp = 0;
	}
	RebuildSubCheats();	/* Remove memory handlers. */
}

void FCEU_ResetCheats(void)
{
	if (CheatComp) {
		free(CheatComp);
		CheatComp = 0;
	}

   if (cheats)
   {
      struct CHEATF *next = cheats;
      for (;; ) {
         struct CHEATF *last = next;
         next = next->next;
         free(last->name);
         free(last);
         if (!next) break;
      }
      cheats = cheatsl = 0;
   }

	RebuildSubCheats();	/* Remove memory handlers. */
}


int FCEUI_AddCheat(const char *name, uint32 addr, uint8 val, int compare, int type) {
	char *t;
	if (!(t = (char*)malloc(strlen(name) + 1)))
	{
		FCEUD_PrintError("Error allocating memory for cheat data.");
		return(0);
	}
	strcpy(t, name);
	if (!AddCheatEntry(t, addr, val, compare, 1, type)) {
		free(t);
		return(0);
	}
	RebuildSubCheats();
	return(1);
}

void FCEU_ApplyPeriodicCheats(void) {
	struct CHEATF *cur = cheats;
	if (!cur) return;

	for (;; ) {
		if (cur->status && !(cur->type))
			if (CheatRPtrs[cur->addr >> 10])
				CheatRPtrs[cur->addr >> 10][cur->addr] = cur->val;
		if (cur->next)
			cur = cur->next;
		else
			break;
	}
}

static int GGtobin(char c) {
	static char lets[16] = { 'A', 'P', 'Z', 'L', 'G', 'I', 'T', 'Y', 'E', 'O', 'X', 'U', 'K', 'S', 'V', 'N' };
	int x;

	for (x = 0; x < 16; x++)
		if (lets[x] == toupper(c)) return(x);
	return(0);
}

/* Returns 1 on success, 0 on failure. Sets *a,*v,*c. */
int FCEUI_DecodeGG(const char *str, uint16 *a, uint8 *v, int *c) {
	uint8 t;
	uint16 A = 0x8000;
	uint8 V = 0;
	uint8 C = 0;
	int s = strlen(str);
	if (s != 6 && s != 8) return(0);

	t = GGtobin(*str++);
	V |= (t & 0x07);
	V |= (t & 0x08) << 4;

	t = GGtobin(*str++);
	V |= (t & 0x07) << 4;
	A |= (t & 0x08) << 4;

	t = GGtobin(*str++);
	A |= (t & 0x07) << 4;
	/* if(t&0x08) return(0); */  /* 8-character code?! */

	t = GGtobin(*str++);
	A |= (t & 0x07) << 12;
	A |= (t & 0x08);

	t = GGtobin(*str++);
	A |= (t & 0x07);
	A |= (t & 0x08) << 8;

	if (s == 6) {
		t = GGtobin(*str++);
		A |= (t & 0x07) << 8;
		V |= (t & 0x08);

		*a = A;
		*v = V;
		*c = -1;
		return(1);
	} else {
		t = GGtobin(*str++);
		A |= (t & 0x07) << 8;
		C |= (t & 0x08);

		t = GGtobin(*str++);
		C |= (t & 0x07);
		C |= (t & 0x08) << 4;

		t = GGtobin(*str++);
		C |= (t & 0x07) << 4;
		V |= (t & 0x08);
		*a = A;
		*v = V;
		*c = C;
		return(1);
	}
	return(0);
}

int FCEUI_DecodePAR(const char *str, uint16 *a, uint8 *v, int *c, int *type) {
	int boo[4];
	if (strlen(str) != 8) return(0);

	sscanf(str, "%02x%02x%02x%02x", boo, boo + 1, boo + 2, boo + 3);

	*c = -1;
	*v = boo[3];
	*a = boo[2] | (boo[1] << 8);

	/* Zero-page addressing modes don't go through the normal read/write handlers in FCEU, so
		we must do the old hacky method of RAM cheats.
	*/
	if (*a < 0x0100)
		*type = 0;
	else
		*type = 1;
	return(1);
}
