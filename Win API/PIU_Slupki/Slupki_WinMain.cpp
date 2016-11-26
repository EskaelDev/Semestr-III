/************************************************************
* -- NIE USUWAJ TEJ INFORMACJI Z PROGRAMU ---------------- *
************************************************************
* -- Program powsta� na bazie kodu �r�d�owego ------------ *
* -- udost�pnionego studentom na potrzeby przedmiotu ----- *
* -- Programowanie Interfejsu U�ytkownika ---------------- *
* -- Copyright (c) 2009 Politechnika �l�ska w Gliwicach -- *
* -- Rados�aw Sok�, Wydzia� Elektryczny ----------------- *
************************************************************/

#include <windows.h>
#include <list>
#include <stdio.h>

TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");
// ###################################
// ##### Klasa 'Slupki' ##############
// ###################################
class Slupki
{
private:
	int dlugosc,
		polozenie,
		wielkosc,
		id;
	static int	skala,
		ilosc;
	bool zaznaczone;
public:
	Slupki(RECT Rect);
	~Slupki() { --ilosc; };
	void Maluj(HDC Kontekst);
	void Polozenie(RECT &Rect);
	void CzyKliknieto(int x, int y);
	RECT Przesun(int x, RECT Rect);

	void Odznacz() { zaznaczone = false; };
	bool CzyZaznaczone() { return zaznaczone; };
};
// ##### Definicja zmiennych statycznych #####
int Slupki::ilosc;
int Slupki::skala;
// ##### Konstruktor parametryczny #####
Slupki::Slupki(RECT Rect)
{
	ilosc++;
	id = ilosc;
	dlugosc = 50;
	zaznaczone = false;
	Polozenie(Rect);
}
// ##### Prosta funkcja odpowiedzialna za rysowanie #####
void Slupki::Maluj(HDC Kontekst)
{
	TCHAR Buf[4];
	swprintf(Buf, L"%i", dlugosc);
	TextOut(Kontekst, wielkosc + 15, polozenie, Buf, wcslen(Buf));
	Rectangle(Kontekst, 10, polozenie - skala, wielkosc, polozenie + skala); // Rysuje slupek
	Rectangle(Kontekst, wielkosc - 10, polozenie - skala - 2, wielkosc + 10, polozenie + skala + 2); // Rysuje wyszczeg�lniony obszar

}
// ##### Funkcja ustawia grubosc slupka, jego wielkosc(dlugosc) oraz polozenie konkretnego slupka #####
void Slupki::Polozenie(RECT &Rect)
{
	wielkosc = Rect.right / 105 * dlugosc + 15;
	skala = Rect.bottom / ilosc / 3.6;
	polozenie = Rect.bottom / (ilosc + 1) * id;
}
// ##### Funkcja odpowiedzialna za ustawienie dlugosc slupka za pomoca myszki #####
RECT Slupki::Przesun(int x, RECT Rect)
{
	if (x > 0 && x < Rect.right) dlugosc = x / (Rect.right / 105);
	if (dlugosc >= 100) dlugosc = 100;
	if (dlugosc <= 0) dlugosc = 0;
	Rect.bottom = polozenie - skala - 5;
	Rect.top = polozenie + skala + 5;
	Rect.left = wielkosc - 30;
	Rect.right = wielkosc + 40;
	return Rect;
}
// ##### Funkcja sprawdza czy kliknieto w obszar odpowiedzialny za przesuwanie #####
void Slupki::CzyKliknieto(int x, int y)
{
	if (x > wielkosc - 10 && y > polozenie - skala - 2 && x < wielkosc + 10 && y < polozenie + skala + 2) zaznaczone = true;
	else zaznaczone = false;
}
// ##### Funkcja od komunikatow  #####
static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
{
	static std::list<Slupki> Pamiec;
	Slupki *dodaj;
	RECT Rect;
	PAINTSTRUCT PS;
	HDC hdc;
	GetClientRect(Okno, &Rect);
	switch (Komunikat) {
	case WM_CREATE:
		dodaj = new Slupki(Rect);
		Pamiec.push_back(*dodaj);
		CreateWindowEx(0, L"BUTTON", L"Dodaj", WS_CHILD + WS_VISIBLE, 10, 10, 50, 20, Okno, (HMENU)1, GetModuleHandle(0), 0);
		CreateWindowEx(0, L"BUTTON", L"Usu�", WS_CHILD + WS_VISIBLE, 65, 10, 50, 20, Okno, (HMENU)2, GetModuleHandle(0), 0);
		HBRUSH	Pedzel, Pedzel2, Stary;
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
			dodaj = new Slupki(Rect);
			Pamiec.push_back(*dodaj);
			break;
		case 2:
			if (!Pamiec.empty()) Pamiec.pop_back();
			break;
		}
		InvalidateRect(Okno, 0, 1);
		break;
	case WM_PAINT:
		hdc = BeginPaint(Okno, &PS);
		for (std::list<Slupki>::iterator it = Pamiec.begin(); it != Pamiec.end(); ++it)
		{

			Pedzel = CreateSolidBrush(0xFF8010);
			Pedzel2 = CreateSolidBrush(0xFF1010);
			Stary = (HBRUSH)SelectObject(hdc, Pedzel);
			it->Polozenie(Rect);
			it->Maluj(PS.hdc);
			DeleteObject(Pedzel);
			DeleteObject(Pedzel2);
			DeleteObject(Stary);
		}
		EndPaint(Okno, &PS);
		break;
	case WM_SIZE:
		InvalidateRect(Okno, 0, 1);
		break;
	case WM_LBUTTONDOWN:
		SetCapture(Okno);
		for (std::list<Slupki>::iterator it = Pamiec.begin(); it != Pamiec.end(); ++it)
		{
			it->CzyKliknieto(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case WM_LBUTTONUP:
		for (std::list<Slupki>::iterator it = Pamiec.begin(); it != Pamiec.end(); ++it)
		{
			it->Odznacz();
		}
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		for (std::list<Slupki>::iterator it = Pamiec.begin(); it != Pamiec.end(); ++it)
		{
			if (it->CzyZaznaczone())
			{
				Rect = it->Przesun(LOWORD(lParam) - 10, Rect);
				InvalidateRect(Okno, &Rect, 1);
				break;
			}
		}
		break;
	case WM_DESTROY:
		DeleteObject(Pedzel);
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
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(0xFFFFFF);
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
	// Zarejestruj klas�. Protestuj, je�eli wyst�pi� b��d.
	if (!RejestrujKlasy()) {
		MessageBox(NULL, TEXT("Nie uda�o si� zarejestrowa� klasy okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 1;
	}
	// Stw�rz g��wne okno. R�wnie� protestuj, je�eli wyst�pi� b��d.
	HWND GlowneOkno = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE,
		NazwaKlasy, TEXT("Okno"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, Instancja, NULL);
	if (GlowneOkno == NULL) {
		MessageBox(NULL, TEXT("Nie uda�o si� stworzy� g��wnego okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 2;
	}
	// Wy�wietl i uaktualnij nowo stworzone okno.
	ShowWindow(GlowneOkno, Widocznosc);
	UpdateWindow(GlowneOkno);
	// G��wna p�tla komunikat�w w�tku.
	MSG Komunikat;
	while (GetMessage(&Komunikat, NULL, 0, 0) > 0) {
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	// Zwolnij pami�� klas i zako�cz proces.
	WyrejestrujKlasy();
	return static_cast<int>(Komunikat.wParam);
}