#include <Windows.h>
#include <tchar.h>

#define COLOURS 16
#define rozmiar_pedzla 20
#define rozmiar_kwadrat 45

HWND hwnd = NULL;
MSG msg;
TCHAR Class_Name[] = TEXT("OKNO_TEST");
TCHAR Title[] = TEXT("Tytul");
bool paintingLeft = false;
enum FIGURE { TRIANGLE, FILL_TRIANGLE, CIRCLE, SQUARE };
FIGURE figure = SQUARE;
RECT przybornik;
RECT figures[4];
COLORREF color = RGB(0, 0, 0);

struct Colours {

	RECT rect;
	COLORREF color;
};

Colours colours[COLOURS];

void UstawPrzybornik() // ustawia przybornik po lewej stronie
{
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	przybornik.bottom = windowRect.bottom;
	przybornik.top = windowRect.top;
	przybornik.right = windowRect.right;
	przybornik.left = windowRect.right - 150;
}

void UstawFigury() // Ustawia figury pod kolorami
{
	int count = 0;
	int y = 20;
	for (int i = 0; i < 4; i++)
	{
		figures[i].left = colours[14 + count].rect.left;
		figures[i].top = colours[15].rect.top + rozmiar_kwadrat + y;
		figures[i].bottom = colours[15].rect.bottom + rozmiar_kwadrat + y;
		figures[i].right = colours[14 + count].rect.right;

		count++;
		if (count % 2 == 0)
		{
			count = 0;
			y += (rozmiar_kwadrat + 5);
		}
	}
}

void UstawKolory() // Ustawia kolory w przyborniku
{
	int y = 15; // Poczatkowy odstep od gory ekranu
	int count = 0; // Posluzy do przesuwania sie do kolejnego wiersza

	for (int i = 0; i < COLOURS; i++)
	{
		int x = ((10 + rozmiar_kwadrat)* count) + przybornik.left + 20;

		colours[i].rect.left = x;
		colours[i].rect.top = y;
		colours[i].rect.right = x + rozmiar_kwadrat;
		colours[i].rect.bottom = y + rozmiar_kwadrat;

		count++;
		if (count % 2 == 0)
		{
			count = 0;
			y += (rozmiar_kwadrat + 5);
		}


	}

}

void JakieKolory() // Tworzymy palete kolorow
{
	colours[0].color = RGB(0, 0, 0);
	colours[1].color = RGB(255, 255, 255);
	colours[2].color = RGB(128, 0, 0);
	colours[3].color = RGB(0, 128, 0);
	colours[4].color = RGB(0, 0, 128);
	colours[5].color = RGB(255, 0, 0);
	colours[6].color = RGB(0, 255, 0);
	colours[7].color = RGB(0, 0, 255);
	colours[8].color = RGB(128, 128, 128);
	colours[9].color = RGB(128, 128, 0);
	colours[10].color = RGB(128, 0, 128);
	colours[11].color = RGB(0, 128, 128);
	colours[12].color = RGB(64, 0, 0);
	colours[13].color = RGB(0, 64, 0);
	colours[14].color = RGB(0, 0, 64);
	colours[15].color = RGB(64, 64, 64);
}

bool SprawdzKolizjePrzybornik(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
	DWORD mouseX = LOWORD(lParam);
	DWORD mouseY = HIWORD(lParam);

	if (mouseX > rect.left - rozmiar_pedzla && mouseX < rect.right && mouseY < rect.bottom && mouseY + 50 > rect.top)
		return true;

	return false;
}

bool SprawdzKolizjeKwadraty(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
	DWORD mouseX = LOWORD(lParam);
	DWORD mouseY = HIWORD(lParam);

	if (mouseX > rect.left  && mouseX < rect.right && mouseY < rect.bottom && mouseY > rect.top)
		return true;

	return false;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_PAINT:
	{
		UstawPrzybornik();
		UstawKolory();
		UstawFigury();

		hdc = BeginPaint(hwnd, &ps);
		HBRUSH pudelko, pedzel;

		pedzel = CreateSolidBrush(RGB(128, 128, 128));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, przybornik.left, przybornik.top, przybornik.right, przybornik.bottom);
		SelectObject(hdc, pudelko);
		DeleteObject(pudelko);
		DeleteObject(pedzel);


		for (int i = 0; i < COLOURS; i++)
		{
			pedzel = CreateSolidBrush(colours[i].color);
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			Rectangle(hdc, colours[i].rect.left, colours[i].rect.top, colours[i].rect.right, colours[i].rect.bottom);
			SelectObject(hdc, pudelko);
			DeleteObject(pudelko);
			DeleteObject(pedzel);
		}

		for (int i = 0; i < 4; i++)
		{
			pedzel = CreateSolidBrush(RGB(255, 255, 255));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			Rectangle(hdc, figures[i].left, figures[i].top, figures[i].right, figures[i].bottom);
			SelectObject(hdc, pudelko);

			DeleteObject(pudelko);
			DeleteObject(pedzel);

		}

		pedzel = CreateSolidBrush(RGB(0, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		POINT point[3];
		point[0].x = figures[0].left; point[0].y = figures[0].top;
		point[1].x = figures[0].left + (rozmiar_kwadrat / 2); point[1].y = figures[0].top + rozmiar_kwadrat;
		point[2].x = figures[0].left + rozmiar_kwadrat; point[2].y = figures[0].top;
		Polygon(hdc, point, 3);

		SelectObject(hdc, pudelko);
		DeleteObject(pudelko);
		DeleteObject(pedzel);

		pedzel = CreateSolidBrush(RGB(255, 255, 255));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		point[0].x = figures[1].left; point[0].y = figures[1].top;
		point[1].x = figures[1].left + (rozmiar_kwadrat / 2); point[1].y = figures[1].top + rozmiar_kwadrat;
		point[2].x = figures[1].left + rozmiar_kwadrat; point[2].y = figures[1].top;
		Polygon(hdc, point, 3);

		SelectObject(hdc, pudelko);
		DeleteObject(pudelko);
		DeleteObject(pedzel);

		pedzel = CreateSolidBrush(RGB(0, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);
		Ellipse(hdc, figures[2].left + 10, figures[2].top + 10, figures[2].left + 40, figures[2].top + 40);
		SelectObject(hdc, pudelko);

		Rectangle(hdc, figures[3].left + 10, figures[3].top + 10, figures[3].left + 40, figures[3].top + 40);
		SelectObject(hdc, pudelko);

		DeleteObject(pudelko);
		DeleteObject(pedzel);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_MOUSEMOVE:

		if (!SprawdzKolizjePrzybornik(przybornik, lParam))
		{
			if (paintingLeft)
			{
				HDC hdc = GetDC(hwnd);

				HBRUSH pedzel, pudelko;
				HPEN box, pen;

				pedzel = CreateSolidBrush(color);
				pen = CreatePen(PS_SOLID, 1, color);

				pudelko = (HBRUSH)SelectObject(hdc, pedzel);
				box = (HPEN)SelectObject(hdc, pen);

				switch (figure)
				{
				case TRIANGLE: {
					pedzel = CreateSolidBrush(RGB(255, 255, 255));
					pudelko = (HBRUSH)SelectObject(hdc, pedzel);

					POINT point[3];
					point[0].x = LOWORD(lParam); point[0].y = HIWORD(lParam);
					point[1].x = LOWORD(lParam) + (rozmiar_pedzla / 2); point[1].y = HIWORD(lParam) - rozmiar_pedzla;
					point[2].x = LOWORD(lParam) + rozmiar_pedzla; point[2].y = HIWORD(lParam);

					Polygon(hdc, point, 3);
				}break;

				case FILL_TRIANGLE: {
					POINT point[3];
					point[0].x = LOWORD(lParam); point[0].y = HIWORD(lParam);
					point[1].x = LOWORD(lParam) + (rozmiar_pedzla / 2); point[1].y = HIWORD(lParam) - rozmiar_pedzla;
					point[2].x = LOWORD(lParam) + rozmiar_pedzla; point[2].y = HIWORD(lParam);

					Polygon(hdc, point, 3);
				}break;

				case CIRCLE:
					Ellipse(hdc, LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + rozmiar_pedzla, HIWORD(lParam) + rozmiar_pedzla);
					break;

				case SQUARE:
					Rectangle(hdc, LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + rozmiar_pedzla, HIWORD(lParam) + rozmiar_pedzla);
					break;
				}

				SelectObject(hdc, pudelko);
				SelectObject(hdc, box);

				DeleteObject(pen);
				DeleteObject(pedzel);
				DeleteObject(pudelko);
				DeleteObject(box);

				ReleaseDC(hwnd, hdc);

			}
		}
		else
		{
			if (paintingLeft)
			{
				for (int i = 0; i < COLOURS; i++)
					if (SprawdzKolizjeKwadraty(colours[i].rect, lParam)) {
						color = colours[i].color;

						break;
					}

				for (int i = 0; i < 4; i++)
					if (SprawdzKolizjeKwadraty(figures[i], lParam)) {
						switch (i) {
						case 0:
							figure = FILL_TRIANGLE;
							break;

						case 1:
							figure = TRIANGLE;
							break;

						case 2:
							figure = CIRCLE;
							break;

						case 3:
							figure = SQUARE;
							break;
						}

						break;
					}

			}

		}



		break;
	case WM_LBUTTONDOWN:
		paintingLeft = true;

		SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		paintingLeft = false;
		break;
	case WM_KEYDOWN:
		switch ((int)wParam)
		{
		case 0x43:
			InvalidateRect(hwnd, NULL, 1);
			break;
		}
		break;
	case WM_CREATE:
		JakieKolory();
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

//Main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ilCmdLine, int nCmdShow)
{
	WNDCLASSEX window;

	window.cbClsExtra = NULL;
	window.cbSize = sizeof(WNDCLASSEX);
	window.cbWndExtra = NULL;
	window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window.hIconSm = NULL;
	window.hInstance = hInstance;
	window.lpfnWndProc = WndProc;
	window.lpszClassName = Class_Name;
	window.lpszMenuName = 0;
	window.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&window);

	hwnd = ((CreateWindowEx(WS_EX_WINDOWEDGE, Class_Name, Title,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600, NULL, NULL, hInstance, NULL)));

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);



	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass(Class_Name, hInstance);
	return msg.wParam;
}



















#include <Windows.h>
#define COLOURS 16
#define rozmiar_pedzla 20
#define rozmiar_kwadrat 45

TCHAR className[] = TEXT("Nazwa_klasy");
TCHAR appName[] = TEXT("Nazwa_okna");
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DrawBoxOutline(HWND, POINT, POINT, RECT, LPARAM);
void UstawPrzybornik(HWND); // ustawia przybornik po lewej stronie
void UstawFigury(); // Ustawia figury pod kolorami
void UstawKolory(); // Ustawia kolory w przyborniku
void JakieKolory(); // Tworzymy palete kolorow
bool SprawdzKolizjePrzybornik(RECT, LPARAM); //wylaczy obszar przybornika z rysowania
bool SprawdzKolizjeKwadraty(RECT, LPARAM); //wylaczy obszar przybornika z rysowania

enum FIGURE { TRIANGLE, FILL_TRIANGLE, CIRCLE, SQUARE };
FIGURE figure = SQUARE;
RECT przybornik;
RECT figures[4];
COLORREF color = RGB(0, 0, 0);

struct Colours {
	RECT rect;
	COLORREF color;
} colours[COLOURS];

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
	wC.hCursor = LoadCursor(NULL, IDC_CROSS);

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
	static BOOL fBlocking, fValidBox;
	static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_CREATE:
		JakieKolory();
		break;

	case WM_LBUTTONDOWN:
	{
		if (!SprawdzKolizjePrzybornik(przybornik, lParam))
		{
			ptBeg.x = ptEnd.x = LOWORD(lParam);
			ptBeg.y = ptEnd.y = HIWORD(lParam);

			DrawBoxOutline(hwnd, ptBeg, ptEnd, przybornik, lParam);

			SetCapture(hwnd);
			SetCursor(LoadCursor(NULL, IDC_CROSS));

			fBlocking = true;
		}
		else
		{
			for (int i = 0; i < COLOURS; i++)
				if (SprawdzKolizjeKwadraty(colours[i].rect, lParam))
				{
					color = colours[i].color;
					break;
				}

			for (int i = 0; i < 4; i++)
			{
				if (SprawdzKolizjeKwadraty(figures[i], lParam))
				{
					switch (i)
					{
					case 0:
						figure = FILL_TRIANGLE;
						break;

					case 1:
						figure = TRIANGLE;
						break;

					case 2:
						figure = CIRCLE;
						break;

					case 3:
						figure = SQUARE;
						break;
					}
					break;
				}
			}
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		if (!SprawdzKolizjePrzybornik(przybornik, lParam))
		{
			if (fBlocking)
			{
				SetCursor(LoadCursor(NULL, IDC_CROSS));
				DrawBoxOutline(hwnd, ptBeg, ptEnd, przybornik, lParam);

				ptEnd.x = LOWORD(lParam);
				ptEnd.y = HIWORD(lParam);

				DrawBoxOutline(hwnd, ptBeg, ptEnd, przybornik, lParam);
			}
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		if (!SprawdzKolizjePrzybornik(przybornik, lParam))
		{
			if (fBlocking)
			{
				DrawBoxOutline(hwnd, ptBeg, ptEnd, przybornik, lParam);

				ptBoxBeg = ptBeg;
				ptBoxEnd.x = LOWORD(lParam);
				ptBoxEnd.y = HIWORD(lParam);

				ReleaseCapture();
				SetCursor(LoadCursor(NULL, IDC_ARROW));

				fBlocking = false;
				fValidBox = true;

				InvalidateRect(hwnd, NULL, false);
			}
		}
	}
	break;

	case WM_CHAR:
	{
		if (fBlocking & (wParam == '\x1B'))
		{
			DrawBoxOutline(hwnd, ptBeg, ptEnd, przybornik, lParam);

			ReleaseCapture();
			SetCursor(LoadCursor(NULL, IDC_ARROW));

			fBlocking = false;
		}
	}
	break;

	case WM_PAINT:
	{
		if (!SprawdzKolizjePrzybornik(przybornik, lParam))
		{
			UstawPrzybornik(hwnd);
			UstawKolory();
			UstawFigury();
			hdc = BeginPaint(hwnd, &ps);
			HBRUSH pudelko, pedzel;

			pedzel = CreateSolidBrush(RGB(128, 128, 128));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			Rectangle(hdc, przybornik.left, przybornik.top, przybornik.right, przybornik.bottom);
			SelectObject(hdc, pudelko);
			DeleteObject(pudelko);
			DeleteObject(pedzel);


			for (int i = 0; i < COLOURS; i++)
			{
				pedzel = CreateSolidBrush(colours[i].color);
				pudelko = (HBRUSH)SelectObject(hdc, pedzel);

				Rectangle(hdc, colours[i].rect.left, colours[i].rect.top, colours[i].rect.right, colours[i].rect.bottom);
				SelectObject(hdc, pudelko);
				DeleteObject(pudelko);
				DeleteObject(pedzel);
			}

			for (int i = 0; i < 4; i++)
			{
				pedzel = CreateSolidBrush(RGB(255, 255, 255));
				pudelko = (HBRUSH)SelectObject(hdc, pedzel);

				Rectangle(hdc, figures[i].left, figures[i].top, figures[i].right, figures[i].bottom);
				SelectObject(hdc, pudelko);

				DeleteObject(pudelko);
				DeleteObject(pedzel);
			}
			pedzel = CreateSolidBrush(RGB(0, 0, 0));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			POINT point[3];
			point[0].x = figures[0].left; point[0].y = figures[0].top;
			point[1].x = figures[0].left + (rozmiar_kwadrat / 2); point[1].y = figures[0].top + rozmiar_kwadrat;
			point[2].x = figures[0].left + rozmiar_kwadrat; point[2].y = figures[0].top;
			Polygon(hdc, point, 3);

			SelectObject(hdc, pudelko);
			DeleteObject(pudelko);
			DeleteObject(pedzel);

			pedzel = CreateSolidBrush(RGB(255, 255, 255));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			point[0].x = figures[1].left; point[0].y = figures[1].top;
			point[1].x = figures[1].left + (rozmiar_kwadrat / 2); point[1].y = figures[1].top + rozmiar_kwadrat;
			point[2].x = figures[1].left + rozmiar_kwadrat; point[2].y = figures[1].top;
			Polygon(hdc, point, 3);

			SelectObject(hdc, pudelko);
			DeleteObject(pudelko);
			DeleteObject(pedzel);

			pedzel = CreateSolidBrush(RGB(0, 0, 0));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);
			Ellipse(hdc, figures[2].left + 10, figures[2].top + 10, figures[2].left + 40, figures[2].top + 40);
			SelectObject(hdc, pudelko);

			Rectangle(hdc, figures[3].left + 10, figures[3].top + 10, figures[3].left + 40, figures[3].top + 40);
			SelectObject(hdc, pudelko);

			DeleteObject(pudelko);
			DeleteObject(pedzel);

			if (fValidBox)
			{
				pedzel = CreateSolidBrush(color);
				pudelko = (HBRUSH)SelectObject(hdc, pedzel);
				switch (figure)
				{
				case SQUARE:
				{
					Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
					SelectObject(hdc, pedzel);
					DeleteObject(pudelko);
					DeleteObject(pedzel);
					break;
				}
				case TRIANGLE:
				{
					pedzel = CreateSolidBrush(RGB(255, 255, 255));
					pudelko = (HBRUSH)SelectObject(hdc, pedzel);
					POINT point[3];
					point[0].x = ptBoxBeg.x;
					point[0].y = ptBoxEnd.y;

					point[1].x = ptBoxEnd.x;
					point[1].y = ptBoxEnd.y;

					point[2].x = (ptBoxBeg.x + ptBoxEnd.x) / 2;
					point[2].y = ptBoxBeg.y;

					Polygon(hdc, point, 3);
					SelectObject(hdc, pedzel);
					DeleteObject(pudelko);
					DeleteObject(pedzel);
					break;
				}
				case CIRCLE:
				{
					Ellipse(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
					SelectObject(hdc, pedzel);
					DeleteObject(pudelko);
					DeleteObject(pedzel);
					break;
				}
				case FILL_TRIANGLE:
				{
					POINT point[3];
					point[0].x = ptBoxBeg.x;
					point[0].y = ptBoxEnd.y;

					point[1].x = ptBoxEnd.x;
					point[1].y = ptBoxEnd.y;

					point[2].x = (ptBoxBeg.x + ptBoxEnd.x) / 2;
					point[2].y = ptBoxBeg.y;

					Polygon(hdc, point, 3);
					SelectObject(hdc, pedzel);
					DeleteObject(pudelko);
					DeleteObject(pedzel);
					break;
				}
				}


			}
			if (fBlocking)
			{
				SetROP2(hdc, R2_NOT);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
			}
			EndPaint(hwnd, &ps);
		}
	}
	break;

	case WM_CLOSE:
	{
		if (MessageBox(0, TEXT("Czy na pewno chcesz zakoñczyæ dzia³anie aplikacji?"), appName, MB_YESNO) == IDYES)
			DestroyWindow(hwnd);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	UpdateWindow(hwnd);
	return 0;
}

void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd, RECT rect, LPARAM lParam)
{
	if (!SprawdzKolizjePrzybornik(rect, lParam))
	{
		HDC hdc;
		hdc = GetDC(hwnd);

		SetROP2(hdc, R2_NOT);
		SelectObject(hdc, GetStockObject(NULL_BRUSH));

		Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
		ReleaseDC(hwnd, hdc);
	}
}

void UstawPrzybornik(HWND hwnd) // ustawia przybornik po lewej stronie
{
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	przybornik.bottom = windowRect.bottom;
	przybornik.top = windowRect.top;
	przybornik.right = windowRect.right;
	przybornik.left = windowRect.right - 150;
}

void UstawFigury() // Ustawia figury pod kolorami
{
	int count = 0;
	int y = 20;
	for (int i = 0; i < 4; i++)
	{
		figures[i].left = colours[14 + count].rect.left;
		figures[i].top = colours[15].rect.top + rozmiar_kwadrat + y;
		figures[i].bottom = colours[15].rect.bottom + rozmiar_kwadrat + y;
		figures[i].right = colours[14 + count].rect.right;

		count++;
		if (count % 2 == 0)
		{
			count = 0;
			y += (rozmiar_kwadrat + 5);
		}
	}
}

void UstawKolory()  // Ustawia kolory w przyborniku
{
	int x = 0,
		y = 15;     // Poczatkowy odstep od gory ekranu
	int count = 0;  // Posluzy do przesuwania sie do kolejnego wiersza

	for (int i = 0; i < COLOURS; i++)
	{
		x = ((10 + rozmiar_kwadrat)* count) + przybornik.left + 20;

		colours[i].rect.left = x;
		colours[i].rect.top = y;
		colours[i].rect.right = x + rozmiar_kwadrat;
		colours[i].rect.bottom = y + rozmiar_kwadrat;

		count++;
		if (count % 2 == 0)
		{
			count = 0;
			y += (rozmiar_kwadrat + 5);
		}
	}
}

void JakieKolory() // Tworzymy palete kolorow
{
	colours[0].color = RGB(0, 0, 0);
	colours[1].color = RGB(150, 150, 150);
	colours[2].color = RGB(150, 0, 0);
	colours[3].color = RGB(255, 0, 0);
	colours[4].color = RGB(0, 150, 0);
	colours[5].color = RGB(0, 255, 0);
	colours[6].color = RGB(150, 150, 0);
	colours[7].color = RGB(255, 255, 0);
	colours[8].color = RGB(0, 0, 150);
	colours[9].color = RGB(0, 0, 255);
	colours[10].color = RGB(150, 0, 150);
	colours[11].color = RGB(255, 0, 255);
	colours[12].color = RGB(0, 150, 150);
	colours[13].color = RGB(0, 255, 255);
	colours[14].color = RGB(100, 100, 100);
	colours[15].color = RGB(255, 255, 255);
}

bool SprawdzKolizjePrzybornik(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
	DWORD mouseX = LOWORD(lParam);
	DWORD mouseY = HIWORD(lParam);

	if (mouseX > rect.left && mouseX < rect.right && mouseY < rect.bottom && mouseY + 50 > rect.top)
		return true;
	return false;
}

bool SprawdzKolizjeKwadraty(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
	DWORD mouseX = LOWORD(lParam);
	DWORD mouseY = HIWORD(lParam);

	if (mouseX > rect.left  && mouseX < rect.right && mouseY < rect.bottom && mouseY > rect.top)
		return true;
	return false;
}

















#include <Windows.h>
#include <tchar.h>

#define COLOURS 16
#define rozmiar_pedzla 20
#define rozmiar_kwadrat 45
TCHAR className[] = TEXT("Nazwa_klasy"); //Adrianowe
TCHAR appName[] = TEXT("Nazwa_okna");	//Adrianowe
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND hwnd = NULL;
HDC hdc = NULL;
PAINTSTRUCT ps;
bool paintingLeft = false;
MSG msg;
enum FIGURE { TRIANGLE, FILL_TRIANGLE, CIRCLE, SQUARE };
FIGURE figure = SQUARE;
RECT przybornik;
RECT figures[4];
COLORREF color = RGB(0, 0, 0);


struct Colours {

RECT rect;
COLORREF color;
};
Colours colours[COLOURS];

void UstawPrzybornik(HWND); // ustawia przybornik po lewej stronie
void UstawFigury(); // Ustawia figury pod kolorami
void UstawKolory(); // Ustawia kolory w przyborniku
void JakieKolory(); // Tworzymy palete kolorow
bool SprawdzKolizjePrzybornik(RECT rect, LPARAM lParam); //wylaczy obszar przybornika z rysowania
bool SprawdzKolizjeKwadraty(RECT rect, LPARAM lParam); //wylaczy obszar przybornika z rysowania

void DrawBoxOutline(HWND, POINT, POINT); // Adrianowe
//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ilCmdLine, int nCmdShow)
{
WNDCLASSEX wC = { 0 };


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
wC.hCursor = LoadCursor(NULL, IDC_CROSS);

ATOM result = RegisterClassEx(&wC);
if (!result)
{
MessageBox(0, TEXT("B³¹d funkcji RegisterClassEx"), appName, MB_OK);
}

hwnd = ((CreateWindowEx(WS_EX_WINDOWEDGE, className, appName,
WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
800, 600, NULL, NULL, hInstance, NULL)));

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
UnregisterClass(className, hInstance);
return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
static BOOL fBlocking, fValidBox;
static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;


switch (msg)
{
case WM_LBUTTONDOWN:
ptBeg.x = ptEnd.x = LOWORD(lParam);
ptBeg.y = ptEnd.y = HIWORD(lParam);

DrawBoxOutline(hwnd, ptBeg, ptEnd);

SetCapture(hwnd);
SetCursor(LoadCursor(NULL, IDC_CROSS));

fBlocking = true;
break;

case WM_MOUSEMOVE:

if (fBlocking)
{
SetCursor(LoadCursor(NULL, IDC_CROSS));
DrawBoxOutline(hwnd, ptBeg, ptEnd);

ptEnd.x = LOWORD(lParam);
ptEnd.y = HIWORD(lParam);

DrawBoxOutline(hwnd, ptBeg, ptEnd);
}
break;

case WM_LBUTTONUP:
if (fBlocking)
{
DrawBoxOutline(hwnd, ptBeg, ptEnd);

ptBoxBeg = ptBeg;
ptBoxEnd.x = LOWORD(lParam);
ptBoxEnd.y = HIWORD(lParam);

ReleaseCapture();
SetCursor(LoadCursor(NULL, IDC_ARROW));

fBlocking = false;
fValidBox = true;

InvalidateRect(hwnd, NULL, false);
}
break;

case WM_CHAR:
if (fBlocking & (wParam == '\x1B'))
{
DrawBoxOutline(hwnd, ptBeg, ptEnd);

ReleaseCapture();
SetCursor(LoadCursor(NULL, IDC_ARROW));

fBlocking = false;
}
break;

case WM_PAINT:
UstawPrzybornik(hwnd);
UstawKolory();
JakieKolory();

UstawFigury();
hdc = BeginPaint(hwnd, &ps);
HBRUSH pudelko, pedzel;

pedzel = CreateSolidBrush(RGB(128, 128, 128));
pudelko = (HBRUSH)SelectObject(hdc, pedzel);

Rectangle(hdc, przybornik.left, przybornik.top, przybornik.right, przybornik.bottom);
SelectObject(hdc, pudelko);
DeleteObject(pudelko);
DeleteObject(pedzel);


for (int i = 0; i < COLOURS; i++)
{
pedzel = CreateSolidBrush(colours[i].color);
pudelko = (HBRUSH)SelectObject(hdc, pedzel);

Rectangle(hdc, colours[i].rect.left, colours[i].rect.top, colours[i].rect.right, colours[i].rect.bottom);
SelectObject(hdc, pudelko);
DeleteObject(pudelko);
DeleteObject(pedzel);
}

for (int i = 0; i < 4; i++)
{
pedzel = CreateSolidBrush(RGB(255, 255, 255));
pudelko = (HBRUSH)SelectObject(hdc, pedzel);

Rectangle(hdc, figures[i].left, figures[i].top, figures[i].right, figures[i].bottom);
SelectObject(hdc, pudelko);

DeleteObject(pudelko);
DeleteObject(pedzel);

}

////////////////
pedzel = CreateSolidBrush(RGB(0, 0, 0));
pudelko = (HBRUSH)SelectObject(hdc, pedzel);

POINT point[3];
point[0].x = figures[0].left; point[0].y = figures[0].top;
point[1].x = figures[0].left + (rozmiar_kwadrat / 2); point[1].y = figures[0].top + rozmiar_kwadrat;
point[2].x = figures[0].left + rozmiar_kwadrat; point[2].y = figures[0].top;
Polygon(hdc, point, 3);

SelectObject(hdc, pudelko);
DeleteObject(pudelko);
DeleteObject(pedzel);

pedzel = CreateSolidBrush(RGB(255, 255, 255));
pudelko = (HBRUSH)SelectObject(hdc, pedzel);

point[0].x = figures[1].left; point[0].y = figures[1].top;
point[1].x = figures[1].left + (rozmiar_kwadrat / 2); point[1].y = figures[1].top + rozmiar_kwadrat;
point[2].x = figures[1].left + rozmiar_kwadrat; point[2].y = figures[1].top;
Polygon(hdc, point, 3);

SelectObject(hdc, pudelko);
DeleteObject(pudelko);
DeleteObject(pedzel);

pedzel = CreateSolidBrush(RGB(0, 0, 0));
pudelko = (HBRUSH)SelectObject(hdc, pedzel);
Ellipse(hdc, figures[2].left + 10, figures[2].top + 10, figures[2].left + 40, figures[2].top + 40);
SelectObject(hdc, pudelko);

Rectangle(hdc, figures[3].left + 10, figures[3].top + 10, figures[3].left + 40, figures[3].top + 40);
SelectObject(hdc, pudelko);

DeleteObject(pudelko);
DeleteObject(pedzel);
////////////////
if (fValidBox)
{
SelectObject(hdc, GetStockObject(BLACK_BRUSH));
Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
}
if (fBlocking)
{
SetROP2(hdc, R2_NOT);
SelectObject(hdc, GetStockObject(NULL_BRUSH));
Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
}
EndPaint(hwnd, &ps);
break;

case WM_CLOSE:
//if (MessageBox(0, TEXT("Czy na pewno chcesz zakoñczyæ dzia³anie aplikacji?"), appName, MB_YESNO) == 6)
DestroyWindow(hwnd);
break;
case WM_DESTROY:
PostQuitMessage(0);
break;
default:
return DefWindowProc(hwnd, msg, wParam, lParam);
}
}
//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//
void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd)
{
HDC hdc;

hdc = GetDC(hwnd);

SetROP2(hdc, R2_NOT);
SelectObject(hdc, GetStockObject(NULL_BRUSH));
Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);

ReleaseDC(hwnd, hdc);
}

void UstawPrzybornik(HWND hwnd) // ustawia przybornik po lewej stronie
{
RECT windowRect;
GetClientRect(hwnd, &windowRect);

przybornik.bottom = windowRect.bottom;
przybornik.top = windowRect.top;
przybornik.right = windowRect.right;
przybornik.left = windowRect.right - 150;
}

void UstawFigury() // Ustawia figury pod kolorami
{
int count = 0;
int y = 20;
for (int i = 0; i < 4; i++)
{
figures[i].left = colours[14 + count].rect.left;
figures[i].top = colours[15].rect.top + rozmiar_kwadrat + y;
figures[i].bottom = colours[15].rect.bottom + rozmiar_kwadrat + y;
figures[i].right = colours[14 + count].rect.right;

count++;
if (count % 2 == 0)
{
count = 0;
y += (rozmiar_kwadrat + 5);
}
}
}

void UstawKolory() // Ustawia kolory w przyborniku
{
int y = 15; // Poczatkowy odstep od gory ekranu
int count = 0; // Posluzy do przesuwania sie do kolejnego wiersza

for (int i = 0; i < COLOURS; i++)
{
int x = ((10 + rozmiar_kwadrat)* count) + przybornik.left + 20;

colours[i].rect.left = x;
colours[i].rect.top = y;
colours[i].rect.right = x + rozmiar_kwadrat;
colours[i].rect.bottom = y + rozmiar_kwadrat;

count++;
if (count % 2 == 0)
{
count = 0;
y += (rozmiar_kwadrat + 5);
}


}

}

void JakieKolory() // Tworzymy palete kolorow
{
colours[0].color = RGB(0, 0, 0);
colours[1].color = RGB(255, 255, 255);
colours[2].color = RGB(128, 0, 0);
colours[3].color = RGB(0, 128, 0);
colours[4].color = RGB(0, 0, 128);
colours[5].color = RGB(255, 0, 0);
colours[6].color = RGB(0, 255, 0);
colours[7].color = RGB(0, 0, 255);
colours[8].color = RGB(128, 128, 128);
colours[9].color = RGB(128, 128, 0);
colours[10].color = RGB(128, 0, 128);
colours[11].color = RGB(0, 128, 128);
colours[12].color = RGB(64, 0, 0);
colours[13].color = RGB(0, 64, 0);
colours[14].color = RGB(0, 0, 64);
colours[15].color = RGB(64, 64, 64);
}

bool SprawdzKolizjePrzybornik(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
DWORD mouseX = LOWORD(lParam);
DWORD mouseY = HIWORD(lParam);

if (mouseX > rect.left - rozmiar_pedzla && mouseX < rect.right && mouseY < rect.bottom && mouseY + 50 > rect.top)
return true;

return false;
}

bool SprawdzKolizjeKwadraty(RECT rect, LPARAM lParam) //wylaczy obszar przybornika z rysowania
{
DWORD mouseX = LOWORD(lParam);
DWORD mouseY = HIWORD(lParam);

if (mouseX > rect.left  && mouseX < rect.right && mouseY < rect.bottom && mouseY > rect.top)
return true;

return false;
}