// Pliki Naglowkowe
#include <Windows.h>
#include <stdio.h>
// Zmienne Globalne

TCHAR AppName[] = TEXT("Nazwa pierwsza");
TCHAR NewAppName[] = TEXT("Nazwa druga");
TCHAR TestName[] = TEXT("co tu pisze");
TCHAR Blad[] = TEXT("Blad");
int okno = 0;

// Deklaracje Funkcji
LRESULT CALLBACK ProceduraOkna(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static bool RejestrujKlase();

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	RejestrujKlase();

	// Tworzenie Okna
	HWND Okno_1 = CreateWindowEx(0, TestName, AppName,  WS_MAXIMIZEBOX, 100, 100, 400, 300, 0, 0, hInstance, 0);
	if (Okno_1 == NULL)
	{
		MessageBox(0, Blad, TEXT("Uchwyt okna nie utworzono"), MB_OK);
		return -1;
	}
	HWND Okno_2 = CreateWindowEx(0, TestName, AppName, WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, 0, 0, hInstance, 0);
	if (Okno_2 == NULL)
	{
		MessageBox(0, Blad, TEXT("Uchwyt okna nie utworzono"), MB_OK);
		return -1;
	}
	HWND Okno_3 = CreateWindowEx(0, TestName, AppName, WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, 0, 0, hInstance, 0);
	if (Okno_3 == NULL)
	{
		MessageBox(0, Blad, TEXT("Uchwyt okna nie utworzono"), MB_OK);
		return -1;	
	}

	// Wyswietlanie i odswierzanie okna
	ShowWindow(Okno_1, SW_NORMAL);
	UpdateWindow(Okno_1);
	ShowWindow(Okno_2, SW_NORMAL);
	UpdateWindow(Okno_2);
	ShowWindow(Okno_3, SW_NORMAL);
	UpdateWindow(Okno_3);

	// Petla Wiadomosci
	MSG msg;
	for (;;)
	{
		if (0 != GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;
	}
	return msg.wParam;
}

// Definicje Funkcji
LRESULT CALLBACK ProceduraOkna(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	TCHAR buff[64];
	HWND NoweOkno;
	switch (message)
	{
	case WM_CREATE:
		++okno;
		return 0;
	case WM_MOVE:
		swprintf(buff, TEXT("Pozycja okna %d x %d"), LOWORD(lParam), HIWORD(lParam));
		hdc = GetDC(hwnd);
		TextOut(hdc, 1, 1, buff, wcslen(buff));
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_SIZE:
		swprintf(buff, TEXT("Rozmiar okna %d x %d"), LOWORD(lParam), HIWORD(lParam));
		hdc = GetDC(hwnd);
		TextOut(hdc, 1, 20, buff, wcslen(buff));
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_DESTROY:
		if (okno == 0)
		{
			MessageBox(0, L"KONIEC!", L"", MB_OK);
			PostQuitMessage(0);
		}
		return 0;
	case WM_LBUTTONDOWN:
		NoweOkno = CreateWindowEx(0, TestName, AppName, WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, 0, 0, GetModuleHandle(0), 0);
		ShowWindow(NoweOkno, SW_SHOW);
		UpdateWindow(NoweOkno);
		return 0;
	case WM_CLOSE:
		--okno;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

static bool RejestrujKlase()
{
	WNDCLASSEX wndclassex;

	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc = ProceduraOkna;
	wndclassex.cbClsExtra = 0;
	wndclassex.cbWndExtra = 0;
	wndclassex.hInstance = GetModuleHandle(0);
	wndclassex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndclassex.hCursor = LoadCursor(0, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclassex.lpszMenuName = 0;
	wndclassex.lpszClassName = TestName;
	wndclassex.hIconSm = LoadIcon(0, IDI_APPLICATION);

	// Rejestracja Klasy Okna
	if (RegisterClassEx(&wndclassex) == 0)
	{
		MessageBox(0, Blad, TEXT("Nie zarejestrowano klasy"), MB_OK);
		return -1;
	}
	return 0;
}