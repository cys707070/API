#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass=TEXT("Mouse");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName= MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

typedef struct _line {
	POINT p[500];
	int iCount;
	COLORREF PenColor;
	int PenThick;
} line;

line lines[1000];

int iTempCount;
int iLines;
COLORREF iCurrentPenColor = RGB(255, 0, 0); //default : »¡°­.
int iCurrentPenThick = 2;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	HPEN hpen;
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_RED:
			iCurrentPenColor = RGB(255, 0, 0);
			break;
		case ID_GREEN:
			iCurrentPenColor = RGB(0, 255, 0);
			break;
		case ID_BLUE:
			iCurrentPenColor = RGB(0, 0, 255);
			break;
		
		case ID_thick2:
			iCurrentPenThick = 2;
			break;
		case ID_thick4:
			iCurrentPenThick = 4;
			break;
		case ID_thick6:
			iCurrentPenThick = 6;
			break;
		case ID_thick8:
			iCurrentPenThick = 8;
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		lines[iLines].p[iTempCount].x = x;
		lines[iLines].p[iTempCount].y = y;

		lines[iLines].PenColor = iCurrentPenColor;
		lines[iLines].PenThick = iCurrentPenThick;
		iTempCount++;
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			hdc=GetDC(hWnd);
			MoveToEx(hdc,x,y,NULL);
			hpen = CreatePen(PS_SOLID, lines[iLines].PenThick, lines[iLines].PenColor);
			SelectObject(hdc, hpen);
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			lines[iLines].p[iTempCount].x = x;
			lines[iLines].p[iTempCount].y = y;
			iTempCount++;
			LineTo(hdc,x,y);
			ReleaseDC(hWnd,hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		lines[iLines].iCount = iTempCount;
		iTempCount = 0;
		iLines++;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLines; i++)
		{			
			for (int j = 1; j < lines[i].iCount; j++)
			{
				hpen = CreatePen(PS_SOLID, lines[i].PenThick, lines[i].PenColor);
				hpen = (HPEN)SelectObject(hdc, hpen);
				MoveToEx(hdc, lines[i].p[j-1].x, 
					lines[i].p[j-1].y, NULL);
				LineTo(hdc, lines[i].p[j].x,
					lines[i].p[j].y);
				DeleteObject(SelectObject(hdc, hpen));
			}	
		}
		
		
		return 0;

	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
