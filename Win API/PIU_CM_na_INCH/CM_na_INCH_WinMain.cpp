#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <Strsafe.h>

#define ID_RBUTTON1 101
#define ID_RBUTTON2 102
#define ID_TFIELD1 201
#define ID_TFIELD2 202


#define H_RBUTTON 30
#define W_RBUTTON 100
#define H_TFIELD 30
#define W_TFIELD 100

TCHAR NazwaAplikacji[] = TEXT("Aplikacja studencka");
TCHAR NazwaKlasy[] = TEXT("OKNOGLOWNE");
RECT rect;
HFONT hNormalFont;
HWND rButton1 = NULL, rButton2 = NULL, tField1 = NULL , tField2 = NULL;
bool CMtoINCH = false, INCHtoCM = FALSE;




float CM_to_INCH(float cm)
{
	return cm / 2.54;
}
float INCH_to_CM(float inch)
{
	return inch * 2.54;
}
// ##### Funkcja od komunikatow  #####
static LRESULT CALLBACK FunkcjaOkienkowa(HWND Okno, UINT Komunikat, WPARAM wParam, LPARAM lParam)
{
	GetClientRect(Okno, &rect);

	int xB = rect.right / 2 - 50, yB = rect.bottom / 2;

	switch (Komunikat) {
	case WM_CREATE:
	{
		hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		MoveWindow(rButton1, xB, yB, W_RBUTTON, H_RBUTTON, true);
		MoveWindow(rButton2, xB, yB + H_RBUTTON, W_RBUTTON, H_RBUTTON, true);
		MoveWindow(tField1, xB, yB - H_RBUTTON, W_TFIELD, H_TFIELD, true);
		MoveWindow(tField2, xB, yB + 2*H_RBUTTON, W_TFIELD, H_TFIELD, true);

	}
	break;
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_RBUTTON1:
		{
			CMtoINCH = true;
			INCHtoCM = false;
		}
		break;
		case ID_RBUTTON2:
		{
			CMtoINCH = false;
			INCHtoCM = true;
		}
		break;
		}

		static TCHAR liczba[64], wynik[64];
		//GetWindowText(tField1, liczba, 64);				// pobiera napis z tField1
		float Liczba = GetDlgItemInt(Okno, ID_TFIELD1, NULL, FALSE);
		float wynikF = 0;

		if (CMtoINCH)
			wynikF = CM_to_INCH(Liczba);
		else
			if (INCHtoCM)
				wynikF = INCH_to_CM(Liczba);
		//swprintf(wynik, TEXT("Rozmiar okna %d x %d"), LOWORD(lParam), HIWORD(lParam));

		StringCchPrintf(wynik, 64, TEXT("%f"), wynikF);

		SetWindowText(tField2, wynik);
		/*HDC hdc;
		hdc = GetDC(Okno);
		TextOut(hdc, xB, yB + 2*H_RBUTTON, tekst, 10);
		ReleaseDC(Okno, hdc);*/
	}
	break;
	case WM_PAINT:
	{

	}
	break;
	case WM_SIZE:
		MoveWindow(rButton1, xB, yB, W_RBUTTON, H_RBUTTON, true);
		MoveWindow(rButton2, xB, yB + H_RBUTTON, W_RBUTTON, H_RBUTTON, true);
		MoveWindow(tField1, xB, yB - H_RBUTTON, W_TFIELD, H_TFIELD, true);
		MoveWindow(tField2, xB, yB + 2 * H_RBUTTON, W_TFIELD, H_TFIELD, true);



		InvalidateRect(Okno, 0, 1);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(Okno);
		DestroyWindow(rButton1);
		DestroyWindow(rButton2);
		DestroyWindow(tField1);
		DestroyWindow(tField2);
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
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(0x2c2c2c);
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
	HWND GlowneOkno = CreateWindowEx(WS_EX_WINDOWEDGE,	// rozszerzone parametry stylu okna
		NazwaKlasy,										// nazwa klasy okna
		TEXT("Okno"),									// naglowek okna
		WS_OVERLAPPEDWINDOW,							// styl okna
		CW_USEDEFAULT,									// poczatkowa pozycja x
		CW_USEDEFAULT,									// poczatkowa pozycja y
		CW_USEDEFAULT,									// poczatkowa wielkosc x
		CW_USEDEFAULT,									// poczatkowa wielkosc y
		NULL,											// uchwyt okna nadrzednego
		NULL,											// uchwyt menu okna
		Instancja,										// uchwyt kopii programu
		NULL);											// parametry tworzenia



	if (GlowneOkno == NULL) {
		MessageBox(NULL, TEXT("Nie uda�o si� stworzy� g��wnego okna!"),
			NazwaAplikacji, MB_ICONSTOP | MB_OK);
		return 2;
	}

	GetClientRect(GlowneOkno, &rect);
	int xB = rect.right / 2 - 50, yB = rect.bottom / 2;


	rButton1 = CreateWindowEx(NULL,						// rozszerzone parametry stylu okna
		TEXT("BUTTON"),									// nazwa kontrolki
		TEXT("CM na INCH"),								// nazwa wyswietlana na przycisku
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,			// styl okna, kontrola potomna i widoczna
		xB,												// poczatkowa pozycja x
		yB,												// poczatkowa pozycja y
		W_RBUTTON,										// poczatkowa wielkosc x
		H_RBUTTON,										// poczatkowa wielkosc y
		GlowneOkno,										// uchwyt okna nadrzednego
		(HMENU)ID_RBUTTON1,								// uchwyt menu okna
		Instancja,										// uchwyt kopii programu
		NULL);											// parametry tworzenia	

	SendMessage(rButton1, WM_SETFONT, (WPARAM)hNormalFont, 0); // czcionka dla rButtona

	if (rButton1 == NULL)
		MessageBox(NULL, TEXT("Nie stworzono rButton1 "), NazwaAplikacji, MB_ICONEXCLAMATION | MB_OK);

	rButton2 = CreateWindowEx(NULL,						// rozszerzone parametry stylu okna
		TEXT("BUTTON"),									// nazwa kontrolki
		TEXT("INCH na CM"),								// nazwa wyswietlana na przycisku
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,			// styl okna, kontrola potomna i widoczna
		xB,												// poczatkowa pozycja x
		yB - H_RBUTTON,									// poczatkowa pozycja y
		W_RBUTTON,										// poczatkowa wielkosc x
		H_RBUTTON,										// poczatkowa wielkosc y
		GlowneOkno,										// uchwyt okna nadrzednego
		(HMENU)ID_RBUTTON2,								// uchwyt menu okna
		Instancja,										// uchwyt kopii programu
		NULL);											// parametry tworzenia	

	SendMessage(rButton2, WM_SETFONT, (WPARAM)hNormalFont, 0); // czcionka dla rButtona

	if (rButton2 == NULL)
		MessageBox(NULL, TEXT("Nie stworzono rButton2 "), NazwaAplikacji, MB_ICONEXCLAMATION | MB_OK);

	tField1 = CreateWindowEx(NULL,						// rozszerzone parametry stylu okna
		TEXT("edit"),									// nazwa kontrolki
		TEXT(""),										// nazwa wyswietlana na przycisku
		WS_CHILD | WS_VISIBLE,							// styl okna, kontrola potomna i widoczna
		xB,												// poczatkowa pozycja x
		yB + H_RBUTTON,									// poczatkowa pozycja y
		W_TFIELD,										// poczatkowa wielkosc x
		H_TFIELD,										// poczatkowa wielkosc y
		GlowneOkno,										// uchwyt okna nadrzednego
		(HMENU)ID_TFIELD1,								// uchwyt menu okna
		Instancja,										// uchwyt kopii programu
		NULL);											// parametry tworzenia	

	SendMessage(tField1, WM_SETFONT, (WPARAM)hNormalFont, 0); // czcionka dla rButtona

	if (tField1 == NULL)
		MessageBox(NULL, TEXT("Nie stworzono tField1 "), NazwaAplikacji, MB_ICONEXCLAMATION | MB_OK);

	tField2 = CreateWindowEx(NULL,						// rozszerzone parametry stylu okna
		TEXT("static"),									// nazwa kontrolki
		TEXT("Wynik"),										// nazwa wyswietlana na przycisku
		WS_CHILD | WS_VISIBLE,							// styl okna, kontrola potomna i widoczna
		xB,												// poczatkowa pozycja x
		yB - 2*H_RBUTTON,									// poczatkowa pozycja y
		W_TFIELD,										// poczatkowa wielkosc x
		H_TFIELD,										// poczatkowa wielkosc y
		GlowneOkno,										// uchwyt okna nadrzednego
		(HMENU)ID_TFIELD2,								// uchwyt menu okna
		Instancja,										// uchwyt kopii programu
		NULL);											// parametry tworzenia	

	SendMessage(tField2, WM_SETFONT, (WPARAM)hNormalFont, 0); // czcionka dla rButtona

	if (tField2 == NULL)
		MessageBox(NULL, TEXT("Nie stworzono tField1 "), NazwaAplikacji, MB_ICONEXCLAMATION | MB_OK);



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