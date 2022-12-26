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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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
	COLORREF pColor;
	
	int pWidth;
} line;

line lines[1000];

//int iTempCount;
int iLines;

COLORREF iCurrentPenColor = RGB(255, 0, 0);
int iCurrentPenWidth = 2;
TCHAR *Items[] = { TEXT("2"), TEXT("4"), TEXT("6"), TEXT("8") };//배열한개만

#define ID_LISTBOX 100
HWND hList;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;
	HPEN hPen;

	switch (iMessage) {
	case WM_CREATE:
		hList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			LBS_NOTIFY, 10, 10, 100, 200, hWnd, (HMENU)ID_LISTBOX, g_hInst, NULL);
		for (int i = 0; i<4; i++) {
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		SendMessage(hList, LB_SETCURSEL, (iCurrentPenWidth - 1) / 2, 0); //교수님부분 추가배열 안쓰고 수식으로
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_RED:
			iCurrentPenColor = RGB(255, 0, 0);
			break;
		case IDM_GREEN:
			iCurrentPenColor = RGB(0,255, 0, 0);			
			break;
		case IDM_BLUE:
			iCurrentPenColor = RGB(0,0, 255);
			break;


		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				iCurrentPenWidth = 1 + 
					(2 * SendMessage(hList, LB_GETCURSEL, 0, 0)); //0 1 2 3 추가배열 안쓰고 수식으로
				SetFocus(hWnd);
				break;
			}
		}
		return 0;

	case WM_KEYDOWN: // 제일 최근에 한거 지우기 , 라인 굵은 거 안지워지는이유
		switch (wParam) {
		case VK_BACK:
			if (iLines > 0) 
			{
				iLines--; 
				//lines[iLines].iCount = 0;//이 라인의 의미????
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;

	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);


		lines[iLines].iCount = 0;//이 라인의 의미????

		 
		lines[iLines].pColor =iCurrentPenColor ;
		lines[iLines].pWidth = iCurrentPenWidth;

		lines[iLines].p[lines[iLines].iCount].x = x;
		lines[iLines].p[lines[iLines].iCount].y = y;
		lines[iLines].iCount++;  ///
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {
			hdc=GetDC(hWnd);
			hPen = CreatePen(PS_SOLID, iCurrentPenWidth, iCurrentPenColor);
			
			hPen=(HPEN)SelectObject(hdc, hPen);
			
			MoveToEx(hdc,x,y,NULL);
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			lines[iLines].p[lines[iLines].iCount].x = x;
			lines[iLines].p[lines[iLines].iCount].y = y;
			lines[iLines].iCount++;
			LineTo(hdc,x,y);

			DeleteObject(SelectObject(hdc, hPen));

			ReleaseDC(hWnd,hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		//lines[iLines].iCount = iTempCount;
		//iTempCount = 0; 임시변수 안쓰고 선마나 iCount쓰는중!!!
		iLines++;
		return 0;
		// 내가 생성한 GDI Object는 사용후반드시 삭제한다, 단 선택해제후
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLines; i++)
		{
			hPen = CreatePen(PS_SOLID, lines[i].pWidth, lines[i].pColor);
			hPen = (HPEN)SelectObject(hdc, hPen);  //선택한다.
			MoveToEx(hdc, lines[i].p[0].x, 
					lines[i].p[0].y, NULL);
			for (int j = 0; j < lines[i].iCount-1; j++)
			{
				
				LineTo(hdc, lines[i].p[j+1].x,
					lines[i].p[j+1].y);
			}
			 //선택해제...
			DeleteObject(SelectObject(hdc, hPen));  //그리고 삭제한다...
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
//tempcount없앤이유, 라인즈의 의미, 셋포커스