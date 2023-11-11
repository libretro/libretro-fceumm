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

#ifndef _FCEU_UNIF_H
#define _FCEU_UNIF_H

void BMCFK23CA_Init(CartInfo *);
void BMCFK23C_Init(CartInfo *);
void BMCT2271_Init(CartInfo *);
void EKROM_Init(CartInfo *);
void ELROM_Init(CartInfo *);
void ETROM_Init(CartInfo *);
void EWROM_Init(CartInfo *);
void GNROM_Init(CartInfo *);
void S74LS374NA_Init(CartInfo *);
void SA0161M_Init(CartInfo *);
void SAROM_Init(CartInfo *);
void SKROM_Init(CartInfo *);
void SNROM_Init(CartInfo *);
void SOROM_Init(CartInfo *);
void SSSNROM_Init(CartInfo *);
void Super24_Init(CartInfo *);
void Transformer_Init(CartInfo *);
void UNL3DBlock_Init(CartInfo *);
void UNL43272_Init(CartInfo *);
void COOLBOY_Init(CartInfo *);
void MINDKIDS_Init(CartInfo *);			/* m268 */
void BMC810131C_Init(CartInfo *);
void COOLGIRL_Init(CartInfo *);			/* m342 */
void KG256_Init(CartInfo *);
void WAIXINGFS005_Init(CartInfo *);

extern uint8 *UNIFchrrama;	/* Meh.  So I can stop CHR RAM
							 * bank switcherooing with certain boards...
							 */

#endif
