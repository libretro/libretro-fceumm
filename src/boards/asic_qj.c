/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2025 NewRisingSun
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
#include "asic_QJ.h"
#include "asic_mmc3.h"

static void (*QJ_cbSync)();
static uint8 QJ_reg;

static SFORMAT QJ_state[] = {
	{&QJ_reg,   1, "QJRG" },
	{ 0 }
};

void QJ_syncPRG (int AND, int OR) {
	MMC3_syncPRG(0x0F &AND, QJ_reg <<4 &AND | OR &~AND);
}

void QJ_syncCHR (int AND, int OR) {
	MMC3_syncCHR(0x7F &AND, QJ_reg <<7 &AND | OR &~AND);
}

void QJ_syncMirror () {
	MMC3_syncMirror();
}

DECLFW(QJ_writeWRAM) {
	QJ_reg = V;
	QJ_cbSync();
}

static void QJ_clear () {
	QJ_reg = 0;
	QJ_cbSync();
}

static void QJ_setHandlers () {
}

static void QJ_configure (void (*sync)()) {
	QJ_cbSync = sync;
}

void QJ_activate (uint8 clear, void (*sync)()) {
	MMC3_activate(clear, sync, MMC3_TYPE_SHARP, NULL, NULL, NULL, QJ_writeWRAM);
	QJ_configure(sync);
	QJ_setHandlers();
	if (clear)
		QJ_clear();
	else
		QJ_cbSync();
}

void QJ_addExState () {
	AddExState(QJ_state, ~0, 0, 0);
}

void QJ_restore (int version) {
	QJ_cbSync();
}

void QJ_power () {
	MMC3_power();
	QJ_setHandlers();
	QJ_clear();
}

void QJ_init (CartInfo *info, void (*sync)()) {
	MMC3_addExState();
	QJ_addExState();
	QJ_configure(QJ_cbSync);
	info->Power = QJ_power;
	info->Reset = QJ_cbSync;
	GameStateRestore = QJ_restore;
}
