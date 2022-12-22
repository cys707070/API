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
//Debugging
//F9 : break point 중단점
//F5 Go
//F10 : 한줄씩 실행
//Ctrl + F10 커버까지 시행


//int px[1000], py[1000], iShape; //0은엘립스, 1은 렉탕글 //전역변수
//int iCount; //점의 개수 저장 //애플케이션 끝날때까지 공간이 유지되는게 전역과 스태틱 //로컬변수는 함수 내에시만 유효 
int wCount;

typedef struct tagmyPOINT
{
	LONG x;
	LONG y;
	int iShape;
} myPOINT;
myPOINT pt[1000];
int iCount;

//전역변수 px py icount는 초기화를 안해줘도 전부 0이 들어가있다. main호출되기 이전에 값을 설정하고 main이 호출된다. 스타트코드가 전역객체들 생성하거나 전역변수를 초기화한다.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	//int px[1000], py[1000];
	//int iCount = 0;
	static TCHAR str[256];
	int len;
	HPEN hpen;// , oldPen; //h는 handle을 의미
	HBRUSH hBrush;// , oldBrush;
	//static wchar_t str[128]; //TCHAR 대문자, str은 배열의 이름 -> 그 배열의 시작주소, static쓰거나 전역안하면 깨짐wcslen(str).
	//Message Driven Architecture ---> Event Driven Architecture (키보드, 마우스에 반항해 구동되는)
	//핸들에 대한 이해
	//wchar_t, char
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;//리턴은 함수의 종료
	case WM_CHAR: //Character. 키보드 문자열 입력
		//hdc = GetDC(hWnd);
		//str[wCount] = wParam;
		//TextOut(hdc, 10, 10, str, wcslen(str)); //윈도우즈 환경에서 문자출력할때 쓰는 TextOut(hdc,x,y,문자시작주소,출력할 문자열의 갯수), wcslen()은 c언어의 strlen()
		//wCount++;
		len = lstrlen(str); //윈도우즈함수-글자길이 리턴--> wcslen() strlen()
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_LBUTTONDOWN: //마우스 좌클릭   
		//Device Context : 그리기도구통(팬, 브러쉬, 도화지)
		//모든 그리기 함수는 DC를 요구		
		//hdc = GetDC(hWnd);
		//hpen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
		//SelectObject(hdc, hpen);
		//hBrush = CreateSolidBrush(RGB(105, 205, 125));
		//SelectObject(hdc, hBrush);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		pt[iCount].x = x;
		pt[iCount].y = y;
		pt[iCount].iShape = 0;
		iCount++;
		//Ellipse(hdc, x - 50, y - 50, x + 50, y + 50); //원 좌측상단꼭짓점,우측하단꼭짓점
		//ReleaseDC(hWnd, hdc); //반납 안해도 돌아가지만 뒷정리
		InvalidateRect(hWnd, NULL, TRUE);//os 야 다시 그려야 겠다. WM_PAINT 메시지 발생시켜주라
		return 0;
	case WM_RBUTTONDOWN:
		//hdc = GetDC(hWnd);
		//hpen = CreatePen(PS_SOLID, 3, RGB(100, 155, 55));
		//SelectObject(hdc, hpen);
		//hBrush = CreateSolidBrush(RGB(255, 50, 30));
		//SelectObject(hdc, hBrush);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		pt[iCount].x = x;
		pt[iCount].y = y;
		pt[iCount].iShape = 1;
		iCount++;
		//Rectangle(hdc, x - 50, y - 50, x + 50, y + 50);
		//ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT: //화면재구성(다시그리기)은 프로그래머에게...책임... os는 놀고먹을 수는 없다... 다시그려야 할 시점을 알려준다. //프로그램창을 다시 올릴 때
		hdc = BeginPaint(hWnd, &ps); //paint함수에서는 beginpaint endpaint를 반드시 써야한다. 나머지는 getdc release dc로

		for (int i = 0; i < iCount; i++){
			if (pt[i].iShape == 0){
				hpen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));
				hBrush = CreateSolidBrush(RGB(105, 205, 125));
				SelectObject(hdc, hBrush);
				//oldPen = (HPEN)SelectObject(hdc, hpen);
				hpen = (HPEN)SelectObject(hdc, hpen);
				//SelectObject(hdc, hpen);
				//oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				hBrush = (HBRUSH)SelectObject(hdc, hBrush);
				//SelectObject(hdc, hBrush);
				Ellipse(hdc, pt[i].x - 50, pt[i].y - 50, pt[i].x + 50, pt[i].y + 50);		
				DeleteObject(SelectObject(hdc, hpen));

				DeleteObject(SelectObject(hdc, hBrush));
			}
			else if (pt[i].iShape == 1){
				hpen = CreatePen(PS_SOLID, 3, RGB(100, 155, 55));
				SelectObject(hdc, hpen);
				hBrush = CreateSolidBrush(RGB(255, 50, 30));
				SelectObject(hdc, hBrush);
				//oldPen = (HPEN)SelectObject(hdc, hpen);
				hpen = (HPEN)SelectObject(hdc, hpen);
				//SelectObject(hdc, hpen);
				//oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				hBrush = (HBRUSH)SelectObject(hdc, hBrush);
				//SelectObject(hdc, hBrush);
				Rectangle(hdc, pt[i].x - 50, pt[i].y - 50, pt[i].x + 50, pt[i].y + 50);
				
				DeleteObject(SelectObject(hdc, hpen));
				
				DeleteObject(SelectObject(hdc, hBrush));
			}
		}
		/*for (int i = 0; i < wCount; i++)
			TextOut(hdc, 10, 10, str, wcslen(str));*/
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
