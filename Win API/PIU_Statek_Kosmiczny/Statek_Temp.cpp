#include <windows.h>
#include "resource.h"
#include <time.h>
#include <string>

#define LEWO false
#define PRAWO true

#define ENEMY_MOVE 5
#define PLAYER_MOVE 15
#define SHOT_MOVE 15

using namespace std;

char nkl[] = "Klasa";

inline void wczytajBMP(HDC hdc, HBITMAP &hBitmap, HBITMAP &hOldBitmap, HBITMAP &hMask, HBITMAP &hOldMask, BITMAP &Info, HDC &hMemory, int x, int y)
{
	hOldMask = (HBITMAP)SelectObject(hMemory, hMask);
	GetObject(hMask, sizeof(Info), &Info);
	BitBlt(hdc, x, y, Info.bmWidth, Info.bmHeight, hMemory, 0, 0, SRCAND);
	SelectObject(hMemory, hOldMask);

	hOldBitmap = (HBITMAP)SelectObject(hMemory, hBitmap);
	GetObject(hBitmap, sizeof(Info), &Info);
	BitBlt(hdc, x, y, Info.bmWidth, Info.bmHeight, hMemory, 0, 0, SRCPAINT);
	SelectObject(hMemory, hOldBitmap);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINT PLAYER = { 480, 480 }, ENEMY = { 480, 0 }, SHOT = { PLAYER.x + 26, 480 };
	static HBITMAP hOldSpace, hOldSpacecraft, hOldMask, hOldShoot, hOldShootMask;
	static int damage = 0;
	static bool launched = false;
	static bool kierunek = rand() % 2;
	static int licznik = 0;

	static HBITMAP hSpace = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_SPACE));
	static HBITMAP hSpacecraft = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_SPACECRAFT));
	static HBITMAP hMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_MASK));
	static HBITMAP hShoot = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_SHOT));
	static HBITMAP hShootMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_SHOT_MASK));

	switch (msg)
	{
	case WM_CREATE:
		if (!SetTimer(hWnd, 1, 50, NULL))
		{
			MessageBox(hWnd, "Nie mo¿na utworzyæ timera!", "Problem", MB_ICONWARNING);
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

		if (licznik >= 10)
		{
			kierunek = rand() % 2;
			licznik = 0;
		}
		if (launched)
		{
			if (SHOT.y <= ENEMY.y + 160)
				if (SHOT.x >= ENEMY.x && SHOT.x <= ENEMY.x + 80)
				{
					damage++;
					launched = false;
					SHOT.y = 480;
					char life[27];
					sprintf(life, "Trafienia wroga : %d", damage);
					SetWindowText(hWnd, life);
					if (damage == 10)
					{
						KillTimer(hWnd, 1);
						MessageBox(0, "Zwyciê¿y³eœ wroga!", "Wygrana", MB_OK | MB_ICONINFORMATION);
						PostQuitMessage(0);
					}
				}
				else if (SHOT.y <= 0)
				{
					launched = false;
					SHOT.y = 480;
				}
		}
		GetClientRect(hWnd, &rect);
		InvalidateRect(hWnd, &rect, true);

	}break;

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

		wczytajBMP(hdc, hSpacecraft, hOldSpacecraft, hMask, hOldMask, Info, hMemory, PLAYER.x, 480);

		if (launched)
		{
			wczytajBMP(hdc, hShoot, hOldShoot, hShootMask, hOldShootMask, Info, hMemory, SHOT.x, SHOT.y);
			SHOT.y -= SHOT_MOVE;
		}
		if (kierunek == PRAWO)
		{
			if (ENEMY.x + 80 + ENEMY_MOVE >= 1000)
				kierunek = !kierunek;
			else
			{
				wczytajBMP(hdc, hSpacecraft, hOldSpacecraft, hMask, hOldMask, Info, hMemory, ENEMY.x, 0);
				ENEMY.x += ENEMY_MOVE;
			}
			licznik++;
		}
		if (kierunek == LEWO)
		{
			if (ENEMY.x - ENEMY_MOVE <= 0)
				kierunek = !kierunek;
			else
			{
				wczytajBMP(hdc, hSpacecraft, hOldSpacecraft, hMask, hOldMask, Info, hMemory, ENEMY.x, 0);
				ENEMY.x -= ENEMY_MOVE;
			}
			licznik++;
		}
		DeleteDC(hMemory);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &PS);
	} break;

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
				SHOT.x = PLAYER.x + 26;
			}
			break;
		case VK_ESCAPE:
			PostQuitMessage(0); break;
		}
	} break;

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
	kl.lpszClassName = nkl;
	kl.lpszMenuName = NULL;
	kl.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&kl))
	{
		MessageBox(0, TEXT("Nie zarejestrowano klasy okna!"), TEXT("Blad!"), MB_OK | MB_ICONERROR); return 0;
	}

	HWND mWnd = CreateWindowEx(0, nkl, TEXT("Trafienia wroga = 0"), WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX) | WS_VISIBLE, 0, 0, 1000, 800, 0, 0, hInstance, 0);
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
	UnregisterClass(nkl, hInstance);
	return msg.wParam;
}