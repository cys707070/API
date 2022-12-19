#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int px[1000], py[1000];
int iCount; //점의 갯수 저장
wchar_t pstr[128];
int wCount;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	static wchar_t str[128]; //TCHAR 대문자, str은 배열의 이름 -> 그 배열의 시작주소, static쓰거나 전역안하면 깨짐wcslen(str).

	//Message Driven Architecture ---> Event Driven Architecture (키보드, 마우스에 반항해 구동되는)

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_CHAR: //Character. 키보드 문자열 입력
		hdc = GetDC(hWnd);
		str[wCount] = wParam;
		TextOut(hdc, 10, 10, str, wcslen(str)); //윈도우즈 환경에서 문자출력할때 쓰는 TextOut(hdc,x,y,문자시작주소,출력할 문자열의 갯수), wcslen()은 c언어의 strlen()
		pstr[wCount] = str[wCount];
		wCount++;
		return 0;
	case WM_LBUTTONDOWN: //마우스 좌클릭
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		px[iCount] = x;
		py[iCount] = y;
		iCount++;
		//Rectangle(hdc, 50, 50, 100, 100);
		Ellipse(hdc, x - 50, y - 50, x + 50, y + 50); //원 좌측상단꼭짓점,우측하단꼭짓점
		return 0;
	case WM_PAINT: //화면재구성(다시그리기)은 프로그래머에게...책임... os는 놀고먹을 수는 없다... 다시그려야 할 시점을 알려준다. //프로그램창을 다시 올릴 때
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iCount; i++)
			Ellipse(hdc, px[i] - 50, py[i] - 50, px[i] + 50, py[i] + 50);
		for (int i = 0; i < wCount; i++)
			TextOut(hdc, 10, 10, pstr, wcslen(pstr));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
