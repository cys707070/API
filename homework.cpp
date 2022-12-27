#include <windows.h>
#include <tchar.h>
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

int iCurrentPenWidth = 3;
COLORREF iCurrentPenColor = RGB(255, 0, 0);

TCHAR *Items[] = { TEXT("3"), TEXT("6"), TEXT("9"), TEXT("12") };
HWND hCombo;

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR str[128];
	switch (iMessage) {
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		SetDlgItemText(hDlg, IDC_COMBO1, str);
		for (int i = 0; i<4; i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg, IDC_COMBO1, str, 128);
			iCurrentPenWidth = _ttoi(str);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;
	HPEN hPen;
	//TCHAR str[128];
	switch (iMessage) {
	case WM_CREATE:
		/*		
		hCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE |
			CBS_DROPDOWN, 130, 10, 100, 200, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);
		for (int i = 0; i<4; i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)Items[i]);
		}
		SendMessage(hCombo, CB_SETCURSEL, (iCurrentPenWidth/3)-1, 0);*/
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_DIALOG:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			break;
		
		//case ID_COMBOBOX:
		//	switch (HIWORD(wParam)) {
		//	case CBN_SELCHANGE:
		//		iCurrentPenWidth = 
		//			(SendMessage(hCombo, CB_GETCURSEL, 0, 0) + 1) * 3;
		//		SetFocus(hWnd);
		//		break;
		//	case CBN_EDITCHANGE:
		//		GetWindowText(hCombo, str, 128);
		//		iCurrentPenWidth = _ttoi(str);
		//		//SetFocus(hWnd);  // 두글자 이상 입력이 안됨 ==> WM_LBUTTONDOWN 한곳으로
		//		break;
		//	}
			
		}
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_BACK:
			if (iLines > 0) 
			{
				iLines--; lines[iLines].iCount = 0;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;

	case WM_LBUTTONDOWN:
		SetFocus(hWnd);   // 꼼수 : 여기 한곳에 만 SetFocus() 추가하면 됨
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		
		 
		lines[iLines].pColor =iCurrentPenColor ;
		lines[iLines].pWidth =iCurrentPenWidth;

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
		//iTempCount = 0;
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
}//한자, 순서 고쳐보기
