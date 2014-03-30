#include <stdlib.h>
#include "common.h"

#define MAXLOGTEXT 1024

static HWND logwin = 0;

static char *logtext[MAXLOGTEXT];
static int logcount = 0;

static void RedoText(void) {
	char textbuf[65535];
	int x;

	textbuf[0] = 0;
	if (logcount >= MAXLOGTEXT) {
		x = logcount & (MAXLOGTEXT - 1);
		for (;; ) {
			strcat(textbuf, logtext[x]);
			x = (x + 1) & (MAXLOGTEXT - 1);
			if (x == (logcount & (MAXLOGTEXT - 1))) break;
		}
	} else
		for (x = 0; x < logcount; x++) {
			strcat(textbuf, logtext[x]);
		}
	SetDlgItemText(logwin, 100, textbuf);
	SendDlgItemMessage(logwin, 100, EM_LINESCROLL, 0, 1024 * 4);
}

static BOOL CALLBACK LogCon(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DSMFix(uMsg);
	switch (uMsg) {
	case WM_INITDIALOG: logwin = hwndDlg;
		RedoText(); break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			DestroyWindow(hwndDlg);
			logwin = 0;
		}
		break;
	}
	return 0;
}

void MakeLogWindow(void) {
	if (!logwin)
		CreateDialog(fceu_hInstance, "MESSAGELOG", 0, LogCon);
}

void AddLogText(char *text, int newline) {
	int x;
	char *t;

	if (logcount >= MAXLOGTEXT) free(logtext[logcount & (MAXLOGTEXT - 1)]);

	x = 0;
	t = text;
	while (*t) {
		if (*t == '\n') x++;
		t++;
	}

	if (!(logtext[logcount & (MAXLOGTEXT - 1)] = malloc(strlen(text) + 1 + x + newline * 2)))
		return;

	t = logtext[logcount & (MAXLOGTEXT - 1)];

	while (*text) {
		if (*text == '\n') {
			*t = '\r';
			t++;
		}
		*t = *text;
		t++;
		text++;
	}
	if (newline) {
		*t = '\r';
		t++;
		*t = '\n';
		t++;
	}
	*t = 0;
	logcount++;
	if (logwin)
		RedoText();
}

void FCEUD_Message(char *text) {
	AddLogText(text, 0);
}
