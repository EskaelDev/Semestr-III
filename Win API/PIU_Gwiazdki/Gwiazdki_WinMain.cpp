#include <Windows.h>
#include <time.h>
#include <deque>
#include <tchar.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500


using namespace std;

MSG msg;
TCHAR className[] = TEXT("Gwiazdki");
HWND hwnd;
RECT clientRect;



bool checkRectCollision(RECT rect_1, RECT rect_2)
{
	if (rect_1.left		<	rect_2.right	&&
		rect_1.right	>	rect_2.left		&&
		rect_1.top		<	rect_2.bottom	&&
		rect_1.bottom	>	rect_2.top)
		return true;

	return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	switch (msg)
	{
	case WM_CREATE:
		
		break;
	case WM_PAINT:
		
		break;
	case WM_TIMER:
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = className;
	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Nie uda³o siê zarejestrowaæ klasy okna!"), TEXT("Error 404"), MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(0, className, TEXT("Snake"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("Nie uda³o siê utworzyæ okna !"), TEXT("CreateWindowEx"), MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(className, hInstance);

	return msg.wParam;
}