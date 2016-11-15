#include <Windows.h>
#include <deque>

#define ID_TIMER 1

using namespace std;

TCHAR className[] = TEXT("Class name");
TCHAR appName[] = TEXT("Falling stars");
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Star
{
private:
	RECT field;
	bool ground;
public:
	Star(int, int, int);
	void draw(HDC);
	void move();
	void setGround(bool _ground)
	{
		ground = _ground;
	};
	const RECT getField()
	{
		return field;
	};
	const bool getGround()
	{
		return ground;
	};

	bool checkCollision(Star container2)
	{
		RECT rect2;
		rect2.left = container2.getField().left;
		rect2.top = container2.getField().top;
		rect2.right = container2.getField().right;
		rect2.bottom = container2.getField().bottom;

		if (field.left >= rect2.right)
			return false;

		if (field.right <= rect2.left)
			return false;

		if (field.top >= rect2.bottom)
			return false;

		if (field.bottom <= rect2.top)
			return false;

		return true;
	}

};

Star::Star(int left, int top, int size)
{
	field.left = left;
	field.right = left + size;
	field.top = top;
	field.bottom = top + size;
	ground = false;
}

void Star::draw(HDC hdc)
{
	Ellipse(hdc, field.left, field.top, field.right, field.bottom);
}

void Star::move()
{
	if (!ground)
	{
		int xMove = rand() % 15 + (-7),
			yMove = rand() % 10;
		field.left += xMove;
		field.right += xMove;
		field.top += yMove;
		field.bottom += yMove;
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ilCmdLine, int nCmdShow)
{
	WNDCLASSEX wC = { 0 };
	HWND hwnd = NULL;
	MSG msg;

	wC.cbSize = sizeof(WNDCLASSEX);
	wC.style = CS_VREDRAW | CS_HREDRAW;
	wC.cbClsExtra = NULL;
	wC.cbWndExtra = NULL;
	wC.hInstance = hInstance;
	wC.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 200));
	wC.lpfnWndProc = WndProc;
	wC.lpszClassName = className;
	wC.lpszMenuName = 0;
	wC.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wC.hIconSm = NULL;
	wC.hCursor = LoadCursor(NULL, IDC_ARROW);

	ATOM result = RegisterClassEx(&wC);
	if (!result)
	{
		MessageBox(0, TEXT("B��d funkcji RegisterClassEx"), appName, MB_OK);
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
	static deque<Star>container;
	static deque<Star>container2;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT clientRect;

	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER, 100, 0);
		HPEN pen;
		HBRUSH brush;
		brush = CreateSolidBrush(RGB(255, 255, 255));
		pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		for (deque<Star>::iterator it = container.begin(); it != container.end(); ++it)
			it->draw(hdc);

		for (deque<Star>::iterator it = container2.begin(); it != container2.end(); ++it)
			it->draw(hdc);

		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
	{
		GetClientRect(hwnd, &clientRect);
		Star s(rand() % clientRect.right, rand() % clientRect.bottom * 3 / 4, rand() % 20);
		container.push_back(s);
		for (deque<Star>::iterator it = container.begin(); it != container.end(); ++it)
		{
			if (it->getField().bottom < clientRect.bottom && it->getGround() == false)
			{
				it->move();
			}
			else
			{
				container2.push_back(*it);
				container.erase(it);
				break;
			}
		}
		if (container.size() > 0 && container2.size() > 0)
			for (deque<Star>::iterator it = container.begin(); it != container.end(); ++it)
				for (deque<Star>::iterator it2 = container2.begin(); it2 != container2.end(); ++it2)
					if (it->checkCollision(*it2))
					{
						it->setGround(true);
						break;
					}
		InvalidateRect(hwnd, NULL, true);
	}
	break;

	case WM_SIZE:
		GetClientRect(hwnd, &clientRect);
		break;

	case WM_CLOSE:
		if (MessageBox(0, TEXT("Czy na pewno chcesz zako�czy� dzia�anie aplikacji?"), appName, MB_YESNO) == IDYES)
			DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		DeleteObject(pen);
		DeleteObject(brush);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}