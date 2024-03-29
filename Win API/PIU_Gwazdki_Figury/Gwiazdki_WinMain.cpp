/************************************************************
* -- NIE USUWAJ TEJ INFORMACJI Z PROGRAMU ---------------- *
************************************************************
* -- Program powsta? na bazie kodu ?r�d?owego ------------ *
* -- udost?pnionego studentom na potrzeby przedmiotu ----- *
* -- Programowanie Interfejsu U?ytkownika ---------------- *
* -- Copyright (c) 2009 Politechnika ?l?ska w Gliwicach -- *
* -- Rados?aw Sok�?, Wydzia? Elektryczny ----------------- *
************************************************************/

#include <windows.h>
#include <stdio.h>
#include <list>

using namespace std;
TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");
// ###################################
// ##### Klasa 'Gwiazdka' ############
// ###################################
class Gwiazdka
{
private:
	int Skala,
		xPoz,
		yPoz;
	COLORREF Wypelnienie,
		Obramowanie;
public:
	Gwiazdka(int skala, int x, int y, COLORREF wypelnienie, COLORREF obramowanie);
	void Rysuj(HDC Kont);
	void Przesun(bool v);
	int GetX() { return xPoz; };
	int GetY() { return yPoz; };
};
// ###################################
// ##### Konstruktor parametryczny ###
// ###################################
Gwiazdka::Gwiazdka(int skala, int x, int y, COLORREF wypelnienie, COLORREF obramowanie)
{
	Skala = skala;
	xPoz = x;
	yPoz = y;
	Wypelnienie = wypelnienie;
	Obramowanie = obramowanie;
}
// ###################################
// ##### Funkcja rysujaca gwizdke ####
// ###################################
void Gwiazdka::Rysuj(HDC Kont)
{
	HPEN Border = CreatePen(PS_SOLID, 3, Obramowanie);
	HBRUSH Bk = CreateSolidBrush(Wypelnienie);

	SelectObject(Kont, (HPEN)Border);
	SelectObject(Kont, (HBRUSH)Bk);
	POINT P[8] = { { xPoz, yPoz + 3 * Skala },{ xPoz + 1 * Skala, yPoz + (1 * Skala) },{ xPoz + 3 * Skala, yPoz },
	{ xPoz + (1 * Skala), yPoz - (1 * Skala) },{ xPoz, yPoz - (3 * Skala) },{ xPoz - (1 * Skala), yPoz - (1 * Skala) },
	{ xPoz - (3 * Skala), yPoz },{ xPoz - (1 * Skala), yPoz + (1 * Skala) } };
	Polygon(Kont, P, 8);
	DeleteObject(Border);
	DeleteObject(Bk);
}
// ###################################
// ##### Funkcja przesuwajaca ########
// ###################################
void Gwiazdka::Przesun(bool obszar)
{
	int xMove = rand() % 5,
		yMove = rand() % -5;
	if (obszar)
	{
		xPoz += xMove;
		yPoz += yMove;
	}
	else
	{
		xPoz -= xMove;
		yPoz -= yMove;
	}
}
// ###################################
// ##### Funkcja od komunikatow  #####
// ###################################
static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
{
	static list<Gwiazdka>Pojemnik;
	PAINTSTRUCT PS;
	RECT Rect;
	GetClientRect(Okno, &Rect);

	switch (Komunikat) {
	case WM_CREATE: // Podczas tworzenia okna rysuje 10 gwiazdek
	{
		SetTimer(Okno, 1, 100, 0);
		for (int i = 0; i != 10; ++i)
		{
			Gwiazdka k(rand() % 20, rand() % Rect.right, rand() % Rect.bottom, RGB(rand() % 255, rand() % 255, 0), RGB(rand() % 255, 15 * i, 0));
			Pojemnik.push_back(k);
		}
	}
	break;
	case WM_PAINT: // Tutaj rysuje kazda gwiazdke pokolei
	{
		BeginPaint(Okno, &PS);
		for (list<Gwiazdka>::iterator it = Pojemnik.begin(), itend = Pojemnik.end(); it != itend; ++it)
		{
			it->Rysuj(PS.hdc);
		}
		EndPaint(Okno, &PS);
	}
	break;
	case WM_TIMER: // Tutaj porusza gwiazdki w zaleznosci od wielkiesci obszaru klienckiego
		for (list<Gwiazdka>::iterator it = Pojemnik.begin(), itend = Pojemnik.end(); it != itend; ++it)
		{
			if (Rect.left < it->GetX() && Rect.top < it->GetY()) it->Przesun(0);
			if (Rect.right > it->GetX() && Rect.bottom > it->GetY()) it->Przesun(1);
		}
		InvalidateRect(Okno, 0, 1);
		break;
	case WM_DESTROY:
		KillTimer(Okno, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(Okno, Komunikat, wParam, lParam);
	}
	return 0;
}

static bool RejestrujKlasy()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 128));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = &FunkcjaOkienkowa;
	wc.lpszClassName = NazwaKlasy;
	wc.lpszMenuName = NULL;
	wc.style = 0;
	return (RegisterClassEx(&wc) != 0);
}

static void WyrejestrujKlasy()
{
	UnregisterClass(NazwaKlasy, GetModuleHandle(NULL));
}

int WINAPI WinMain(HINSTANCE Instancja, HINSTANCE Poprzednia, LPSTR Parametry, int Widocznosc)
{
	// Zarejestruj klas?. Protestuj, je?eli wyst?pi? b??d.
	if (!RejestrujKlasy()) {
		MessageBox(NULL, TEXT("Nie uda?o si? zarejestrowa? klasy okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 1;
	}
	// Stw�rz g?�wne okno. R�wnie? protestuj, je?eli wyst?pi? b??d.
	HWND GlowneOkno = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE,
		NazwaKlasy, TEXT("Okno"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, Instancja, NULL);
	if (GlowneOkno == NULL) {
		MessageBox(NULL, TEXT("Nie uda?o si? stworzy? g?�wnego okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 2;
	}
	// Wy?wietl i uaktualnij nowo stworzone okno.
	ShowWindow(GlowneOkno, Widocznosc);
	UpdateWindow(GlowneOkno);
	// G?�wna p?tla komunikat�w w?tku.
	MSG Komunikat;
	while (GetMessage(&Komunikat, NULL, 0, 0) > 0) {
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	// Zwolnij pami?? klas i zako?cz proces.
	WyrejestrujKlasy();
	return static_cast<int>(Komunikat.wParam);
}

