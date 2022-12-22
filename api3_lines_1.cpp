#include <windows.h>

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
	WndClass.lpszMenuName=NULL;
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


typedef struct tagline
{
	POINT pt[1000];
	int iCount;  //한개 선내에서의 점개수....
} line;

line lines[500];
int iLineCount;
int iTempCount;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
	//	bNowDraw=TRUE;
		lines[iLineCount].pt[iTempCount].x = x;
		lines[iLineCount].pt[iTempCount].y = y;
		iTempCount++; //그리는 현재의 선내에서의 점개수를 추적하기 위해 사용

		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {  //왼쪽버튼을 누른 상태에서 움직이면
			hdc=GetDC(hWnd);
			MoveToEx(hdc,x,y,NULL);
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			LineTo(hdc,x,y);
			ReleaseDC(hWnd,hdc);


			lines[iLineCount].pt[iTempCount].x = x;
			lines[iLineCount].pt[iTempCount].y = y;
			iTempCount++;
		}
		return 0;
	case WM_LBUTTONUP:
	//	bNowDraw=FALSE;
		lines[iLineCount].iCount = iTempCount;
		iLineCount++;
		iTempCount = 0;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLineCount; i++){
			MoveToEx(hdc, lines[i].pt[0].x, lines[i].pt[0].y, NULL);
			for (int j = 1; j < lines[i].iCount; j++){
				LineTo(hdc, lines[i].pt[j].x, lines[i].pt[j].y);
			}
		}

		EndPaint(hWnd, &ps);

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
