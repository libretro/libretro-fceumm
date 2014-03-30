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


#include "state.c"		// Save/Load state AS

void DSMFix(UINT msg) {
	switch (msg) {
	case WM_VSCROLL:
	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_ENTERMENULOOP: StopSound(); break;
	}
}
static void ConfigGUI(void);
static void ConfigTiming(void);
static void ConfigPalette(void);
static void ConfigDirectories(void);

void ShowNetplayConsole(void);

static HMENU fceumenu = 0;

static int tog = 0;

void ShowCursorAbs(int w) {
	static int stat = 0;
	if (w) {
		if (stat == -1) {
			stat++; ShowCursor(1);
		}
	} else {
		if (stat == 0) {
			stat--; ShowCursor(0);
		}
	}
}


void CalcWindowSize(RECT *al) {
	al->left = 0;
	al->right = VNSWID * winsizemulx;
	al->top = 0;
	al->bottom = totallines * winsizemuly;

	AdjustWindowRectEx(al, GetWindowLong(hAppWnd, GWL_STYLE), GetMenu(hAppWnd) != NULL, GetWindowLong(hAppWnd, GWL_EXSTYLE));

	al->right -= al->left;
	al->left = 0;
	al->bottom -= al->top;
	al->top = 0;
}


void RedoMenuGI(FCEUGI *gi) {
	int simpled[] = { 101, 111, 110, 200, 201, 204, 203, 0 };
	int x;

	x = 0;
	while (simpled[x]) {
		#ifndef FCEUDEF_DEBUGGER
		if (simpled[x] == 203)
			EnableMenuItem(fceumenu, simpled[x], MF_BYCOMMAND | MF_GRAYED);
		else
		#endif
			EnableMenuItem(fceumenu, simpled[x], MF_BYCOMMAND | (gi ? MF_ENABLED : MF_GRAYED));
		x++;
	}
}

void UpdateMenu(void) {
	static int *polo[2] = { &genie, &palyo };
	static int polo2[2] = { 310, 311 };
	int x;

	for (x = 0; x < 2; x++)
		CheckMenuItem(fceumenu, polo2[x], *polo[x] ? MF_CHECKED : MF_UNCHECKED);
	if (eoptions & EO_BGRUN)
		CheckMenuItem(fceumenu, 301, MF_CHECKED);
	else
		CheckMenuItem(fceumenu, 301, MF_UNCHECKED);
}

static HMENU recentmenu, recentdmenu;
char *rfiles[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
char *rdirs[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void UpdateRMenu(HMENU menu, char **strs, int mitem, int baseid) {
	MENUITEMINFO moo;
	int x;

	moo.cbSize = sizeof(moo);
	moo.fMask = MIIM_SUBMENU | MIIM_STATE;

	GetMenuItemInfo(GetSubMenu(fceumenu, 0), mitem, FALSE, &moo);
	moo.hSubMenu = menu;
	moo.fState = strs[0] ? MFS_ENABLED : MFS_GRAYED;

	SetMenuItemInfo(GetSubMenu(fceumenu, 0), mitem, FALSE, &moo);

	for (x = 0; x < 10; x++)
		RemoveMenu(menu, baseid + x, MF_BYCOMMAND);
	for (x = 9; x >= 0; x--) {
		char tmp[128 + 5];
		if (!strs[x]) continue;

		moo.cbSize = sizeof(moo);
		moo.fMask = MIIM_DATA | MIIM_ID | MIIM_TYPE;

		if (strlen(strs[x]) < 128) {
			sprintf(tmp, "&%d. %s", (x + 1) % 10, strs[x]);
		} else
			sprintf(tmp, "&%d. %s", (x + 1) % 10, strs[x] + strlen(strs[x]) - 127);

		moo.cch = strlen(tmp);
		moo.fType = 0;
		moo.wID = baseid + x;
		moo.dwTypeData = tmp;
		InsertMenuItem(menu, 0, 1, &moo);
	}
	DrawMenuBar(hAppWnd);
}

void AddRecent(char *fn) {
	int x;

	for (x = 0; x < 10; x++)
		if (rfiles[x])
			if (!strcmp(rfiles[x], fn)) {		// Item is already in list.
				int y;
				char *tmp;

				tmp = rfiles[x];				// Save pointer.
				for (y = x; y; y--)
					rfiles[y] = rfiles[y - 1];	// Move items down.

				rfiles[0] = tmp;				// Put item on top.
				UpdateRMenu(recentmenu, rfiles, 102, 600);
				return;
			}

	if (rfiles[9]) free(rfiles[9]);
	for (x = 9; x; x--) rfiles[x] = rfiles[x - 1];
	rfiles[0] = malloc(strlen(fn) + 1);
	strcpy(rfiles[0], fn);
	UpdateRMenu(recentmenu, rfiles, 102, 600);
}

void AddRecentDir(char *fn) {
	int x;

	for (x = 0; x < 10; x++)
		if (rdirs[x])
			if (!strcmp(rdirs[x], fn)) {		// Item is already in list.
				int y;
				char *tmp;

				tmp = rdirs[x];					// Save pointer.
				for (y = x; y; y--)
					rdirs[y] = rdirs[y - 1];	// Move items down.

				rdirs[0] = tmp;					// Put item on top.
				UpdateRMenu(recentdmenu, rdirs, 103, 700);
				return;
			}

	if (rdirs[9]) free(rdirs[9]);
	for (x = 9; x; x--) rdirs[x] = rdirs[x - 1];
	rdirs[0] = malloc(strlen(fn) + 1);
	strcpy(rdirs[0], fn);
	UpdateRMenu(recentdmenu, rdirs, 103, 700);
}



void HideMenu(int h) {
	if (h) {
		SetMenu(hAppWnd, 0);
	} else {
		SetMenu(hAppWnd, fceumenu);
	}
}

static LONG WindowXC = 1 << 30, WindowYC;
void HideFWindow(int h) {
	LONG desa;

	if (h) {	// Full-screen
		RECT bo;
		GetWindowRect(hAppWnd, &bo);
		WindowXC = bo.left;
		WindowYC = bo.top;

		SetMenu(hAppWnd, 0);
		desa = WS_POPUP | WS_CLIPSIBLINGS;
	} else {
		desa = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS;
		HideMenu(tog);
		// Stupid DirectDraw bug(I think?) requires this.  Need to investigate it.
		SetWindowPos(hAppWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE);
	}

	SetWindowLong(hAppWnd, GWL_STYLE, desa | (GetWindowLong(hAppWnd, GWL_STYLE) & WS_VISIBLE));
	SetWindowPos(hAppWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);
}

void ToggleHideMenu(void) {
	if (!fullscreen) {
		tog ^= 1;
		HideMenu(tog);
		SetMainWindowStuff();
	}
}

static void ALoad(char *nameo) {
	if ((GI = FCEUI_LoadGame(nameo))) {
		FixFL();
		SetMainWindowStuff();
		AddRecent(nameo);
		RefreshThrottleFPS();
		if (eoptions & EO_HIDEMENU && !tog)
			ToggleHideMenu();
		if (eoptions & EO_FSAFTERLOAD)
			SetFSVideoMode();
	} else
		StopSound();
	ParseGIInput(GI);
	RedoMenuGI(GI);
}

void LoadNewGamey(HWND hParent, char *initialdir) {
	const char filter[] = "All usable files(*.nes,*.nsf,*.fds,*.unf,*.zip,*.gz)\0*.nes;*.nsf;*.fds;*.unf;*.zip;*.gz\0All non-compressed usable files(*.nes,*.nsf,*.fds,*.unf)\0*.nes;*.nsf;*.fds;*.unf\0All files (*.*)\0*.*\0";
	char nameo[2048];
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = fceu_hInstance;
	ofn.lpstrTitle = "FCE Ultra Open File...";
	ofn.lpstrFilter = filter;
	nameo[0] = 0;
	ofn.hwndOwner = hParent;
	ofn.lpstrFile = nameo;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = initialdir ? initialdir : gfsdir;
	if (GetOpenFileName(&ofn)) {
		char *tmpdir;

		if ((tmpdir = malloc(ofn.nFileOffset + 1))) {
			strncpy(tmpdir, ofn.lpstrFile, ofn.nFileOffset);
			tmpdir[ofn.nFileOffset] = 0;
			AddRecentDir(tmpdir);

			if (!initialdir) {	// Prevent setting the File->Open default
								// directory when a "Recent Directory" is selected.
				if (gfsdir)
					free(gfsdir);
				gfsdir = tmpdir;
			} else
				free(tmpdir);
		}
		ALoad(nameo);
	}
}

#ifdef COPYFAMI
void CopyFamiStart(HWND hParent, char *initialdir) {
	if (GI = FCEUI_CopyFamiStart()) {
		FixFL();
		SetMainWindowStuff();
		RefreshThrottleFPS();
		if (eoptions & EO_HIDEMENU && !tog)
			ToggleHideMenu();
		if (eoptions & EO_FSAFTERLOAD)
			SetFSVideoMode();
	} else
		StopSound();
	ParseGIInput(GI);
	RedoMenuGI(GI);
}
#endif

static uint32 mousex, mousey, mouseb;
void GetMouseData(uint32 *md) {
	md[0] = mousex;
	md[1] = mousey;
	if (!fullscreen) {
		if (ismaximized) {
			RECT t;
			GetClientRect(hAppWnd, &t);
			md[0] = md[0] * VNSWID / (t.right ? t.right : 1);
			md[1] = md[1] * totallines / (t.bottom ? t.bottom : 1);
		} else {
			md[0] /= winsizemulx;
			md[1] /= winsizemuly;
		}
		md[0] += VNSCLIP;
	}

	md[1] += srendline;
	md[2] = ((mouseb == MK_LBUTTON) ? 1 : 0) | ((mouseb == MK_RBUTTON) ? 2 : 0);
}

static int vchanged = 0;

LRESULT FAR PASCAL AppWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	DSMFix(msg);
	switch (msg) {
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		mouseb = wParam;
		goto proco;
	case WM_MOUSEMOVE:
	{
		mousex = LOWORD(lParam);
		mousey = HIWORD(lParam);
	}
		goto proco;
	case WM_ERASEBKGND:
		if (xbsave)
			return(0);
		else
			goto proco;
	case WM_PAINT: if (xbsave) {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			FCEUD_BlitScreen(xbsave);
			EndPaint(hWnd, &ps);
			return(0);
	}
		goto proco;
	case WM_SIZE:
		if (!fullscreen && !changerecursive)
			switch (wParam) {
			case SIZE_MAXIMIZED: ismaximized = 1; SetMainWindowStuff(); break;
			case SIZE_RESTORED: ismaximized = 0; SetMainWindowStuff(); break;
			}
		break;
	case WM_SIZING:
	{
		RECT *wrect = (RECT*)lParam;
		RECT srect;

		int h = wrect->bottom - wrect->top;
		int w = wrect->right - wrect->left;
		int how = 0;

		if (wParam == WMSZ_BOTTOM || wParam == WMSZ_TOP)
			how = 2;
		else if (wParam == WMSZ_LEFT || wParam == WMSZ_RIGHT)
			how = 1;
		else if (wParam == WMSZ_BOTTOMLEFT || wParam == WMSZ_BOTTOMRIGHT
				 || wParam == WMSZ_TOPRIGHT || wParam == WMSZ_TOPLEFT)
			how = 3;
		if (how & 1)
			winsizemulx *= (double)w / winwidth;
		if (how & 2)
			winsizemuly *= (double)h / winheight;
		if (how & 1) FixWXY(0);
		else FixWXY(1);

		CalcWindowSize(&srect);
		winwidth = srect.right;
		winheight = srect.bottom;
		wrect->right = wrect->left + srect.right;
		wrect->bottom = wrect->top + srect.bottom;
	}
		goto proco;
	case WM_DISPLAYCHANGE:
		if (!fullscreen && !changerecursive)
			vchanged = 1;
		goto proco;
	case WM_DROPFILES:
	{
		UINT len;
		char *ftmp;

		len = DragQueryFile((HANDLE)wParam, 0, 0, 0) + 1;
		if ((ftmp = malloc(len))) {
			DragQueryFile((HANDLE)wParam, 0, ftmp, len);
			ALoad(ftmp);
			free(ftmp);
		}
	}
	break;
	case WM_COMMAND:
		if (!(wParam >> 16)) {
			wParam &= 0xFFFF;
			if (wParam >= 600 && wParam <= 609) {			// Recent files
				if (rfiles[wParam - 600]) ALoad(rfiles[wParam - 600]);
			} else if (wParam >= 700 && wParam <= 709) {	// Recent dirs
				if (rdirs[wParam - 700])
					LoadNewGamey(hWnd, rdirs[wParam - 700]);
			}
			switch (wParam) {
			case 300: ToggleHideMenu(); break;
			case 301: eoptions ^= EO_BGRUN; UpdateMenu(); break;

			case 310: genie ^= 1; FCEUI_SetGameGenie(genie); UpdateMenu(); break;
			case 311: palyo ^= 1;
				FCEUI_SetVidSystem(palyo);
				RefreshThrottleFPS();
				UpdateMenu();
				FixFL();
				SetMainWindowStuff();
				break;

			case 320: StopSound(); ConfigDirectories(); break;
			case 327: StopSound(); ConfigGUI(); break;
			case 321: StopSound(); ConfigInput(hWnd); break;
			case 322: ConfigTiming(); break;
			case 323: StopSound(); ShowNetplayConsole(); break;
			case 324: StopSound(); ConfigPalette(); break;
			case 325: StopSound(); ConfigSound(); break;
			case 326: ConfigVideo(); break;

			case 200: FCEUI_ResetNES(); break;
			case 201: FCEUI_PowerNES(); break;

#ifdef FCEUDEF_DEBUGGER
			case 203: BeginDSeq(hWnd); break;
#endif

			case 204: ConfigAddCheat(hWnd); break;
			case 205: DoPPUView(); break;
			case 100: StopSound();
				LoadNewGamey(hWnd, 0);
				break;
#ifdef COPYFAMI
			case 140: StopSound();
				CopyFamiStart(hWnd, 0);
				break;
#endif
			case 101: DoFCEUExit(); break;
			case 110: SaveStateAs(); break;
			case 111: LoadStateFrom(); break;

			case 120:
			{
				MENUITEMINFO mi;
				char *str;

				StopSound();
				if (CreateSoundSave())
					str = "Stop Sound Logging";
				else
					str = "Log Sound As...";
				memset(&mi, 0, sizeof(mi));
				mi.fMask = MIIM_DATA | MIIM_TYPE;
				mi.cbSize = sizeof(mi);
				GetMenuItemInfo(fceumenu, 120, 0, &mi);
				mi.fMask = MIIM_DATA | MIIM_TYPE;
				mi.cbSize = sizeof(mi);
				mi.dwTypeData = str;
				mi.cch = strlen(str);
				SetMenuItemInfo(fceumenu, 120, 0, &mi);
			}
			break;
			case 130: DoFCEUExit(); break;
			case 400: StopSound(); ShowAboutBox(); break;
			case 401: MakeLogWindow(); break;
			}
		}
		break;


	case WM_SYSCOMMAND:
		if (GI && wParam == SC_SCREENSAVE && (goptions & GOO_DISABLESS))
			return(0);

		if (wParam == SC_KEYMENU) {
			if (GI && InputType[2] == SIFC_FKB && cidisabled)
				break;
			if (GI && InputType[2] == SIFC_SUBORKB && cidisabled)
				break;
			if (GI && InputType[2] == SIFC_PEC586KB && cidisabled)
				break;
			if (lParam == VK_RETURN || fullscreen || tog) break;
		}
		goto proco;
	case WM_SYSKEYDOWN:
		if (GI && InputType[2] == SIFC_FKB && cidisabled)
			break;		// Hopefully this won't break DInput...
		if (GI && InputType[2] == SIFC_SUBORKB && cidisabled)
			break;
		if (GI && InputType[2] == SIFC_PEC586KB && cidisabled)
			break;

		if (fullscreen || tog) {
			if (wParam == VK_MENU)
				break;
		}
		if (wParam == VK_F10) {
			if (!moocow) FCEUD_PrintError("Iyee");
			if (!(lParam & 0x40000000))
				FCEUI_ResetNES();
			break;
		}

		if (wParam == VK_RETURN) {
			if (!(lParam & (1 << 30))) {
				UpdateMenu();
				changerecursive = 1;
				if (!SetVideoMode(fullscreen ^ 1))
					SetVideoMode(fullscreen);
				changerecursive = 0;
			}
			break;
		}
		goto proco;

	case WM_KEYDOWN:
		if (GI) {
			// Only disable command keys if a game is loaded(and the other
			// conditions are right, of course).
			if (InputType[2] == SIFC_FKB) {
				if (wParam == VK_SCROLL) {
					cidisabled ^= 1;
					FCEUI_DispMessage("Family Keyboard %sabled.", cidisabled ? "en" : "dis");
				}
				if (cidisabled)
					break;	// Hopefully this won't break DInput...
			}
			if (InputType[2] == SIFC_SUBORKB) {
				if (wParam == VK_SCROLL) {
					cidisabled ^= 1;
					FCEUI_DispMessage("Subor Keyboard %sabled.", cidisabled ? "en" : "dis");
				}
				if (cidisabled)
					break;	// Hopefully this won't break DInput...
			}
			if (InputType[2] == SIFC_PEC586KB) {
				if (wParam == VK_SCROLL) {
					cidisabled ^= 1;
					FCEUI_DispMessage("PEC586 Keyboard %sabled.", cidisabled ? "en" : "dis");
				}
				if (cidisabled)
					break;	// Hopefully this won't break DInput...
			}
		}
		if (!(lParam & 0x40000000))
			switch (wParam) {
			case VK_F11: FCEUI_PowerNES(); break;
			case VK_ESCAPE:
			case VK_F12: DoFCEUExit(); break;
			case VK_F2: userpause ^= 1; break;
			case VK_F3: ToggleHideMenu(); break;
			}
		goto proco;
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT: DoFCEUExit(); break;
	case WM_ACTIVATEAPP:
		if ((BOOL)wParam) {
			nofocus = 0;
		} else {
			nofocus = 1;
		}
	default:
proco:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void FixWXY(int pref) {
	if (eoptions & EO_FORCEASPECT) {
		// First, make sure the ratio is valid, and if it's not, change
		// it so that it doesn't break everything.
		if (saspectw < 0.01) saspectw = 0.01;
		if (saspecth < 0.01) saspecth = 0.01;
		if ((saspectw / saspecth) > 100) saspecth = saspectw;
		if ((saspecth / saspectw) > 100) saspectw = saspecth;

		if ((saspectw / saspecth) < 0.1) saspecth = saspectw;
		if ((saspecth / saspectw) > 0.1) saspectw = saspecth;

		if (!pref) {
			winsizemuly = winsizemulx * 256 / 240 * 3 / 4 * saspectw / saspecth;
		} else {
			winsizemulx = winsizemuly * 240 / 256 * 4 / 3 * saspecth / saspectw;
		}
	}
	if (winspecial) {
		int mult;
		if (winspecial == 1 || winspecial == 2) mult = 2;
		else mult = 3;
		if (winsizemulx < mult) {
			if (eoptions & EO_FORCEASPECT)
				winsizemuly *= mult / winsizemulx;
			winsizemulx = mult;
		}
		if (winsizemuly < mult) {
			if (eoptions & EO_FORCEASPECT)
				winsizemulx *= mult / winsizemuly;
			winsizemuly = mult;
		}
	}

	if (winsizemulx < 0.1)
		winsizemulx = 0.1;
	if (winsizemuly < 0.1)
		winsizemuly = 0.1;

	if (eoptions & EO_FORCEISCALE) {
		int x, y;

		x = winsizemulx * 2;
		y = winsizemuly * 2;

		x = (x >> 1) + (x & 1);
		y = (y >> 1) + (y & 1);

		if (!x) x = 1;
		if (!y) y = 1;

		winsizemulx = x;
		winsizemuly = y;
	}

	if (winsizemulx > 100) winsizemulx = 100;
	if (winsizemuly > 100) winsizemuly = 100;
}

void UpdateFCEUWindow(void) {
	if (vchanged && !fullscreen && !changerecursive && !nofocus) {
		SetVideoMode(0);
		vchanged = 0;
	}

	BlockingCheck();
#ifdef FCEUDEF_DEBUGGER
	UpdateDebugger();
#endif

	if (!(eoptions & EO_BGRUN))
		while (nofocus) {
			StopSound();
			Sleep(75);
			BlockingCheck();
		}
	if (userpause) {
		StopSound();
		while (userpause) {
			Sleep(50);
			BlockingCheck();
		}
	}
}

void ByebyeWindow(void) {
	SetMenu(hAppWnd, 0);
	DestroyMenu(fceumenu);
	DestroyWindow(hAppWnd);
}

int CreateMainWindow(void) {
	WNDCLASSEX winclass;
	RECT tmp;

	memset(&winclass, 0, sizeof(winclass));
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
	winclass.lpfnWndProc = AppWndProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = fceu_hInstance;
	winclass.hIcon = LoadIcon(fceu_hInstance, "ICON_1");
	winclass.hIconSm = LoadIcon(fceu_hInstance, "ICON_1");
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = GetStockObject(BLACK_BRUSH);
	winclass.lpszClassName = "FCEULTRA";

	if (!RegisterClassEx(&winclass))
		return FALSE;

	AdjustWindowRectEx(&tmp, WS_OVERLAPPEDWINDOW, 1, 0);

	fceumenu = LoadMenu(fceu_hInstance, "FCEUMENU");
#ifndef COPYFAMI
	DeleteMenu(fceumenu, 140, MF_BYCOMMAND);
#endif
	recentmenu = CreateMenu();
	recentdmenu = CreateMenu();

	UpdateRMenu(recentmenu, rfiles, 102, 600);
	UpdateRMenu(recentdmenu, rdirs, 103, 700);

	RedoMenuGI(NULL);

	hAppWnd = CreateWindowEx(0, "FCEULTRA", "FCE Ultra",
							 WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS,		/* Style */
							 CW_USEDEFAULT, CW_USEDEFAULT, 256, 240,	/* X,Y ; Width, Height */
							 NULL, fceumenu, fceu_hInstance, NULL);
	DragAcceptFiles(hAppWnd, 1);
	SetMainWindowStuff();
	return 1;
}


int SetMainWindowStuff(void) {
	RECT tmp;

	GetWindowRect(hAppWnd, &tmp);

	if (ismaximized) {
		winwidth = tmp.right - tmp.left;
		winheight = tmp.bottom - tmp.top;

		ShowWindow(hAppWnd, SW_SHOWMAXIMIZED);
	} else {
		RECT srect;
		if (WindowXC != (1 << 30)) {
			/* Subtracting and adding for if(eoptions&EO_USERFORCE) below. */
			tmp.bottom -= tmp.top;
			tmp.bottom += WindowYC;

			tmp.right -= tmp.left;
			tmp.right += WindowXC;


			tmp.left = WindowXC;
			tmp.top = WindowYC;
			WindowXC = 1 << 30;
		}

		CalcWindowSize(&srect);
		SetWindowPos(hAppWnd, HWND_TOP, tmp.left, tmp.top, srect.right, srect.bottom, SWP_SHOWWINDOW);
		winwidth = srect.right;
		winheight = srect.bottom;

		ShowWindow(hAppWnd, SW_SHOWNORMAL);
	}
	return 1;
}

int GetClientAbsRect(LPRECT lpRect) {
	POINT point;
	point.x = point.y = 0;
	if (!ClientToScreen(hAppWnd, &point)) return 0;

	lpRect->top = point.y;
	lpRect->left = point.x;

	if (ismaximized) {
		RECT al;
		GetClientRect(hAppWnd, &al);
		lpRect->right = point.x + al.right;
		lpRect->bottom = point.y + al.bottom;
	} else {
		lpRect->right = point.x + VNSWID * winsizemulx;
		lpRect->bottom = point.y + totallines * winsizemuly;
	}
	return 1;
}


int LoadPaletteFile(void) {
	FILE *fp;
	const char filter[] = "All usable files(*.pal)\0*.pal\0All files (*.*)\0*.*\0";
	char nameo[2048];
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hInstance = fceu_hInstance;
	ofn.lpstrTitle = "FCE Ultra Open Palette File...";
	ofn.lpstrFilter = filter;
	nameo[0] = 0;
	ofn.lpstrFile = nameo;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrInitialDir = 0;
	if (GetOpenFileName(&ofn)) {
		if ((fp = FCEUD_UTF8fopen(nameo, "rb"))) {
			fread(cpalette, 1, 192, fp);
			fclose(fp);
			FCEUI_SetPaletteArray(cpalette);
			eoptions |= EO_CPALETTE;
			return(1);
		} else
			FCEUD_PrintError("Error opening palette file!");
	}
	return(0);
}
static HWND pwindow;
static BOOL CALLBACK PaletteConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DSMFix(uMsg);
	switch (uMsg) {
	case WM_INITDIALOG:
		if (ntsccol)
			CheckDlgButton(hwndDlg, 100, BST_CHECKED);
		SendDlgItemMessage(hwndDlg, 500, TBM_SETRANGE, 1, MAKELONG(0, 128));
		SendDlgItemMessage(hwndDlg, 501, TBM_SETRANGE, 1, MAKELONG(0, 128));
		FCEUI_GetNTSCTH(&ntsctint, &ntschue);
		SendDlgItemMessage(hwndDlg, 500, TBM_SETPOS, 1, ntsctint);
		SendDlgItemMessage(hwndDlg, 501, TBM_SETPOS, 1, ntschue);
		EnableWindow(GetDlgItem(hwndDlg, 201), (eoptions & EO_CPALETTE) ? 1 : 0);
		break;
	case WM_HSCROLL:
		ntsctint = SendDlgItemMessage(hwndDlg, 500, TBM_GETPOS, 0, (LPARAM)(LPSTR)0);
		ntschue = SendDlgItemMessage(hwndDlg, 501, TBM_GETPOS, 0, (LPARAM)(LPSTR)0);
		FCEUI_SetNTSCTH(ntsccol, ntsctint, ntschue);
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		if (!(wParam >> 16))
			switch (wParam & 0xFFFF) {
			case 100: ntsccol ^= 1; FCEUI_SetNTSCTH(ntsccol, ntsctint, ntschue); break;
			case 200:
				StopSound();
				if (LoadPaletteFile())
					EnableWindow(GetDlgItem(hwndDlg, 201), 1);
				break;
			case 201: FCEUI_SetPaletteArray(0);
				eoptions &= ~EO_CPALETTE;
				EnableWindow(GetDlgItem(hwndDlg, 201), 0);
				break;
			case 1:
 gornk:
				DestroyWindow(hwndDlg);
				pwindow = 0;		// Yay for user race conditions.
				break;
			}
	}
	return 0;
}

static void ConfigPalette(void) {
	if (!pwindow)
		pwindow = CreateDialog(fceu_hInstance, "PALCONFIG", 0, PaletteConCallB);
	else
		SetFocus(pwindow);
}


static BOOL CALLBACK TimingConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int x;

	switch (uMsg) {
	case WM_INITDIALOG:
		if (eoptions & EO_HIGHPRIO)
			CheckDlgButton(hwndDlg, 105, BST_CHECKED);
		if (eoptions & EO_NOTHROTTLE)
			CheckDlgButton(hwndDlg, 101, BST_CHECKED);
		for (x = 0; x < 10; x++) {
			char buf[8];
			sprintf(buf, "%d", x);
			SendDlgItemMessage(hwndDlg, 110, CB_ADDSTRING, 0, (LPARAM)(LPSTR)buf);
			SendDlgItemMessage(hwndDlg, 111, CB_ADDSTRING, 0, (LPARAM)(LPSTR)buf);
		}
		SendDlgItemMessage(hwndDlg, 110, CB_SETCURSEL, maxconbskip, (LPARAM)(LPSTR)0);
		SendDlgItemMessage(hwndDlg, 111, CB_SETCURSEL, ffbskip, (LPARAM)(LPSTR)0);
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		if (!(wParam >> 16))
			switch (wParam & 0xFFFF) {
			case 1:
gornk:
				if (IsDlgButtonChecked(hwndDlg, 105) == BST_CHECKED)
					eoptions |= EO_HIGHPRIO;
				else
					eoptions &= ~EO_HIGHPRIO;

				if (IsDlgButtonChecked(hwndDlg, 101) == BST_CHECKED)
					eoptions |= EO_NOTHROTTLE;
				else
					eoptions &= ~EO_NOTHROTTLE;

				maxconbskip = SendDlgItemMessage(hwndDlg, 110, CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				ffbskip = SendDlgItemMessage(hwndDlg, 111, CB_GETCURSEL, 0, (LPARAM)(LPSTR)0);
				EndDialog(hwndDlg, 0);
				break;
			}
	}
	return 0;
}

void DoTimingConfigFix(void) {
	DoPriority();
}

static void ConfigTiming(void) {
	DialogBox(fceu_hInstance, "TIMINGCONFIG", hAppWnd, TimingConCallB);
	DoTimingConfigFix();
}

static BOOL CALLBACK GUIConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		if (eoptions & EO_FOAFTERSTART)
			CheckDlgButton(hwndDlg, 102, BST_CHECKED);
		if (eoptions & EO_HIDEMENU)
			CheckDlgButton(hwndDlg, 104, BST_CHECKED);
		if (goptions & GOO_CONFIRMEXIT)
			CheckDlgButton(hwndDlg, 110, BST_CHECKED);
		if (goptions & GOO_DISABLESS)
			CheckDlgButton(hwndDlg, 111, BST_CHECKED);
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		if (!(wParam >> 16))
			switch (wParam & 0xFFFF) {
			case 1:
gornk:
				if (IsDlgButtonChecked(hwndDlg, 102) == BST_CHECKED)
					eoptions |= EO_FOAFTERSTART;
				else
					eoptions &= ~EO_FOAFTERSTART;
				if (IsDlgButtonChecked(hwndDlg, 104) == BST_CHECKED)
					eoptions |= EO_HIDEMENU;
				else
					eoptions &= ~EO_HIDEMENU;

				goptions &= ~(GOO_CONFIRMEXIT | GOO_DISABLESS);

				if (IsDlgButtonChecked(hwndDlg, 110) == BST_CHECKED)
					goptions |= GOO_CONFIRMEXIT;
				if (IsDlgButtonChecked(hwndDlg, 111) == BST_CHECKED)
					goptions |= GOO_DISABLESS;
				EndDialog(hwndDlg, 0);
				break;
			}
	}
	return 0;
}

static void ConfigGUI(void) {
	DialogBox(fceu_hInstance, "GUICONFIG", hAppWnd, GUIConCallB);
}


static int BrowseForFolder(HWND hParent, char *htext, char *buf) {
	BROWSEINFO bi;
	LPCITEMIDLIST pidl;
	int ret = 1;

	buf[0] = 0;

	memset(&bi, 0, sizeof(bi));

	bi.hwndOwner = hParent;
	bi.lpszTitle = htext;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	if (FAILED(CoInitialize(0)))
		return(0);

	if (!(pidl = SHBrowseForFolder(&bi))) {
		ret = 0;
		goto end1;
	}

	if (!SHGetPathFromIDList(pidl, buf)) {
		ret = 0;
		goto end2;
	}

end2:
	/* This probably isn't the best way to free the memory... */
	CoTaskMemFree((PVOID)pidl);

end1:
	CoUninitialize();
	return(ret);
}

static BOOL CALLBACK DirConCallB(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int x;

	switch (uMsg) {
	case WM_INITDIALOG:
		for (x = 0; x < 6; x++)
			SetDlgItemText(hwndDlg, 100 + x, DOvers[x]);
		if (eoptions & EO_SNAPNAME)
			CheckDlgButton(hwndDlg, 300, BST_CHECKED);
		break;
	case WM_CLOSE:
	case WM_QUIT: goto gornk;
	case WM_COMMAND:
		if (!(wParam >> 16)) {
			if ((wParam & 0xFFFF) >= 200 && (wParam & 0xFFFF) <= 205) {
				static char *helpert[6] = { "Cheats", "Miscellaneous", "Nonvolatile Game Data", "Save States", "Screen Snapshots", "Base Directory" };
				char name[MAX_PATH];

				if (BrowseForFolder(hwndDlg, helpert[((wParam & 0xFFFF) - 200)], name))
					SetDlgItemText(hwndDlg, 100 + ((wParam & 0xFFFF) - 200), name);
			} else switch (wParam & 0xFFFF) {
				case 1:
gornk:

					if (IsDlgButtonChecked(hwndDlg, 300) == BST_CHECKED)
						eoptions |= EO_SNAPNAME;
					else
						eoptions &= ~EO_SNAPNAME;

					RemoveDirs();		// Remove empty directories.

					for (x = 0; x < 6; x++) {
						LONG len;
						len = SendDlgItemMessage(hwndDlg, 100 + x, WM_GETTEXTLENGTH, 0, 0);
						if (len <= 0) {
							if (DOvers[x]) free(DOvers[x]);
							DOvers[x] = 0;
							continue;
						}
						len++;			// Add 1 for null character.
						if (!(DOvers[x] = malloc(len)))
							continue;
						if (!GetDlgItemText(hwndDlg, 100 + x, DOvers[x], len)) {
							free(DOvers[x]);
							DOvers[x] = 0;
							continue;
						}
					}

					CreateDirs();		// Create needed directories.
					SetDirs();			// Set the directories in the core.
					EndDialog(hwndDlg, 0);
					break;
				}
		}
	}
	return 0;
}



static void ConfigDirectories(void) {
	DialogBox(fceu_hInstance, "DIRCONFIG", hAppWnd, DirConCallB);
}

