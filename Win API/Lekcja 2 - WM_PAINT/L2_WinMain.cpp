// Pliki Naglowkowe
#include <Windows.h>

// Zmienne Globalne

TCHAR AppName[] = TEXT("Nazwa pierwsza");
TCHAR NewAppName[] = TEXT("Nazwa druga");
TCHAR TestName[] = TEXT("co tu pisze");
TCHAR Blad[] = TEXT("Blad");

// Deklaracje Funkcji
LRESULT CALLBACK ProceduraOkna(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndclassex;

	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_VREDRAW;
	wndclassex.lpfnWndProc = ProceduraOkna;
	wndclassex.cbClsExtra = 0;
	wndclassex.cbWndExtra = 0;
	wndclassex.hInstance = hInstance;
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

	// Tworzenie Okna
	HWND UchwytOkna = CreateWindowEx(0, TestName, AppName, WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, 0, 0, hInstance, 0);
	if (UchwytOkna == NULL)
	{
		MessageBox(0, Blad, TEXT("Uchwyt okna nie utworzono"), MB_OK);
		return -1;
	}


	// Wyswietlanie i odswierzanie okna
	ShowWindow(UchwytOkna, SW_NORMAL);
	UpdateWindow(UchwytOkna);


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
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}