#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500
#define SQUARE_SIZE 20
#define EAT_SQUARE_SIZE 30
#define SPEED 10

#include <Windows.h>
#include <time.h>
#include <deque>
#include <tchar.h>

using namespace std;

MSG msg;
TCHAR className[] = TEXT("Snake");
HWND hwnd;
RECT clientRect;

UINT timer = 250;
UINT score = 0;

RECT foodRect;
COLORREF foodColor = RGB(255, 0, 0);
COLORREF greenColor = RGB(0, 255, 0);
COLORREF brownColor = RGB(139, 69, 19);

enum DIRECTION { UP, DOWN, LEFT, RIGHT };
DIRECTION direction = UP;

struct Container {
	RECT rect;
	RECT safeOldRect;
	COLORREF color;
	DIRECTION direction;
	DIRECTION oldDirection;
};

deque<Container> posContainer;

void initFood() {
	srand(time(NULL));

	foodColor = RGB(rand() % 255, rand() % 255, rand() % 255);

	int size = ((rand() % 3) + 2) * 10;
	int x = (rand() % (clientRect.right - clientRect.left - size)) + clientRect.left;
	int y = (rand() % (clientRect.bottom - clientRect.top - size)) + clientRect.top;

	foodRect.left = x;
	foodRect.top = y;
	foodRect.right = x + size;
	foodRect.bottom = y + size;
}

void initSnake() {
	srand(time(NULL));

	int size = SQUARE_SIZE;
	int x = clientRect.left + 300;
	int y = clientRect.top + 300;

	Container container;
	container.rect.left = x;
	container.rect.top = y;
	container.rect.right = x + size;
	container.rect.bottom = y + size;
	container.safeOldRect = container.rect;
	container.direction = UP;
	container.oldDirection = UP;
	container.color = brownColor;

	posContainer.push_back(container);
}

bool checkRectCollision(RECT rect1, RECT rect2) {
	if (rect1.left < rect2.right &&
		rect1.right > rect2.left &&
		rect1.top < rect2.bottom &&
		rect1.bottom > rect2.top) {
		return true;
	}

	return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 1, timer, 0);
		GetClientRect(hwnd, &clientRect);
		initFood();
		initSnake();
		break;

	case WM_PAINT: {
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH brushBox, brush;
		HPEN penBox, pen;

		brush = CreateSolidBrush(foodColor);
		brushBox = (HBRUSH)SelectObject(hdc, brush);

		pen = CreatePen(PS_SOLID, 1, foodColor);
		penBox = (HPEN)SelectObject(hdc, pen);

		Rectangle(hdc, foodRect.left, foodRect.top, foodRect.right, foodRect.bottom);

		for (deque<Container>::iterator it = posContainer.begin(); it != posContainer.end(); ++it) {
			brush = CreateSolidBrush(it->color);
			brushBox = (HBRUSH)SelectObject(hdc, brush);

			pen = CreatePen(PS_SOLID, 3, it->color);
			penBox = (HPEN)SelectObject(hdc, pen);

			Rectangle(hdc, it->rect.left, it->rect.top, it->rect.right, it->rect.bottom);
			SelectObject(hdc, brushBox);
			SelectObject(hdc, penBox);

			DeleteObject(brushBox);
			DeleteObject(brush);
			DeleteObject(penBox);
			DeleteObject(pen);
		}

		TCHAR buf[128];
		_swprintf(buf, TEXT("SCORE: %i           SIZE: %i \n"), score, posContainer.size());

		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 50, 10, buf, wcslen(buf));

		EndPaint(hwnd, &ps);
	}break;

	case WM_TIMER: {
		posContainer[0].direction = direction;
		posContainer[0].oldDirection = posContainer[0].direction;

		int counter = 0;

		for (deque<Container>::iterator it = posContainer.begin(); it != posContainer.end(); ++it) {
			switch (it->direction) {
			case UP: {
				it->rect.top -= (SPEED + SQUARE_SIZE);
				it->rect.bottom -= (SPEED + SQUARE_SIZE);

				if (it->rect.top < clientRect.top) {
					it->rect.top = clientRect.bottom - SQUARE_SIZE - 10;
					it->rect.bottom = clientRect.bottom - 10;
				}
			}break;

			case DOWN:
				it->rect.top += (SPEED + SQUARE_SIZE);
				it->rect.bottom += (SPEED + SQUARE_SIZE);

				if (it->rect.bottom > clientRect.bottom) {
					it->rect.top = clientRect.top + 10;
					it->rect.bottom = clientRect.top + SQUARE_SIZE + 10;
				}
				break;

			case RIGHT:
				it->rect.left += (SPEED + SQUARE_SIZE);
				it->rect.right += (SPEED + SQUARE_SIZE);

				if (it->rect.right > clientRect.right) {
					it->rect.left = clientRect.left + 10;
					it->rect.right = clientRect.left + SQUARE_SIZE + 10;
				}
				break;

			case LEFT:
				it->rect.left -= (SPEED + SQUARE_SIZE);
				it->rect.right -= (SPEED + SQUARE_SIZE);

				if (it->rect.left < clientRect.left) {
					it->rect.left = clientRect.right - SQUARE_SIZE - 10;
					it->rect.right = clientRect.right - 10;
				}
				break;
			}
		}

		for (UINT i = 1; i < posContainer.size(); i++) {
			posContainer[i].oldDirection = posContainer[i].direction;
			posContainer[i].direction = posContainer[i - 1].oldDirection;
		}

		if (checkRectCollision(posContainer[0].rect, foodRect)) {
			initFood();

			Container container;
			container.direction = posContainer[posContainer.size() - 1].direction;

			if ((posContainer.size() + 1) % 2 == 0)
				container.color = greenColor;
			else
				container.color = brownColor;

			RECT rect = posContainer[posContainer.size() - 1].rect;
			switch (container.direction) {
			case UP:
				rect.top += (SPEED + SQUARE_SIZE);
				rect.bottom += (SPEED + SQUARE_SIZE);
				break;

			case DOWN:
				rect.top -= (SPEED + SQUARE_SIZE);
				rect.bottom -= (SPEED + SQUARE_SIZE);
				break;

			case RIGHT:
				rect.left -= (SPEED + SQUARE_SIZE);
				rect.right -= (SPEED + SQUARE_SIZE);
				break;

			case LEFT:
				rect.left += (SPEED + SQUARE_SIZE);
				rect.right += (SPEED + SQUARE_SIZE);
				break;
			}

			container.rect = rect;
			posContainer.push_back(container);



			if (timer > 30) {
				timer -= 10;
				SetTimer(hwnd, 1, timer, 0);
			}

			score += (300 - timer);
		}

		if (posContainer.size() > 1)
			for (deque<Container>::iterator it = posContainer.begin() + 1; it != posContainer.end(); ++it)
				if (checkRectCollision(posContainer[0].rect, it->rect)) {
					posContainer.clear();
					initSnake();
					timer = 250;

					SetTimer(hwnd, 1, timer, 0);
					break;
				}

		InvalidateRect(hwnd, NULL, 1);
	}break;

	case WM_KEYDOWN:
		switch ((int)wParam) {
		case VK_UP:
			direction = UP;
			break;

		case VK_DOWN:
			direction = DOWN;
			break;

		case VK_RIGHT:
			direction = RIGHT;
			break;

		case VK_LEFT:
			direction = LEFT;
			break;
		}
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
		MessageBox(NULL, TEXT("Nie uda�o si� zarejestrowa� klasy okna!"), TEXT("Error 404"), MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(0, className, TEXT("Snake"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("Nie uda�o si� utworzy� okna !"), TEXT("CreateWindowEx"), MB_OK | MB_ICONERROR);
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