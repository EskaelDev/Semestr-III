#include <Windows.h>
#include "resource.h"
#include <time.h>
#include <string>
#include <deque>

#define LEFT false
#define RIGHT true

#define CHICKEN_MOVE 3
#define PLAYER_MOVE 9
#define EGG_MOVE 12

#define TIMER1 1
#define TIMER2 2
#define TIMER3 3


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

struct EGG
{
	RECT area;
	int velocity;
};
std::deque<EGG> Eggs;

//void NewEgg(int x)
//{
//	EGG egg;
//	egg.x = x;
//	egg.y = 0;
//	egg.dropSpeed = EGG_MOVE;	
//	egg.basketTouched = 0;
//}
/*
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
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT PS;
	HDC hdc, hdcMemo;
	static HBITMAP hFarm, hBasket, hBasketMask, hEgg, hEggMask, hChicken, hChickenMask;
	//static HBITMAP hOLDBasket, hOLDBasketMask, hOLDEgg, hOLDEggMask, hOLDChicken, hOLDChickenMask;
	static BITMAP farmInfo, basketInfo, chickenInfo, eggInfo;
	static RECT clientRect;
	static int basketX, chickenX, score = 0, velocity = 10;

	switch (msg)
	{
	case WM_CREATE: 
	
	{
		/*	if (!SetTimer(hWnd, 1, 50, NULL))
			{
				MessageBox(hWnd, TEXT("Nie mozna utworzyc timera"), TEXT("Problem"), MB_ICONWARNING);
				break;
			}

				static POINT PLAYER = { 480, 480 },
							 ENEMY	= { 480, 0 };
				static HBITMAP hOldFarm, hOldBasket, hOldBasketMask, hOldEgg, hOldEggMask, hOldChicken, hOldChickenMask;
				static int points = 0;
				static bool direction = rand() % 2;
				static int counter = 0;

				static HBITMAP hFarm = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1));
				static HBITMAP hChicken = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP2));
				static HBITMAP hChickenMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP3));
				static HBITMAP hBasket = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP4));
				static HBITMAP hBasketMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP5));
				static HBITMAP hEgg = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP6));
				static HBITMAP hEggMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP6));
				*/

		

		//////////////////////////////////////////////////////////

		SetTimer(hWnd, TIMER1, 10, NULL);
		SetTimer(hWnd, TIMER2, 50, NULL);
		SetTimer(hWnd, TIMER3, 500, NULL);

		GetClientRect(hWnd, &clientRect);

		hFarm = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1));
		hChicken = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP11));
		hChickenMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP10));
		hBasket = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP9));
		hBasketMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP8));
		hEgg = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP13));
		hEggMask = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP12));


		GetObject(hFarm, sizeof(farmInfo), &farmInfo);
		GetObject(hBasket, sizeof(basketInfo), &basketInfo);
		GetObject(hChicken, sizeof(chickenInfo), &chickenInfo);
		GetObject(hEgg, sizeof(eggInfo), &eggInfo);

		basketX = (clientRect.right - basketInfo.bmWidth) / 2;
		chickenX = (clientRect.right - chickenInfo.bmWidth) / 2;
	}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;

	case WM_TIMER:
	{
		/*RECT rect;

		if (counter>=10)
		{
			direction = rand() % 2;
			counter = 0;
		}

		NewEgg(2);*/
		switch (wParam)
		{
		case TIMER1:
			InvalidateRect(hWnd, NULL, 0);
			for (deque<EGG>::iterator it = Eggs.begin(); it != Eggs.end(); it++)
			{
				if (it->area.bottom >= clientRect.bottom - basketInfo.bmHeight && (it->area.left > basketX && it->area.right < basketInfo.bmWidth + basketX))
				{
					score++;
					wchar_t life[27];
					wsprintf(life, TEXT("Z³apane jajka: %d"), score);
					SetWindowText(hWnd, life);
					Eggs.erase(it);
					break;
				}
				else
					if (it->area.bottom + it->velocity < clientRect.bottom)
					{
						it->area.top += it->velocity;
						it->area.bottom += it->velocity;
					}
					else
						Eggs.erase(it);
				break;
			}

			if (score == 5)
			{
				score = 0;
				MessageBox(NULL, TEXT("Wygra³eœ!"), TEXT("Koniec gry"), MB_OK);
			}
			break;

		case TIMER2:
			if (chickenInfo.bmWidth + chickenX >= clientRect.right)
				velocity = -velocity;

			if (velocity < 0 && chickenX < 0)
				velocity = -velocity;

			chickenX +=velocity;
			break;

		case TIMER3:
		{
			EGG eg;
			eg.velocity = 10;
			eg.area.left = chickenX + (chickenInfo.bmWidth / 2) - 5;
			eg.area.top = clientRect.top + chickenInfo.bmHeight + eg.velocity;
			eg.area.right = chickenX + chickenInfo.bmWidth / 2 + 5;
			eg.area.bottom = clientRect.top + chickenInfo.bmHeight + 15 - eg.velocity;
			Eggs.push_back(eg);
		}
		break;

		}
		break;


	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &PS);
		hdcMemo = CreateCompatibleDC(hdc);

		
		//loadBMP(hdc, hChicken, hOLDChicken, hChickenMask, hOLDChickenMask, chickenInfo, hdcMemo, chickenX, 0);

		SelectObject(hdcMemo, hFarm);
		BitBlt(hdc, 0, 0, farmInfo.bmWidth, farmInfo.bmHeight, hdcMemo, 0, 0, SRCCOPY);

		SelectObject(hdcMemo, hBasketMask);
		BitBlt(hdc, basketX, clientRect.bottom - basketInfo.bmHeight, basketInfo.bmWidth, basketInfo.bmHeight, hdcMemo, 0, 0, SRCAND);

		SelectObject(hdcMemo, hBasket);
		BitBlt(hdc, basketX, clientRect.bottom - basketInfo.bmHeight, basketInfo.bmWidth, basketInfo.bmHeight, hdcMemo, 0, 0, SRCPAINT);

		SelectObject(hdcMemo, hChickenMask);
		BitBlt(hdc, chickenX, clientRect.top, chickenInfo.bmWidth, chickenInfo.bmHeight, hdcMemo, 0, 0, SRCAND);

		SelectObject(hdcMemo, hChicken);
		BitBlt(hdc, chickenX, clientRect.top, chickenInfo.bmWidth, chickenInfo.bmHeight, hdcMemo, 0, 0, SRCPAINT);

		for (deque<EGG>::iterator it = Eggs.begin(); it != Eggs.end(); it++)
		{
			SelectObject(hdcMemo, hEggMask);
			BitBlt(hdc, it->area.left, it->area.top, eggInfo.bmWidth, eggInfo.bmHeight, hdcMemo, 0, 0, SRCAND);

			SelectObject(hdcMemo, hEgg);
			BitBlt(hdc, it->area.left, it->area.top, eggInfo.bmWidth, eggInfo.bmHeight, hdcMemo, 0, 0, SRCPAINT);
		}
		DeleteDC(hdcMemo);
		EndPaint(hWnd, &PS);

	}
	break;

	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_LEFT:
			if (basketX - 5 > clientRect.left)
				basketX -= 5;
			break;

		case VK_RIGHT:
			if (basketInfo.bmWidth + basketX + 5 < clientRect.right)
				basketX += 5;
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

	HWND mWnd = CreateWindowEx(0, ClassName, TEXT("Z³apane jajka = 0"), WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX) | WS_VISIBLE, 0, 0, 1000, 800, 0, 0, hInstance, 0);
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