#include <Windows.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define COLOURS 4
#define RECTANGLE_SIZE 20

TCHAR className[] = TEXT("Nazwa_klasy");
TCHAR appName[] = TEXT("Nazwa_okna");

unsigned int red = 0, green = 0, blue = 0;
unsigned int pixelSize = 10;
bool paintingLeft = false;
bool paintingRight = false;

RECT rect[COLOURS];

void initColours() {
	int squareSize = 50;
	int y = 10;

	for (int i = 0; i < COLOURS; i++) {
		int x = ((10 + squareSize) * i) + 5;

		rect[i].left = x;
		rect[i].top = y;
		rect[i].right = x + squareSize;
		rect[i].bottom = y + squareSize;
	}
}

bool checkCollision(LPARAM lParam) {
	int mouseX = LOWORD(lParam);
	int mouseY = HIWORD(lParam);

	for (int i = 0; i < COLOURS; i++) {
		if (mouseX > rect[i].left && mouseX < rect[i].right && mouseY < rect[i].bottom && mouseY > rect[i].top) {
			switch (i) {
			case 0:
				red = 255; green = blue = 0;
				break;

			case 1:
				red = blue = 0; green = 255;
				break;

			case 2:
				red = green = 0; blue = 255;
				break;

			case 3:
				red = green = blue = 0;
				break;
			}

			return false;
		}
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wC = { 0 };
	HWND hwnd = NULL;
	MSG msg;

	wC.cbSize = sizeof(WNDCLASSEX);
	wC.cbClsExtra = NULL;
	wC.cbWndExtra = NULL;
	wC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wC.hIconSm = NULL;
	wC.hCursor = LoadCursor(NULL, IDC_CROSS);
	wC.hInstance = hInstance;
	wC.lpfnWndProc = WndProc;
	wC.lpszClassName = className;
	wC.lpszMenuName = 0;
	wC.style = CS_VREDRAW | CS_HREDRAW;

	initColours();

	ATOM result = RegisterClassEx(&wC);
	if (!result)
	{
		MessageBox(0, TEXT("B³¹d funkcji RegisterClassEx"), appName, MB_OK);
	}

	hwnd = CreateWindow(
		className,
		appName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd = NULL)
	{
		if (result)
		{
			UnregisterClass(className, hInstance);
			return -1;
		}
		return -1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (result)
	{
		UnregisterClass(className, hInstance);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH pudelko, pedzel;
		pedzel = CreateSolidBrush(RGB(255, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, rect[0].left, rect[0].top, rect[0].right, rect[0].bottom);
		SelectObject(hdc, pudelko);

		/////////////////////////////////////////

		pedzel = CreateSolidBrush(RGB(0, 255, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, rect[1].left, rect[1].top, rect[1].right, rect[1].bottom);
		SelectObject(hdc, pudelko);

		/////////////////////////////////////////

		pedzel = CreateSolidBrush(RGB(0, 0, 255));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, rect[2].left, rect[2].top, rect[2].right, rect[2].bottom);
		SelectObject(hdc, pudelko);

		/////////////////////////////////////////

		pedzel = CreateSolidBrush(RGB(0, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, rect[3].left, rect[3].top, rect[3].right, rect[3].bottom);
		SelectObject(hdc, pudelko);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_MOUSEMOVE:
		if (checkCollision(lParam))
			if (paintingLeft) {
				HDC hdc = GetDC(hwnd);

				for (int i = 0; i < RECTANGLE_SIZE; i++)
					for (int j = 0; j < RECTANGLE_SIZE; j++)
						SetPixel(hdc, LOWORD(lParam) + j, HIWORD(lParam) + i, RGB(red, green, blue));

				ReleaseDC(hwnd, hdc);
			}
			else if (paintingRight) {
				HDC hdc = GetDC(hwnd);

				HBRUSH pudelko, pedzel;
				HPEN box, pen;
				pedzel = CreateSolidBrush(RGB(red, green, blue));
				pudelko = (HBRUSH)SelectObject(hdc, pedzel);

				pen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
				box = (HPEN)SelectObject(hdc, pen);

				POINT point[3];
				point[0].x = LOWORD(lParam); point[0].y = HIWORD(lParam);
				point[1].x = LOWORD(lParam) + (RECTANGLE_SIZE / 2); point[1].y = HIWORD(lParam) - RECTANGLE_SIZE;
				point[2].x = LOWORD(lParam) + RECTANGLE_SIZE; point[2].y = HIWORD(lParam);

				Polygon(hdc, point, 3);
				SelectObject(hdc, pudelko);
				SelectObject(hdc, box);

				ReleaseDC(hwnd, hdc);
			}
			break;

	case WM_LBUTTONDOWN: {
		paintingLeft = true;

		SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam);
	}break;

	case WM_LBUTTONUP:
		paintingLeft = false;
		break;

	case WM_RBUTTONUP:
		paintingRight = false;
		break;

	case WM_RBUTTONDOWN: {
		paintingRight = true;

		SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam);
	}break;

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