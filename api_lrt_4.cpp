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
//F9 : break point �ߴ���
//F5 Go
//F10 : ���پ� ����
//Ctrl + F10 Ŀ������ ����


//int px[1000], py[1000], iShape; //0��������, 1�� ������ //��������
//int iCount; //���� ���� ���� //�������̼� ���������� ������ �����Ǵ°� ������ ����ƽ //���ú����� �Լ� �����ø� ��ȿ 
int wCount;

typedef struct tagmyPOINT
{
	LONG x;
	LONG y;
	int iShape;
} myPOINT;
myPOINT pt[1000];
int iCount;

//�������� px py icount�� �ʱ�ȭ�� �����൵ ���� 0�� ���ִ�. mainȣ��Ǳ� ������ ���� �����ϰ� main�� ȣ��ȴ�. ��ŸƮ�ڵ尡 ������ü�� �����ϰų� ���������� �ʱ�ȭ�Ѵ�.
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	//int px[1000], py[1000];
	//int iCount = 0;
	static TCHAR str[256];
	int len;
	HPEN hpen;// , oldPen; //h�� handle�� �ǹ�
	HBRUSH hBrush;// , oldBrush;
	//static wchar_t str[128]; //TCHAR �빮��, str�� �迭�� �̸� -> �� �迭�� �����ּ�, static���ų� �������ϸ� ����wcslen(str).
	//Message Driven Architecture ---> Event Driven Architecture (Ű����, ���콺�� ������ �����Ǵ�)
	//�ڵ鿡 ���� ����
	//wchar_t, char
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;//������ �Լ��� ����
	case WM_CHAR: //Character. Ű���� ���ڿ� �Է�
		//hdc = GetDC(hWnd);
		//str[wCount] = wParam;
		//TextOut(hdc, 10, 10, str, wcslen(str)); //�������� ȯ�濡�� ��������Ҷ� ���� TextOut(hdc,x,y,���ڽ����ּ�,����� ���ڿ��� ����), wcslen()�� c����� strlen()
		//wCount++;
		len = lstrlen(str); //���������Լ�-���ڱ��� ����--> wcslen() strlen()
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_LBUTTONDOWN: //���콺 ��Ŭ��   
		//Device Context : �׸��⵵����(��, �귯��, ��ȭ��)
		//��� �׸��� �Լ��� DC�� �䱸		
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
		//Ellipse(hdc, x - 50, y - 50, x + 50, y + 50); //�� ������ܲ�����,�����ϴܲ�����
		//ReleaseDC(hWnd, hdc); //�ݳ� ���ص� ���ư����� ������
		InvalidateRect(hWnd, NULL, TRUE);//os �� �ٽ� �׷��� �ڴ�. WM_PAINT �޽��� �߻������ֶ�
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
	case WM_PAINT: //ȭ���籸��(�ٽñ׸���)�� ���α׷��ӿ���...å��... os�� ������ ���� ����... �ٽñ׷��� �� ������ �˷��ش�. //���α׷�â�� �ٽ� �ø� ��
		hdc = BeginPaint(hWnd, &ps); //paint�Լ������� beginpaint endpaint�� �ݵ�� ����Ѵ�. �������� getdc release dc��

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
