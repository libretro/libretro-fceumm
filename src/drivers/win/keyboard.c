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

#include "common.h"
#include <dinput.h>

#include "input.h"
#include "keyboard.h"

HRESULT ddrval;

static LPDIRECTINPUTDEVICE7 lpdid = 0;

void KeyboardClose(void) {
	if (lpdid) IDirectInputDevice7_Unacquire(lpdid);
	lpdid = 0;
}

static char keys[256];
void KeyboardUpdateState(void) {
	char tk[256];

	ddrval = IDirectInputDevice7_GetDeviceState(lpdid, 256, tk);
	switch (ddrval) {
	case DI_OK: memcpy(keys, tk, 256); break;
	case DIERR_INPUTLOST:
	case DIERR_NOTACQUIRED:
		memset(keys, 0, 256);
		IDirectInputDevice7_Acquire(lpdid);
		break;
	}
}

char *GetKeyboard(void) {
	return(keys);
}

int KeyboardInitialize(void) {
	if (lpdid)
		return(1);

	ddrval = IDirectInput7_CreateDeviceEx(lpDI, &GUID_SysKeyboard, &IID_IDirectInputDevice7, (LPVOID*)&lpdid, 0);
	if (ddrval != DI_OK) {
		FCEUD_PrintError("DirectInput: Error creating keyboard device.");
		return 0;
	}

	ddrval = IDirectInputDevice7_SetCooperativeLevel(lpdid, hAppWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (ddrval != DI_OK) {
		FCEUD_PrintError("DirectInput: Error setting keyboard cooperative level.");
		return 0;
	}

	ddrval = IDirectInputDevice7_SetDataFormat(lpdid, &c_dfDIKeyboard);
	if (ddrval != DI_OK) {
		FCEUD_PrintError("DirectInput: Error setting keyboard data format.");
		return 0;
	}

	ddrval = IDirectInputDevice7_Acquire(lpdid);
	/* Not really a fatal error. */
	//if(ddrval != DI_OK)
	//{
	// FCEUD_PrintError("DirectInput: Error acquiring keyboard.");
	// return 0;
	//}
	return 1;
}
