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



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

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
		
	}
	break;

	case WM_PAINT:
	{
		
	}
	break;

	case WM_KEYDOWN:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		switch (LOWORD(wParam))
		{
		case VK_LEFT:
			
			break;

		case VK_RIGHT:
			
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