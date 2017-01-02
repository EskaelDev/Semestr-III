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





























#include <Windows.h>
#include <time.h>
#include <deque>

using namespace std;

#define TIMER1 1
#define TIMER2 2

struct Bullet
{
	RECT surface;
	int speed;
};

deque<Bullet>bullet;

TCHAR className[] = TEXT("Nazwa_klasy");
TCHAR appName[] = TEXT("Statek");
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	wC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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
		(WS_OVERLAPPEDWINDOW | WS_VISIBLE) ^ (WS_SIZEBOX),
		100,
		100,
		1000,
		800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hdcMem;
	static HBITMAP background, player, playerMask, enemy, enemyMask;
	static BITMAP bgInfo, playerInfo, enemyInfo;
	static HBRUSH whiteColor;
	static RECT cR;
	static int playerX, enemyX, damage = 0, speed = 10;

	switch (msg)
	{
	case WM_CREATE:
	{
		SetTimer(hwnd, TIMER1, 10, NULL);
		SetTimer(hwnd, TIMER2, 80, NULL);
		GetClientRect(hwnd, &cR);

		background = (HBITMAP)LoadImage(NULL, TEXT("bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		player = (HBITMAP)LoadImage(NULL, TEXT("ship.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		playerMask = (HBITMAP)LoadImage(NULL, TEXT("ship_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		enemy = (HBITMAP)LoadImage(NULL, TEXT("enemy.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		enemyMask = (HBITMAP)LoadImage(NULL, TEXT("enemy_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		whiteColor = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));

		GetObject(background, sizeof(bgInfo), &bgInfo);
		GetObject(player, sizeof(playerInfo), &playerInfo);
		GetObject(enemy, sizeof(enemyInfo), &enemyInfo);
		playerX = (cR.right - cR.left - playerInfo.bmWidth) / 2;
		enemyX = (cR.right - cR.left - enemyInfo.bmWidth) / 2;
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, background);
		BitBlt(hdc, 0, 0, bgInfo.bmWidth, bgInfo.bmHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, playerMask);
		BitBlt(hdc, playerX, cR.bottom - playerInfo.bmHeight, playerInfo.bmWidth, playerInfo.bmHeight, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, player);
		BitBlt(hdc, playerX, cR.bottom - playerInfo.bmHeight, playerInfo.bmWidth, playerInfo.bmHeight, hdcMem, 0, 0, SRCPAINT);

		SelectObject(hdcMem, enemyMask);
		BitBlt(hdc, enemyX, cR.top, enemyInfo.bmWidth, enemyInfo.bmHeight, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, enemy);
		BitBlt(hdc, enemyX, cR.top, enemyInfo.bmWidth, enemyInfo.bmHeight, hdcMem, 0, 0, SRCPAINT);

		for (deque<Bullet>::iterator it = bullet.begin(); it != bullet.end(); ++it)
		{
			SelectObject(hdc, whiteColor);
			Ellipse(hdc, it->surface.left, it->surface.top, it->surface.right, it->surface.bottom);
		}

		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_TIMER:
		switch (wParam)
		{
		case TIMER1:
			InvalidateRect(hwnd, NULL, 0);
			for (deque<Bullet>::iterator it = bullet.begin(); it != bullet.end(); it++)
			{
				if (it->surface.top <= cR.top + enemyInfo.bmWidth && (it->surface.left > enemyX && it->surface.right < enemyX + enemyInfo.bmWidth))
				{
					damage++;
					bullet.erase(it);
					break;
				}
				else if (it->surface.top - it->speed > 0)
				{
					it->surface.top -= it->speed;
					it->surface.bottom -= it->speed;
				}
				else
					bullet.erase(it);
				break;
			}
			if (10 == damage)
			{
				damage = 0;
				MessageBox(NULL, TEXT("Wygra�e�!"), TEXT("Koniec gry"), MB_OK);
			}

			break;
		case TIMER2:
			if (enemyX + enemyInfo.bmWidth >= cR.right)
				speed = -speed;

			if (speed < 0 && enemyX < 0)
				speed = -speed;

			enemyX += speed;
			break;
		}
		break;


	case WM_KEYDOWN:
		switch ((int)wParam)
		{
		case VK_RIGHT:
			if (playerX + playerInfo.bmWidth + 5 < cR.right)
				playerX += 5;
			break;

		case VK_LEFT:
			if (playerX - 5 > cR.left)
				playerX -= 5;
			break;

		case VK_SPACE:
			if (bullet.size() < 1)
			{
				Bullet b;
				b.speed = 10;
				b.surface.left = playerX + (playerInfo.bmWidth / 2) - 5;
				b.surface.top = cR.bottom - playerInfo.bmHeight - b.speed;
				b.surface.right = playerX + (playerInfo.bmWidth / 2) + 5;
				b.surface.bottom = cR.bottom - playerInfo.bmHeight + 15 - b.speed;
				bullet.push_back(b);
			}
			break;

		default:
			break;
		}
		break;

	case WM_CLOSE:
		if (MessageBox(0, TEXT("Czy na pewno chcesz zako�czy� dzia�anie aplikacji?"), appName, MB_YESNO) == IDYES)
			DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}