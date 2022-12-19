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
int iCount; //���� ���� ����
wchar_t pstr[128];
int wCount;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	static wchar_t str[128]; //TCHAR �빮��, str�� �迭�� �̸� -> �� �迭�� �����ּ�, static���ų� �������ϸ� ����wcslen(str).

	//Message Driven Architecture ---> Event Driven Architecture (Ű����, ���콺�� ������ �����Ǵ�)

	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_CHAR: //Character. Ű���� ���ڿ� �Է�
		hdc = GetDC(hWnd);
		str[wCount] = wParam;
		TextOut(hdc, 10, 10, str, wcslen(str)); //�������� ȯ�濡�� ��������Ҷ� ���� TextOut(hdc,x,y,���ڽ����ּ�,����� ���ڿ��� ����), wcslen()�� c����� strlen()
		pstr[wCount] = str[wCount];
		wCount++;
		return 0;
	case WM_LBUTTONDOWN: //���콺 ��Ŭ��
		hdc = GetDC(hWnd);
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		px[iCount] = x;
		py[iCount] = y;
		iCount++;
		//Rectangle(hdc, 50, 50, 100, 100);
		Ellipse(hdc, x - 50, y - 50, x + 50, y + 50); //�� ������ܲ�����,�����ϴܲ�����
		return 0;
	case WM_PAINT: //ȭ���籸��(�ٽñ׸���)�� ���α׷��ӿ���...å��... os�� ������ ���� ����... �ٽñ׷��� �� ������ �˷��ش�. //���α׷�â�� �ٽ� �ø� ��
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
