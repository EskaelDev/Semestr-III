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
		if (MessageBox(0, TEXT("Czy na pewno chcesz zako�czy� dzia�anie aplikacji?"), appName, MB_YESNO) == IDYES)
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



////////////////////////////////////////////////////////////////////PASZKWIL

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define COLOURS 24
#define RECTANGLE_SIZE 30
#define SQUARE_SIZE 50
#define FIGURES_LIMIT 20
#define ANGLE 15

#include <Windows.h>
#include <deque>
#include <tchar.h>

using namespace std;

MSG msg;
TCHAR className[] = TEXT("Simple Class");
HWND hwnd;
COLORREF color = RGB(0, 0, 0);
RECT drawPanelRect;

unsigned int pixelSize = 10;
bool paintingLeft = false, paintingRight = false;
bool freeMode = false;
int figureNumber = 0;

POINT diffrentFigure[2];

enum FIGURE { TRIANGLE, FILL_TRIANGLE, CIRCLE, SQUARE };
FIGURE figure = SQUARE;

RECT figures[4];
float rotateFigures[4];

struct Colours {
	RECT rect;
	COLORREF color;
};

struct Container {
	float x, y;
	float sizeX, sizeY;
	float rotate;
	COLORREF color;
	FIGURE figure;
};

Colours colours[COLOURS];
deque<Container> posContainer;

void initDrawPanelRect() {
	const float x = SCREEN_WIDTH * 0.7f;

	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	drawPanelRect.left = SCREEN_WIDTH * 0.7f;
	drawPanelRect.top = -1;
	drawPanelRect.right = windowRect.right;
	drawPanelRect.bottom = windowRect.bottom + 2;
}

void initFigures() {
	for (unsigned int i = 0; i < 4; i++) {
		figures[i].left = colours[20 + i].rect.left;
		figures[i].top = colours[23].rect.top + SQUARE_SIZE + 20;
		figures[i].right = colours[20 + i].rect.right;
		figures[i].bottom = colours[23].rect.bottom + SQUARE_SIZE + 20;
	}
}

void initColours() {
	int y = 10;
	int count = 0;

	for (unsigned int i = 0; i < COLOURS; i++) {
		int x = ((5 + SQUARE_SIZE) * count) + drawPanelRect.left + 5;

		colours[i].rect.left = x;
		colours[i].rect.top = y;
		colours[i].rect.right = x + SQUARE_SIZE;
		colours[i].rect.bottom = y + SQUARE_SIZE;

		count++;

		if (count % 4 == 0) {
			count = 0;
			y += (SQUARE_SIZE + 5);
		}
	}

	colours[0].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[1].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[2].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[3].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[4].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[5].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[6].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[7].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[8].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[9].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[10].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[11].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[12].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[13].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[14].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[15].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[16].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[17].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[18].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[19].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[20].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[21].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[22].color = RGB(rand() % 255, rand() % 255, rand() % 255);
	colours[23].color = RGB(rand() % 255, rand() % 255, rand() % 255);
}

bool checkCollision(RECT rect, LPARAM lParam) {
	float mouseX = LOWORD(lParam);
	float mouseY = HIWORD(lParam);

	if (mouseX > rect.left - SQUARE_SIZE && mouseX < rect.right && mouseY < rect.bottom && mouseY > rect.top)
		return true;

	return false;
}

void drawTriangle(const HDC hdc, const float x, const float y, const float sizeX, const float sizeY, const float rotate) {
	const float rad = (float)rotate / (360.0f / (2 * 3.1416f));

	const float xPos = x + (sizeX / 2);
	const float yPos = y + (sizeY / 2);

	POINT angle[3];

	angle[0].x = cos(rad) * (x - xPos) - sin(rad) * (y - yPos) + xPos;
	angle[0].y = sin(rad) * (x - xPos) + cos(rad) * (y - yPos) + yPos;

	angle[1].x = cos(rad) * ((x - xPos) + sizeX) - sin(rad) * (y - yPos) + xPos;
	angle[1].y = sin(rad) * ((x - xPos) + sizeX) + cos(rad) * (y - yPos) + yPos;

	angle[2].x = cos(rad) * ((x - xPos) + (sizeX / 2)) - sin(rad) * ((y - yPos) + sizeY) + xPos;
	angle[2].y = sin(rad) * ((x - xPos) + (sizeX / 2)) + cos(rad) * ((y - yPos) + sizeY) + yPos;

	Polygon(hdc, angle, 3);
}

void drawRectangle(const HDC hdc, const float x, const float y, const float sizeX, const float sizeY, const float rotate) {
	const float rad = (float)rotate / (360.0f / (2 * 3.1416f));

	const float xPos = x + (sizeX / 2);
	const float yPos = y + (sizeY / 2);

	POINT angle[4];

	angle[0].x = cos(rad) * (x - xPos) - sin(rad) * (y - yPos) + xPos;
	angle[0].y = sin(rad) * (x - xPos) + cos(rad) * (y - yPos) + yPos;

	angle[1].x = cos(rad) * ((x - xPos) + sizeX) - sin(rad) * (y - yPos) + xPos;
	angle[1].y = sin(rad) * ((x - xPos) + sizeX) + cos(rad) * (y - yPos) + yPos;

	angle[2].x = cos(rad) * ((x - xPos) + sizeX) - sin(rad) * ((y - yPos) + sizeY) + xPos;
	angle[2].y = sin(rad) * ((x - xPos) + sizeX) + cos(rad) * ((y - yPos) + sizeY) + yPos;

	angle[3].x = cos(rad) * (x - xPos) - sin(rad) * ((y - yPos) + sizeY) + xPos;
	angle[3].y = sin(rad) * (x - xPos) + cos(rad) * ((y - yPos) + sizeY) + yPos;

	Polygon(hdc, angle, 4);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_PAINT: {
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH pudelko, pedzel;

		pedzel = CreateSolidBrush(RGB(128, 128, 128));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		Rectangle(hdc, drawPanelRect.left, drawPanelRect.top, drawPanelRect.right, drawPanelRect.bottom);

		for (unsigned int i = 0; i < COLOURS; i++) {
			pedzel = CreateSolidBrush(colours[i].color);
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			Rectangle(hdc, colours[i].rect.left, colours[i].rect.top, colours[i].rect.right, colours[i].rect.bottom);
			SelectObject(hdc, pudelko);
		}

		for (unsigned int i = 0; i < 4; i++) {
			pedzel = CreateSolidBrush(RGB(255, 255, 255));
			pudelko = (HBRUSH)SelectObject(hdc, pedzel);

			Rectangle(hdc, figures[i].left, figures[i].top, figures[i].right, figures[i].bottom);
			SelectObject(hdc, pudelko);
		}

		pedzel = CreateSolidBrush(RGB(0, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		drawTriangle(hdc, figures[0].left + 10, figures[0].top + 10, 30, 30, 0);
		SelectObject(hdc, pudelko);

		pedzel = CreateSolidBrush(RGB(255, 255, 255));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);

		drawTriangle(hdc, figures[1].left + 10, figures[1].top + 10, 30, 30, 0);
		SelectObject(hdc, pudelko);

		pedzel = CreateSolidBrush(RGB(0, 0, 0));
		pudelko = (HBRUSH)SelectObject(hdc, pedzel);
		Ellipse(hdc, figures[2].left + 10, figures[2].top + 10, figures[2].left + 40, figures[2].top + 40);
		SelectObject(hdc, pudelko);

		Rectangle(hdc, figures[3].left + 10, figures[3].top + 10, figures[3].left + 40, figures[3].top + 40);

		DeleteObject(pudelko);
		DeleteObject(pedzel);

		if (posContainer.size() > 0)
			for (unsigned int i = 0; i < posContainer.size(); i++) {
				HPEN box, pen;

				pedzel = CreateSolidBrush(posContainer[i].color);
				pen = CreatePen(PS_SOLID, 1, posContainer[i].color);

				pudelko = (HBRUSH)SelectObject(hdc, pedzel);
				box = (HPEN)SelectObject(hdc, pen);

				switch (posContainer[i].figure)
				{
				case TRIANGLE: {
					pedzel = CreateSolidBrush(RGB(255, 255, 255));
					pudelko = (HBRUSH)SelectObject(hdc, pedzel);

					drawTriangle(hdc, posContainer[i].x, posContainer[i].y, posContainer[i].sizeX, posContainer[i].sizeY, posContainer[i].rotate);
				}break;

				case FILL_TRIANGLE: {
					drawTriangle(hdc, posContainer[i].x, posContainer[i].y, posContainer[i].sizeX, posContainer[i].sizeY, posContainer[i].rotate);
				}break;

				case CIRCLE:
					Ellipse(hdc, posContainer[i].x, posContainer[i].y, posContainer[i].x + posContainer[i].sizeX, posContainer[i].y + posContainer[i].sizeY);
					break;

				case SQUARE:
					drawRectangle(hdc, posContainer[i].x, posContainer[i].y, posContainer[i].sizeX, posContainer[i].sizeY, posContainer[i].rotate);
					break;
				}

				SelectObject(hdc, box);
				SelectObject(hdc, pudelko);

				DeleteObject(box);
				DeleteObject(pudelko);
				DeleteObject(pedzel);
				DeleteObject(pen);
			}

		EndPaint(hwnd, &ps);
	}break;

	case WM_MOUSEMOVE:
		if (!checkCollision(drawPanelRect, lParam)) {
			bool isRotate = false;

			if (paintingLeft && !freeMode && posContainer.size() < FIGURES_LIMIT) {
				for (int i = 0; i < posContainer.size(); i++) {
					RECT rect;
					rect.left = posContainer[i].x;
					rect.top = posContainer[i].y;
					rect.right = posContainer[i].x + posContainer[i].sizeX;
					rect.bottom = posContainer[i].y + posContainer[i].sizeY;

					if (checkCollision(rect, lParam)) {
						posContainer[i].rotate += ANGLE;
						if (posContainer[i].rotate >= 360.0f)
							posContainer[i].rotate = posContainer[i].rotate - 360.0f;

						isRotate = true;

						InvalidateRect(hwnd, NULL, 1);
					}
				}
			}

			if (paintingLeft && !freeMode && posContainer.size() < FIGURES_LIMIT - 1 && !isRotate) {
				Container container;
				container.color = color;
				container.figure = figure;
				container.x = LOWORD(lParam);
				container.y = HIWORD(lParam);
				container.sizeX = SQUARE_SIZE;
				container.sizeY = SQUARE_SIZE;
				container.rotate = 0;

				posContainer.push_back(container);

				InvalidateRect(hwnd, NULL, 1);
			}

			if (paintingRight && posContainer.size() < FIGURES_LIMIT) {
				for (int i = 0; i < posContainer.size(); i++) {
					RECT rect;
					rect.left = posContainer[i].x;
					rect.top = posContainer[i].y;
					rect.right = posContainer[i].x + posContainer[i].sizeX;
					rect.bottom = posContainer[i].y + posContainer[i].sizeY;

					if (checkCollision(rect, lParam)) {
						posContainer.erase(posContainer.begin() + i);
						InvalidateRect(hwnd, NULL, 1);
						i = 0;
					}
				}
			}
		}
		else {
			if (paintingLeft) {
				for (int i = 0; i < COLOURS; i++)
					if (checkCollision(colours[i].rect, lParam)) {
						color = colours[i].color;

						break;
					}

				for (int i = 0; i < 4; i++)
					if (checkCollision(figures[i], lParam)) {
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

	case WM_LBUTTONDOWN: {
		paintingLeft = true;

		if (freeMode) {
			diffrentFigure[figureNumber].x = LOWORD(lParam);
			diffrentFigure[figureNumber].y = HIWORD(lParam);

			if (figureNumber < 1)
				figureNumber++;
			else {
				figureNumber = 0;
				freeMode = false;

				Container container;
				container.color = color;
				container.figure = figure;
				container.x = diffrentFigure[0].x;
				container.y = diffrentFigure[0].y;
				container.sizeX = (diffrentFigure[1].x - diffrentFigure[0].x);
				container.sizeY = (diffrentFigure[1].y - diffrentFigure[0].y);
				container.rotate = 0;

				posContainer.push_back(container);

				InvalidateRect(hwnd, NULL, 1);
			}
		}

		SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam);
	}break;

	case WM_LBUTTONUP:
		paintingLeft = false;
		break;

	case WM_RBUTTONDOWN: {
		paintingRight = true;

		SendMessage(hwnd, WM_MOUSEMOVE, wParam, lParam);
	}break;

	case WM_RBUTTONUP:
		paintingRight = false;
		break;

	case WM_KEYDOWN:
		switch ((int)wParam)
		{
		case 0x43:
			posContainer.clear();
			InvalidateRect(hwnd, NULL, 1);
			break;

		case VK_F1:
			freeMode = !freeMode;
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
		MessageBox(NULL, TEXT("Nie uda�o si� zarejestrowa� klasy okna!"), TEXT("B��d"), MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx(0, className, TEXT("TU RYSUJ XD"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("Nie uda�o si� utworzy� okna 1!"), TEXT("CreateWindowEx"), MB_OK | MB_ICONERROR);
		return 1;
	}

	initDrawPanelRect();
	initColours();
	initFigures();

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

////////////////////////////////////////////////////PASZKWIL













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
MessageBox(0, TEXT("B��d funkcji RegisterClassEx"), appName, MB_OK);
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
//if (MessageBox(0, TEXT("Czy na pewno chcesz zako�czy� dzia�anie aplikacji?"), appName, MB_YESNO) == 6)
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