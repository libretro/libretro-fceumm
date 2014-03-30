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


LPDIRECTSOUND ppDS = 0;				/* DirectSound object. */
LPDIRECTSOUNDBUFFER ppbuf = 0;		/* Primary buffer object. */
LPDIRECTSOUNDBUFFER ppbufsec = 0;	/* Secondary buffer object. */
LPDIRECTSOUNDBUFFER ppbufw;			/* Buffer to actually write to. */

long DSBufferSize;					/* The size of the buffer that we can write to, in bytes. */

long BufHowMuch;					/* How many bytes we should try to buffer. */
DWORD ToWritePos;					/* Position which the next write to the buffer
									should write to. */



DSBUFFERDESC DSBufferDesc;
WAVEFORMATEX wfa;
WAVEFORMATEX wf;

static int bittage;

void TrashSound(void) {
	FCEUI_Sound(0);
	if (ppbufsec) {
		IDirectSoundBuffer_Stop(ppbufsec);
		IDirectSoundBuffer_Release(ppbufsec);
		ppbufsec = 0;
	}
	if (ppbuf) {
		IDirectSoundBuffer_Stop(ppbuf);
		IDirectSoundBuffer_Release(ppbuf);
		ppbuf = 0;
	}
	if (ppDS) {
		IDirectSound_Release(ppDS);
		ppDS = 0;
	}
}

void CheckDStatus(void) {
	DWORD status;
	status = 0;
	IDirectSoundBuffer_GetStatus(ppbufw, &status);

	if (status & DSBSTATUS_BUFFERLOST) {
		IDirectSoundBuffer_Restore(ppbufw);
	}

	if (!(status & DSBSTATUS_PLAYING)) {
		ToWritePos = 0;
		IDirectSoundBuffer_SetFormat(ppbufw, &wf);
		IDirectSoundBuffer_Play(ppbufw, 0, 0, DSBPLAY_LOOPING);
	}
}

static uint32_t RawCanWrite(void) {
	DWORD CurWritePos, CurPlayPos = 0;

	CheckDStatus();

	CurWritePos = 0;

	if (IDirectSoundBuffer_GetCurrentPosition(ppbufw, &CurPlayPos, &CurWritePos) == DS_OK) {
//   FCEU_DispMessage("%8d",(CurWritePos-CurPlayPos));
	}
	CurWritePos = (CurPlayPos + BufHowMuch) % DSBufferSize;

	/*  If the current write pos is >= half the buffer size less than the to write pos,
		assume DirectSound has wrapped around.
	*/

	if (((int32_t)ToWritePos - (int32_t)CurWritePos) >= (DSBufferSize / 2)) {
		CurWritePos += DSBufferSize;
		//FCEU_printf("Fixit: %d,%d,%d\n",ToWritePos,CurWritePos,CurWritePos-DSBufferSize);
	}
	if (ToWritePos < CurWritePos) {
		int32_t howmuch = (int32_t)CurWritePos - (int32_t)ToWritePos;
		if (howmuch > BufHowMuch) {	/* Oopsie.  Severe buffer overflow... */
			//FCEU_printf("Ack");
			ToWritePos = CurWritePos % DSBufferSize;
		}
		return(CurWritePos - ToWritePos);
	} else
		return(0);
}

int32 GetWriteSound(void) {
	return(RawCanWrite() >> bittage);
}

int32 GetMaxSound(void) {
	return(BufHowMuch >> bittage);
}

static int RawWrite(void *data, uint32_t len) {
// uint32_t cw;

	//printf("Pre: %d\n",SexyALI_DSound_RawCanWrite(device));
	//fflush(stdout);

	CheckDStatus();
	/* In this block, we write as much data as we can, then we write
	   the rest of it in >=1ms chunks.
	*/
	while (len) {
		VOID *LockPtr[2] = { 0, 0 };
		DWORD LockLen[2] = { 0, 0 };
		int32_t curlen;

		while (!(curlen = RawCanWrite())) {
			Sleep(1);
		}

		if (curlen > len) curlen = len;

		if (DS_OK == IDirectSoundBuffer_Lock(ppbufw, ToWritePos, curlen, &LockPtr[0], &LockLen[0], &LockPtr[1], &LockLen[1], 0)) {
		}

		if (LockPtr[1] != 0 && LockPtr[1] != LockPtr[0]) {
			memcpy(LockPtr[0], data, LockLen[0]);
			memcpy(LockPtr[1], data + LockLen[0], len - LockLen[0]);
		} else if (LockPtr[0]) {
			memcpy(LockPtr[0], data, curlen);
		}
		IDirectSoundBuffer_Unlock(ppbufw, LockPtr[0], LockLen[0], LockPtr[1], LockLen[1]);
		ToWritePos = (ToWritePos + curlen) % DSBufferSize;

		len -= curlen;
		data = (void*)((uint8_t*)data + curlen);
		if (len)
			Sleep(1);
	}	// end while(len) loop


	return(1);
}

void FCEUD_WriteSoundData(int32 *Buffer, int Count) {
	static int16 MBuffer[2 * 96000 / 50];	/* * 2 for safety. */
	int P;

	if (!bittage) {
		for (P = 0; P < Count; P++)
			*(((uint8*)MBuffer) + P) = ((int8)(Buffer[P] >> 8)) ^ 128;
		RawWrite(MBuffer, Count);
	} else {
		for (P = 0; P < Count; P++)
			MBuffer[P] = Buffer[P];
		//FCEU_printf("Pre: %d\n",RawCanWrite() / 2);
		RawWrite(MBuffer, Count * 2);
		//FCEU_printf("Post: %d\n",RawCanWrite() / 2);
	}
}

int InitSound() {
	DSCAPS dscaps;
	DSBCAPS dsbcaps;

	memset(&wf, 0x00, sizeof(wf));
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 1;
	wf.nSamplesPerSec = soundrate;

	ddrval = DirectSoundCreate(0, &ppDS, 0);
	if (ddrval != DS_OK) {
		FCEUD_PrintError("DirectSound: Error creating DirectSound object.");
		return 0;
	}

	if (soundoptions & SO_SECONDARY) {
 trysecondary:
		ddrval = IDirectSound_SetCooperativeLevel(ppDS, hAppWnd, DSSCL_PRIORITY);
		if (ddrval != DS_OK) {
			FCEUD_PrintError("DirectSound: Error setting cooperative level to DDSCL_PRIORITY.");
			TrashSound();
			return 0;
		}
	} else {
		ddrval = IDirectSound_SetCooperativeLevel(ppDS, hAppWnd, DSSCL_WRITEPRIMARY);
		if (ddrval != DS_OK) {
			FCEUD_PrintError("DirectSound: Error setting cooperative level to DDSCL_WRITEPRIMARY.  Forcing use of secondary sound buffer and trying again...");
			soundoptions |= SO_SECONDARY;
			goto trysecondary;
		}
	}
	memset(&dscaps, 0x00, sizeof(dscaps));
	dscaps.dwSize = sizeof(dscaps);
	ddrval = IDirectSound_GetCaps(ppDS, &dscaps);
	if (ddrval != DS_OK) {
		FCEUD_PrintError("DirectSound: Error getting capabilities.");
		return 0;
	}

	if (dscaps.dwFlags & DSCAPS_EMULDRIVER)
		FCEUD_PrintError("DirectSound: Sound device is being emulated through waveform-audio functions.  Sound quality will most likely be awful.  Try to update your sound device's sound drivers.");

	IDirectSound_Compact(ppDS);

	memset(&DSBufferDesc, 0x00, sizeof(DSBUFFERDESC));
	DSBufferDesc.dwSize = sizeof(DSBufferDesc);
	if (soundoptions & SO_SECONDARY)
		DSBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	else
		DSBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_GETCURRENTPOSITION2;

	ddrval = IDirectSound_CreateSoundBuffer(ppDS, &DSBufferDesc, &ppbuf, 0);
	if (ddrval != DS_OK) {
		FCEUD_PrintError("DirectSound: Error creating primary buffer.");
		TrashSound();
		return 0;
	}

	memset(&wfa, 0x00, sizeof(wfa));

	if (soundoptions & SO_FORCE8BIT)
		bittage = 0;
	else {
		bittage = 1;
		if ((!(dscaps.dwFlags & DSCAPS_PRIMARY16BIT) && !(soundoptions & SO_SECONDARY)) ||
			(!(dscaps.dwFlags & DSCAPS_SECONDARY16BIT) && (soundoptions & SO_SECONDARY))) {
			FCEUD_PrintError("DirectSound: 16-bit sound is not supported.  Forcing 8-bit sound.");
			bittage = 0;
			soundoptions |= SO_FORCE8BIT;
		}
	}

	wf.wBitsPerSample = 8 << bittage;
	wf.nBlockAlign = bittage + 1;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

	ddrval = IDirectSoundBuffer_SetFormat(ppbuf, &wf);
	if (ddrval != DS_OK) {
		FCEUD_PrintError("DirectSound: Error setting primary buffer format.");
		TrashSound();
		return 0;
	}

	IDirectSoundBuffer_GetFormat(ppbuf, &wfa, sizeof(wfa), 0);

	if (soundoptions & SO_SECONDARY) {
		memset(&DSBufferDesc, 0x00, sizeof(DSBUFFERDESC));
		DSBufferDesc.dwSize = sizeof(DSBufferDesc);
		DSBufferDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
		if (soundoptions & SO_GFOCUS)
			DSBufferDesc.dwFlags |= DSBCAPS_GLOBALFOCUS;
		DSBufferDesc.dwBufferBytes = 65536;
		DSBufferDesc.lpwfxFormat = &wfa;
		ddrval = IDirectSound_CreateSoundBuffer(ppDS, &DSBufferDesc, &ppbufsec, 0);
		if (ddrval != DS_OK) {
			FCEUD_PrintError("DirectSound: Error creating secondary buffer.");
			TrashSound();
			return 0;
		}
	}

	//sprintf(TempArray,"%d\n",wfa.nSamplesPerSec);
	//FCEUD_PrintError(TempArray);

	if (soundoptions & SO_SECONDARY) {
		DSBufferSize = 65536;
		IDirectSoundBuffer_SetCurrentPosition(ppbufsec, 0);
		ppbufw = ppbufsec;
	} else {
		memset(&dsbcaps, 0, sizeof(dsbcaps));
		dsbcaps.dwSize = sizeof(dsbcaps);
		ddrval = IDirectSoundBuffer_GetCaps(ppbuf, &dsbcaps);
		if (ddrval != DS_OK) {
			FCEUD_PrintError("DirectSound: Error getting buffer capabilities.");
			TrashSound();
			return 0;
		}

		DSBufferSize = dsbcaps.dwBufferBytes;

		if (DSBufferSize < 8192) {
			FCEUD_PrintError("DirectSound: Primary buffer size is too small!");
			TrashSound();
			return 0;
		}
		ppbufw = ppbuf;
	}

	BufHowMuch = (soundbuftime * soundrate / 1000) << bittage;
	FCEUI_Sound(soundrate);
	return 1;
}

static HWND uug = 0;

static void UpdateSD(HWND hwndDlg) {
	int t;

	CheckDlgButton(hwndDlg, 126, soundo ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwndDlg, 122, (soundoptions & SO_FORCE8BIT) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwndDlg, 123, (soundoptions & SO_SECONDARY) ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hwndDlg, 124, (soundoptions & SO_GFOCUS) ? BST_CHECKED : BST_UNCHECKED);
	SendDlgItemMessage(hwndDlg, 129, CB_SETCURSEL, soundquality, (LPARAM)(LPSTR)0);
	t = 0;
	if (soundrate == 22050) t = 1;
	else if (soundrate == 44100) t = 2;
	else if (soundrate == 48000) t = 3;
	else if (soundrate == 96000) t = 4;
	SendDlgItemMessage(hwndDlg, 200, CB_SETCURSEL, t, (LPARAM)(LPSTR)0);
}

BOOL CALLBACK SoundConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCLBUTTONDOWN: StopSound(); break;

	case WM_INITDIALOG:
		/* Volume Trackbar */
		SendDlgItemMessage(hwndDlg, 500, TBM_SETRANGE, 1, MAKELONG(0, 150));
		SendDlgItemMessage(hwndDlg, 500, TBM_SETTICFREQ, 25, 0);
		SendDlgItemMessage(hwndDlg, 500, TBM_SETPOS, 1, 150 - soundvolume);

		/* buffer size time trackbar */
		SendDlgItemMessage(hwndDlg, 128, TBM_SETRANGE, 1, MAKELONG(15, 200));
		SendDlgItemMessage(hwndDlg, 128, TBM_SETTICFREQ, 1, 0);
		SendDlgItemMessage(hwndDlg, 128, TBM_SETPOS, 1, soundbuftime);

		{
			char tbuf[8];
			sprintf(tbuf, "%d", soundbuftime);
			SetDlgItemText(hwndDlg, 666, (LPTSTR)tbuf);
		}

		SendDlgItemMessage(hwndDlg, 129, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"Low");
		SendDlgItemMessage(hwndDlg, 129, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"High");
		SendDlgItemMessage(hwndDlg, 129, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"Highest");

		SendDlgItemMessage(hwndDlg, 200, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"11025");
		SendDlgItemMessage(hwndDlg, 200, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"22050");
		SendDlgItemMessage(hwndDlg, 200, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"44100");
		SendDlgItemMessage(hwndDlg, 200, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"48000");
		SendDlgItemMessage(hwndDlg, 200, CB_ADDSTRING, 0, (LPARAM)(LPSTR)"96000");

		UpdateSD(hwndDlg);
		break;
	case WM_VSCROLL:
		soundvolume = 150 - SendDlgItemMessage(hwndDlg, 500, TBM_GETPOS, 0, 0);
		FCEUI_SetSoundVolume(soundvolume);
		break;
	case WM_HSCROLL:
	{
		char tbuf[8];
		soundbuftime = SendDlgItemMessage(hwndDlg, 128, TBM_GETPOS, 0, 0);
		sprintf(tbuf, "%d", soundbuftime);
		SetDlgItemText(hwndDlg, 666, (LPTSTR)tbuf);
		BufHowMuch = (soundbuftime * soundrate / 1000) << bittage;
		//soundbufsize=(soundbuftime*soundrate/1000);
	}
	break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		switch (HIWORD(wParam)) {
		case CBN_SELENDOK:
			switch (LOWORD(wParam)) {
			case 200:
			{
				int tmp;
				tmp = SendDlgItemMessage(hwndDlg, 200, CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				if (tmp == 0) tmp = 11025;
				else if (tmp == 1) tmp = 22050;
				else if (tmp == 2) tmp = 44100;
				else if (tmp == 3) tmp = 48000;
				else tmp = 96000;
				if (tmp != soundrate) {
					soundrate = tmp;
					if (soundrate < 44100) {
						soundquality = 0;
						FCEUI_SetSoundQuality(0);
						UpdateSD(hwndDlg);
					}
					if (soundo) {
						TrashSound();
						soundo = InitSound();
						UpdateSD(hwndDlg);
					}
				}
			}
			break;

			case 129:
				soundquality = SendDlgItemMessage(hwndDlg, 129, CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				if (soundrate < 44100) soundquality = 0;
				FCEUI_SetSoundQuality(soundquality);
				UpdateSD(hwndDlg);
				break;
			}
			break;

		case BN_CLICKED:
			switch (LOWORD(wParam)) {
			case 122: soundoptions ^= SO_FORCE8BIT;
				if (soundo) {
					TrashSound();
					soundo = InitSound();
					UpdateSD(hwndDlg);
				}
				break;
			case 123: soundoptions ^= SO_SECONDARY;
				if (soundo) {
					TrashSound();
					soundo = InitSound();
					UpdateSD(hwndDlg);
				}
				break;
			case 124: soundoptions ^= SO_GFOCUS;
				if (soundo) {
					TrashSound();
					soundo = InitSound();
					UpdateSD(hwndDlg);
				}
				break;
			case 126: soundo = !soundo;
				if (!soundo) TrashSound();
				else soundo = InitSound();
				UpdateSD(hwndDlg);
				break;
			}
		}

		if (!(wParam >> 16))
			switch (wParam & 0xFFFF) {
			case 1:
 gornk:
				DestroyWindow(hwndDlg);
				uug = 0;
				break;
			}
	}
	return 0;
}


void ConfigSound(void) {
	if (!uug)
		uug = CreateDialog(fceu_hInstance, "SOUNDCONFIG", 0, SoundConCallB);
	else
		SetFocus(uug);
}


void StopSound(void) {
	if (soundo) {
		VOID *LockPtr = 0;
		DWORD LockLen = 0;
		if (DS_OK == IDirectSoundBuffer_Lock(ppbufw, 0, DSBufferSize, &LockPtr, &LockLen, 0, 0, 0)) {
			//FCEUD_PrintError("K");
			if (bittage)
				memset(LockPtr, 0, LockLen);
			else
				memset(LockPtr, 0x80, LockLen);
			IDirectSoundBuffer_Unlock(ppbufw, LockPtr, LockLen, 0, 0);
		}

		//IDirectSoundBuffer_Stop(ppbufw);
	}
}

#include "wave.c"
