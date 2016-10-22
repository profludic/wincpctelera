#include "winCpctelera.h"

#ifdef _USEWINGDI

#pragma comment(lib,"winmm.lib")

extern COLORREF GetColorHW(int pHW);
extern int GetPixelBit();
extern int GetScreenWidth();
extern UCHAR* GetRenderingBuffer();
extern u8 GetCpcKeyPos(u16 pVKeyID);

static HPALETTE _hPal;
static HWND _hWnd;
static DWORD _lastTime;

static UCHAR iconDataFile[] = { 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0xE8, 0x02, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x0A, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBB, 0x00, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xBB, 0xBF, 0xBB, 0x0A, 0xAA, 0xAA, 0xAA, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xFB, 0xBF, 0xB0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBB, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBF, 0xFB, 0xBB, 0xF0, 0x08, 0xF8, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xFB, 0xBF, 0xBF, 0xBB, 0xF0, 0xF0, 0x80, 0xF0, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xBB, 0xBF, 0xBB, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xBB, 0xBF, 0xBB, 0xF0, 0x0F, 0xFF, 0xF0, 0xF0, 0x80, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x08, 0xF8, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x80, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xFC, 0x0F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0x3F, 0xFF, 0xFC, 0x0E, 0x1F, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xF0, 0x08, 0x07, 0xFF, 0xE0, 0x00, 0x03, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x7F, 0xC0, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x03, 0xF8, 0x06, 0x00, 0x07, 0xFC, 0x0F, 0x00, 0x0F, 0xFF, 0xFF, 0x80, 0x1F, 0xFF, 0xFF, 0xC0, 0x3F, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void Close()
{
	if (_hPal != NULL)
		DeleteObject(_hPal);
}

void FillBorder(HDC pHdc)
{
	int hw = _amstrad._curPal[BORDER_COLOR];
	HBRUSH brush = CreateSolidBrush(GetColorHW(hw));

	HDC hdc;
	if (pHdc == NULL)
		hdc = GetDC(_hWnd);
	else
		hdc = pHdc;

	for (int i = 0; i < 4; i++)
	{
		RECT top = { 0, 0, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY };
		FillRect(hdc, &top, brush);

		RECT left = { 0, BORDER_UP_CY, BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
		FillRect(hdc, &left, brush);

		RECT bottom = { 0, BORDER_UP_CY + HEIGHT_SCREEN, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY };
		FillRect(hdc, &bottom, brush);

		RECT right = { BORDER_CX + WIDTH_SCREEN, BORDER_UP_CY, BORDER_CX + WIDTH_SCREEN + BORDER_CX, BORDER_UP_CY + HEIGHT_SCREEN };
		FillRect(hdc, &right, brush);
	}

	DeleteObject(brush);

	if (pHdc == NULL)
		ReleaseDC(_hWnd, hdc);
}

void CreatePaletteCpc()
{
	NPLOGPALETTE logPalette = (NPLOGPALETTE)malloc(sizeof(LOGPALETTE) + NB_COLORS * sizeof(PALETTEENTRY));

	logPalette->palNumEntries = NB_COLORS;
	logPalette->palVersion = 0x300;

	for (int i = 0; i < NB_COLORS; i++)
	{
		int hw = _amstrad._curPal[i];
		COLORREF rgb = GetColorHW(hw);

		logPalette->palPalEntry[i].peBlue = GetBValue(rgb);
		logPalette->palPalEntry[i].peRed = GetRValue(rgb);
		logPalette->palPalEntry[i].peGreen = GetGValue(rgb);
		logPalette->palPalEntry[i].peFlags = 0;
	}

	if (_hPal != NULL)
		DeleteObject(_hPal);

	_hPal = CreatePalette(logPalette);

	free(logPalette);

	FillBorder(NULL);
}

LPBITMAPINFO CreateBitmapInfo(int pBitCount, int cx, int cy)
{
	int nbColor = 1;
	for (int i = 0; i < pBitCount; i++)
		nbColor *= 2;

	int sizeBitmapInfo = sizeof(BITMAPINFOHEADER) + nbColor * sizeof(WORD);
	LPBITMAPINFO bitmapInfos = (LPBITMAPINFO)malloc(sizeBitmapInfo);
	memset(bitmapInfos, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfos->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfos->bmiHeader.biWidth = cx;
	bitmapInfos->bmiHeader.biHeight = -cy;
	bitmapInfos->bmiHeader.biPlanes = 1;
	bitmapInfos->bmiHeader.biBitCount = pBitCount;
	bitmapInfos->bmiHeader.biCompression = BI_RGB;
	bitmapInfos->bmiHeader.biClrUsed = nbColor;

	WORD* pal = (WORD*)bitmapInfos->bmiColors;
	for (int i = 0; i < nbColor; i++)
		pal[i] = i;

	return bitmapInfos;
}

void Redraw(HWND pWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(pWnd, &ps);

	FillBorder(hdc);

	SelectPalette(hdc, _hPal, FALSE);
	RealizePalette(hdc);

	LPBITMAPINFO bitmapInfos = CreateBitmapInfo(GetPixelBit(), GetScreenWidth(), CPC_SCR_CY_LINE);
	int r = StretchDIBits(hdc, BORDER_CX, BORDER_UP_CY, WIDTH_SCREEN, HEIGHT_SCREEN, 0, 0, GetScreenWidth(), CPC_SCR_CY_LINE, GetRenderingBuffer(), bitmapInfos, DIB_PAL_COLORS, SRCCOPY);

	/*LPBITMAPINFO bitmapInfos = CreateBitmapInfo(GetPixelBit(), GetScreenWidth(), CPC_INTERRUPT_LINE);

	int interruptLine = 0;
	while (interruptLine < CPC_SCR_CY_LINE)
	{
		StretchDIBits(hdc,	BORDER_CX, BORDER_UP_CY + interruptLine * 2, WIDTH_SCREEN, CPC_INTERRUPT_LINE * 2,
							0, 0, GetScreenWidth(), CPC_INTERRUPT_LINE, GetRenderingBuffer() + interruptLine * CPC_SCR_CX_BYTES, bitmapInfos, DIB_PAL_COLORS, SRCCOPY);
		interruptLine += CPC_INTERRUPT_LINE;
	}*/

	free(bitmapInfos);

	EndPaint(pWnd, &ps);
}

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			_curKey = TRUE;
			int pos = GetCpcKeyPos(wParam);
			cpct_keyboardStatusBuffer[pos / 8] = 0xFF ^ (1 << (pos % 8));
			break;

		case WM_PAINT:
			Redraw(hWnd);
			break;

		case WM_DESTROY:
			Close();
			exit(0);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HICON createIcon(PBYTE iconData, int iconSize)
{
	HICON hIcon = NULL;
	int offset = LookupIconIdFromDirectoryEx(iconData, TRUE, iconSize, iconSize, LR_DEFAULTCOLOR);

	if (offset != 0)
		hIcon = CreateIconFromResourceEx(iconData + offset, 0, TRUE, 0x30000, iconSize, iconSize, LR_DEFAULTCOLOR);

	return hIcon;
}

RECT CalculateWindowRect(HWND hWindow, SIZE szDesiredClient)
{
	RECT rcDesiredWindowRect;
	RECT rcCurrentWindowRect;
	RECT rcCurrentClientRect;

	GetWindowRect(hWindow, &rcCurrentWindowRect);
	GetClientRect(hWindow, &rcCurrentClientRect);

	/** Get the difference between the current and desired client areas */
	SIZE szClientDifference = { rcCurrentClientRect.right - szDesiredClient.cx, rcCurrentClientRect.bottom - szDesiredClient.cy };

	/** Get the difference between the current window rect and the desired window rect */
	SetRect(&rcDesiredWindowRect, rcCurrentWindowRect.left, rcCurrentWindowRect.top, rcCurrentWindowRect.right - szClientDifference.cx, rcCurrentWindowRect.bottom - szClientDifference.cy);
	return rcDesiredWindowRect;
}

void PosWindow()
{
	SetWindowLong(_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
	MoveWindow(_hWnd, 0, 0, 0, 0, FALSE);

	int posX = (GetSystemMetrics(SM_CXFULLSCREEN) - FULL_SCREEN_CX) / 2;
	int posY = (GetSystemMetrics(SM_CYFULLSCREEN) - FULL_SCREEN_CY) / 2;

	SIZE szDesiredClient = { FULL_SCREEN_CX, FULL_SCREEN_CY };

	RECT rcNewWindowRect = CalculateWindowRect(_hWnd, szDesiredClient);

	SIZE size = { rcNewWindowRect.right - rcNewWindowRect.left,
		rcNewWindowRect.bottom - rcNewWindowRect.top };

	MoveWindow(_hWnd, posX, posY, size.cx, size.cy, TRUE);
}

void CreateWindowApp()
{
	#define TITLE	"WinCPCTelera"	

	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = createIcon(iconDataFile, 32);
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = TITLE;
	wc.lpszClassName = TITLE;

	RegisterClass(&wc);

	_hWnd = CreateWindow(TITLE,
		NULL,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		0, 0,
		0, 0,
		NULL,
		NULL,
		instance,
		NULL);

	SetWindowText(_hWnd, TITLE);
	PosWindow();

	_hPal = NULL;
	CreatePaletteCpc();
	MsgLoop();
}

void MsgLoop()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			return;
	}
}

void Refresh()
{
	InvalidateRect(_hWnd, NULL, FALSE);
}

VOID CALLBACK InternalTimer(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
	if (_amstrad._internalTimer++ == 6)
		_amstrad._internalTimer = 0;
	
	if (_amstrad._interruptFunction != NULL)
		_amstrad._interruptFunction();

	Refresh();
}

void StartInterrupt()
{
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	int timerRes = tc.wPeriodMin > 1 ? tc.wPeriodMin : 1;
	timeBeginPeriod(timerRes);

	SetTimer(_hWnd, WM_USER + 464, INTERRUPT_MS, InternalTimer);
}

#endif