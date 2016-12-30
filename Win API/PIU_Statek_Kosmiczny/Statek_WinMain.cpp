#include <Windows.h>
#include "resource.h"
#include <time.h>
#include <string>

#define LEFT false
#define RIGHT true

#define ENEMY_MOVE 3
#define PLAYER_MOVE 9
#define ROCKET_MOVE 12

using namespace std;

wchar_t ClassName[] = TEXT("Klasa");

inline void loadBMP(HDC hdc, HBITMAP &hBitmap, HBITMAP &hOldBitmap, HBITMAP &hMask, HBITMAP &hOldMask, BITMAP &Info, HDC &hMemory, int x, int y)
{
	hOldMask = (HBITMAP)SelectObject(hMemory, hMask);
	GetObject(hMask, sizeof(Info), &Info);
	BitBlt(hdc, x, y, Info.bmWidth, Info.bmHeight, hMemory, 0, 0, SRCAND);
	SelectObject(hMemory, hOldMask);

	hOldBitmap = (HBITMAP)SelectObject(hMemory, hBitmap);
	GetObject(hMask, sizeof(Info), &Info);
	BitBlt(hdc, x, y, Info.bmWidth, Info.bmHeight, hMemory, 0, 0, SRCPAINT);
	SelectObject(hMemory, hOldBitmap);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINT PLAYER = { 480, 480 },
				 ENEMY = { 480, 0 },
				 SHOOT = { PLAYER.x + 26, 480 };
	static HBITMAP hOldSpace, hOldSpaceCraft, hOldMask, hOldsShoot, hOldShootMask;
	static int damage = 0;
	static bool launched = false;
	static bool direction = rand() % 2;
	static int counter = 0;

	static HBITMAP hSpace = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP8));
	static HBITMAP hSpaceCraft = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1));
	static HBITMAP hMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP3));
	static HBITMAP hShoot = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP6));
	static HBITMAP hShootMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP7));

	switch (msg)
	{
	case WM_CREATE:
		if (!SetTimer(hWnd, 1, 50, NULL))
		{
			MessageBox(hWnd, TEXT("Nie mozna utworzyc timera"), TEXT("Problem"), MB_ICONWARNING);
			break;
		}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	
	case WM_TIMER:
	{
		RECT rect;

		if (counter >= 10)
		{
			direction = rand() % 2;
			counter = 0;
		}
		if (launched)
		{
			if (SHOOT.y <= ENEMY.y + 160)
				if (SHOOT.x >= ENEMY.x && SHOOT.x <= ENEMY.x + 80)
				{
					damage++;
					launched = false;
					SHOOT.y = 480;
					wchar_t life[27];
					wsprintf(life, TEXT("Trafienia wroga: %d"), damage);
					SetWindowText(hWnd, life);

					if (damage == 10)
					{
						KillTimer(hWnd, 1);
						MessageBox(0, TEXT("Zwyciestwo"), TEXT("Wygrana"), MB_OK | MB_ICONINFORMATION);
						PostQuitMessage(0);
					}
				}
				else if (SHOOT.y <= 0)
				{
					launched = false;
					SHOOT.y = 480;
				}
		}
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, true);
	}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT PS;
		HDC hdc = BeginPaint(hWnd, &PS);
		HDC hMemory = CreateCompatibleDC(hdc);
		BITMAP Info;

		hOldSpace = (HBITMAP)SelectObject(hMemory, hSpace);
		GetObject(hSpace, sizeof(Info), &Info);
		BitBlt(hdc, 0, 0, Info.bmWidth, Info.bmHeight, hMemory, 0, 0, SRCCOPY);
		SelectObject(hMemory, hOldSpace);

		loadBMP(hdc, hSpaceCraft, hOldSpaceCraft, hMask, hOldMask, Info, hMemory, PLAYER.x, 480);

		if (launched)
		{
			loadBMP(hdc, hShoot, hOldsShoot, hShootMask, hOldShootMask, Info, hMemory, SHOOT.x, SHOOT.y);
			SHOOT.y -= ROCKET_MOVE;
		}
		if (direction == RIGHT)
		{
			if (ENEMY.x + 80 + ENEMY_MOVE >= 1000)
				direction = !direction;
			else
			{
				loadBMP(hdc, hSpaceCraft, hOldSpace, hMask, hOldMask, Info, hMemory, ENEMY.x, 0);
				ENEMY.x += ENEMY_MOVE;
			}
			counter++;
		}
		if (direction == LEFT)
		{
			if (ENEMY.x - ENEMY_MOVE <= 0)
				direction = !direction;
			else
			{
				loadBMP(hdc, hSpaceCraft, hOldSpaceCraft, hMask, hOldMask, Info, hMemory, ENEMY.x, 0);
				ENEMY.x -= ENEMY_MOVE;
			}
			counter++;
		}
		DeleteDC(hMemory);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &PS);
	}
		break;

	case WM_KEYDOWN:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		switch (LOWORD(wParam))
		{
		case VK_LEFT:
			if (PLAYER.x - PLAYER_MOVE >= 0)
			{
				PLAYER.x -= PLAYER_MOVE;
			}
			break;

		case VK_RIGHT:
			if (PLAYER.x + 80 + PLAYER_MOVE < 1000)
			{
				PLAYER.x += PLAYER_MOVE;
			}
			break;

		case VK_SPACE:
			if (!launched)
			{
				launched = true;
				SHOOT.x = PLAYER.x + 26;
			}
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	srand(time(NULL));
	WNDCLASSEX kl;
	kl.cbClsExtra = kl.cbWndExtra = 0;
	kl.cbSize = sizeof(WNDCLASSEX);
	kl.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	kl.hCursor = LoadCursor(hInstance, IDI_APPLICATION);
	kl.hIcon = kl.hIconSm = LoadIcon(hInstance, IDC_ICON);
	kl.hInstance = hInstance;
	kl.lpfnWndProc = &WndProc;
	kl.lpszClassName = ClassName;
	kl.lpszMenuName = NULL;
	kl.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&kl))
	{
		MessageBox(0, TEXT("Nie zarejestrowano klasy okna!"), TEXT("Blad!"), MB_OK | MB_ICONERROR); return 0;
	}

	HWND mWnd = CreateWindowEx(0, ClassName, TEXT("Trafienia wroga = 0"), WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX) | WS_VISIBLE, 0, 0, 1000, 800, 0, 0, hInstance, 0);
	if (!mWnd)
	{
		MessageBox(0, TEXT("Nie stworzono okna!"), TEXT("Blad!"), MB_OK | MB_ICONERROR); return 0;
	}

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(ClassName, hInstance);
	return msg.wParam;
}