#include <windows.h>

TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");

class Figura
{
public:
	virtual void Wyswietl(HDC hdc) = 0;
};

class Trojkat : public Figura
{
	POINT Punkty[3];//punkty 
	COLORREF Ramka;//kolor linii
	int Grubosc;//grubosc obramowania
	COLORREF Wypelnienie;//kolor wypelnienia
	bool Wypelnic;//czy ma byc wypelniony

public:
	//zwracanie poszczegolnych wartosci
	POINT ReturnPoint(int Ktory) { return Punkty[Ktory]; }
	COLORREF ReturnRamka() { return Ramka; }
	int ReturnGrubosc() { return Grubosc; }
	COLORREF ReturnWypelnienie() { return Wypelnienie; }
	bool ReturnWypelnic() { return Wypelnic; }

	Trojkat(POINT pynkty[3], COLORREF ramka, int grubosc, COLORREF wypelnienie, bool wypelnic);//konstruktor parametryczny
	Trojkat(Trojkat &T);//konstruktor kopiujacy
	void Wyswietl(HDC hdc);
};

void Trojkat::Wyswietl(HDC hdc)
{


	if (Wypelnic == TRUE) {
		HBRUSH StaryPedzel = (HBRUSH)SelectObject(hdc,
			CreateSolidBrush(Wypelnienie));
	}
	HPEN Pioro1 = CreatePen(PS_SOLID, Grubosc, Ramka);
	SelectObject(hdc, Pioro1);
	Polygon(hdc, Punkty, 3);

};


Trojkat::Trojkat(POINT punkty[3], COLORREF ramka, int grubosc, COLORREF wypelnienie, bool wypelnic)
{
	Punkty[0] = punkty[0];
	Punkty[1] = punkty[1];
	Punkty[2] = punkty[2];
	Ramka = ramka;
	Grubosc = grubosc;
	Wypelnienie = wypelnienie;
	Wypelnic = wypelnic;
}

Trojkat::Trojkat(Trojkat &T)
{
	Punkty[0] = T.ReturnPoint(0);
	Punkty[1] = T.ReturnPoint(1);
	Punkty[2] = T.ReturnPoint(2);
	Ramka = T.ReturnRamka();
	Grubosc = T.ReturnGrubosc();
	Wypelnienie = T.ReturnWypelnienie();
	Wypelnic = T.ReturnWypelnic();
}



class Elipsa : public Figura
{
	int Left, Right, Top, Bottom;//promienie
	COLORREF Ramka;//kolor linii
	int Grubosc;//grubosc obramowania
	COLORREF Wypelnienie;//kolor wypelnienia
	bool Wypelnic;//czy ma byc wypelniony

public:
	//zwracanie poszczegolnych wartosci
	int ReturnLeft() { return Left; }
	int ReturnRight() { return Right; }
	int ReturnTop() { return Top; }
	int ReturnBottom() { return Bottom; }

	COLORREF ReturnRamka() { return Ramka; }
	int ReturnGrubosc() { return Grubosc; }
	COLORREF ReturnWypelnienie() { return Wypelnienie; }
	bool ReturnWypelnic() { return Wypelnic; }

	Elipsa(int left, int right, int top, int bottom, COLORREF ramka, //konstruktor parametryczny
		int grubosc, COLORREF wypelnienie, bool wypelnic);

	Elipsa(Elipsa &E);//konstruktor kopiujacy

	void Wyswietl(HDC hdc);

};


Elipsa::Elipsa(Elipsa &E)
{

	Right = E.ReturnRight();
	Top = E.ReturnTop();
	Bottom = E.ReturnBottom();
	Ramka = E.ReturnRamka();
	Grubosc = E.ReturnGrubosc();
	Wypelnienie = E.ReturnWypelnienie();
	Wypelnic = E.ReturnWypelnic();
}

Elipsa::Elipsa(int left, int right, int top, int bottom, COLORREF ramka,
	int grubosc, COLORREF wypelnienie, bool wypelnic)
{
	Left = left;
	Right = right;
	Top = top;
	Bottom = bottom;
	Ramka = ramka;
	Grubosc = grubosc;
	Wypelnienie = wypelnienie;
	Wypelnic = wypelnic;
}


void Elipsa::Wyswietl(HDC hdc)
{
	RECT R;

	if (Wypelnic == TRUE) {
		HBRUSH StaryPedzel = (HBRUSH)SelectObject(hdc,
			CreateSolidBrush(Wypelnienie));
	}
	HPEN Pioro1 = CreatePen(PS_SOLID, Grubosc, Ramka);
	SelectObject(hdc, Pioro1);
	Ellipse(hdc, Left, Top, Right, Bottom);

};





static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
{
	int wX, wY;//rozmiar okna klienckiego
	HDC hdc;
	PAINTSTRUCT  ps;
	hdc = GetDC(Okno);

	POINT P[] = { { 10,100 },{ 50,50 },{ 100,100 } };

	Trojkat T1(P, RGB(255, 0, 0), 1, RGB(0, 255, 0), FALSE);
	Elipsa E1(150, 300, 200, 100, RGB(0, 0, 200), 2, RGB(0, 0, 0), TRUE);

	switch (Komunikat) {
	case WM_SIZE://zmiana rozmiaru
		wX = LOWORD(lParam);
		wY = HIWORD(lParam);
	case WM_PAINT:
		hdc = BeginPaint(Okno, &ps);
		T1.Wyswietl(hdc);
		E1.Wyswietl(hdc);

		EndPaint(Okno, &ps);
		break;
	case WM_DESTROY:
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
	wc.hbrBackground = (HBRUSH)(1 + COLOR_WINDOW);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = &FunkcjaOkienkowa;
	wc.lpszClassName = NazwaKlasy;
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
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
	// Stwórz g?ówne okno. Równie? protestuj, je?eli wyst?pi? b??d.


	HWND GlowneOkno = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE,
		NazwaKlasy, TEXT("Okno"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
		NULL, NULL, Instancja, NULL);
	if (GlowneOkno == NULL) {
		MessageBox(NULL, TEXT("Nie uda?o si? stworzy? g?ównego okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 2;
	}
	// Wy?wietl i uaktualnij nowo stworzone okno.
	ShowWindow(GlowneOkno, Widocznosc);
	UpdateWindow(GlowneOkno);
	// G?ówna p?tla komunikatów w?tku.
	MSG Komunikat;
	while (GetMessage(&Komunikat, NULL, 0, 0) > 0) {
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	// Zwolnij pami?? klas i zako?cz proces.
	WyrejestrujKlasy();
	return static_cast<int>(Komunikat.wParam);
}

