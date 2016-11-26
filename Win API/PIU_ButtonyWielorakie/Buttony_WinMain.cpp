#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>
#include <tchar.h>

#pragma warning (disable : 4996)

// ----------------------- DEKLARACJE FUNKCJI ---------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int $RegisterClass(WNDCLASSEX wC, HINSTANCE hInstance);
static void $Draw(const HWND Window);												// Wyœwietla rozmiar okna

// ----------------------- DEKLARACJE ZMIENNYCH GLOBALNYCH --------------------------------------------------------------------------

#define ID_BUTTON1		101 
#define ID_CHECKBOX2	102 
#define ID_RADIOBUTTON3	103 
#define ID_GROUPBOX4	104
#define ID_RADIOBUTTON5	105
#define ID_RADIOBUTTON6	106
#define ID_RADIOBUTTON7	107
#define ID_TEXTBOX1		201 
#define ID_TEXTBOX2		202 
#define ID_TEXTBOX3		203 
#define ID_TEXTBOX4		204
#define ID_LISTBOX		301
#define ID_COMBOBOX1	401
#define ID_COMBOBOX2	402
#define ID_SCROLLBAR1	501
#define ID_SCROLLBAR2	502
#define ID_STATIC1		601
#define ID_STATIC2		602 
#define ID_STATIC3		603 
#define ID_STATIC4		604 
#define ID_STATIC5		605

TCHAR $ClassName[] = TEXT("Nazwa_klasy"), $AppName[] = TEXT("Nazwa_okna");
RECT rect;											// Definiuje wspolrzedne lewego gornego i prawego dolnego naroznika prostokata.
HFONT hNormalFont;									// Uchwyt do zwyklej czcionki.
HWND button1 = NULL, button2 = NULL, button3 = NULL, button4 = NULL;					// Uchwyt do przyciskow.
HWND textBox1 = NULL, textBox2 = NULL, textBox3 = NULL, textBox4 = NULL;				// Uchwyt do pola tekstowego.
HWND ListBox1 = NULL;																	// Uchwyt do listy.
HWND ComboBox1 = NULL, ComboBox2 = NULL;												// Uchwyt do listy rozwijalnej.
HWND ScrollBar1 = NULL, ScrollBar2 = NULL;												// Uchwyt do paska przewijania.
HWND Static1 = NULL, Static2 = NULL, Static3 = NULL, Static4 = NULL, Static5 = NULL;	// Uchwyt do kontrolki statycznej.
HWND RadioButton5 = NULL, RadioButton6 = NULL, RadioButton7 = NULL;
// ----------------------------------------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance,					// uchwyt naszej aplikacji
	HINSTANCE hPrevInstance,							// uchwyt poprzedniego wystapienia aplikacji
	LPSTR pStr,											// linia polecen, z jakiej zostal uruchomiony program
	int nCmdShow)										// stan okna programu
{
	WNDCLASSEX wC = { 0 };								// wc - window class (jak nie ma zera to blad rejestracji klasy)
	HWND hwnd = NULL;									// hWnd - hinstance Window
	MSG msg;											// msg - message

	int RegisterResult = $RegisterClass(wC, hInstance);
	if (RegisterResult == -1)
	{
		MessageBox(NULL, TEXT("Nie zarejestrowano klasy okna. Nastapi wylaczenie programu."), $AppName, MB_ICONERROR | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_WINDOWEDGE,				// rozszerzone parametry stylu okna
		$ClassName,										// nazwa klasy okna
		$AppName,										// naglowek okna
		WS_OVERLAPPEDWINDOW,							// styl okna
		CW_USEDEFAULT,									// poczatkowa pozycja x
		CW_USEDEFAULT,									// poczatkowa pozycja y
		CW_USEDEFAULT,									// poczatkowa wielkosc x
		CW_USEDEFAULT,									// poczatkowa wielkosc y
		NULL,											// uchwyt okna nadrzednego
		NULL,											// uchwyt menu okna
		hInstance,										// uchwyt kopii programu
		NULL);											// parametry tworzenia

	if (hwnd == NULL)
	{
		if (!RegisterResult)
		{
			MessageBox(NULL, TEXT("Nie uda³o sie utworzyæ okna. Klasa okna zostanie wyrejestrowana"), $AppName, MB_ICONEXCLAMATION | MB_OK);
			UnregisterClass($ClassName, hInstance);
			return -1;
		}
		return -1;
	}

	button1 = CreateWindowEx(NULL,			// rozszerzone parametry stylu okna
		TEXT("BUTTON"),						// nazwa kontrolki
		TEXT("Przycisk 1"),					// nazwa wyswietlana na przycisku
		WS_CHILD | WS_VISIBLE,				// styl okna, kontrola potomna i widoczna
		200,								// poczatkowa pozycja x
		0,									// poczatkowa pozycja y
		150,								// poczatkowa wielkosc x
		30,									// poczatkowa wielkosc y
		hwnd,								// uchwyt okna nadrzednego
		(HMENU)ID_BUTTON1,					// uchwyt menu okna
		hInstance,							// uchwyt kopii programu
		NULL);								// parametry tworzenia

	SendMessage(button1, WM_SETFONT, (WPARAM)hNormalFont, 0);				// zmiana czcionki dla button1

	if (button1 == NULL)
	{
		MessageBox(NULL, TEXT("Nie udalo sie utworzyc przycisku button 1."), $AppName, MB_ICONEXCLAMATION | MB_OK);
	}

	button2 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Checkbox"), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 200, 40, 150, 30, hwnd,
		(HMENU)ID_CHECKBOX2, hInstance, NULL);
	button3 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Radiobutton"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 200, 80, 150, 30,
		hwnd, (HMENU)ID_RADIOBUTTON3, hInstance, NULL);
	button4 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Groupbox"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 200, 120, 150, 100,
		hwnd, (HMENU)ID_GROUPBOX4, hInstance, NULL);

	textBox1 = CreateWindowEx(NULL, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 230, 150, 100, hwnd,
		(HMENU)ID_TEXTBOX1, hInstance, NULL);
	textBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 340, 150, 100, hwnd,
		(HMENU)ID_TEXTBOX2, hInstance, NULL);
	textBox3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 200, 450, 150,
		100, hwnd, (HMENU)ID_TEXTBOX3, hInstance, NULL);
	textBox4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL |
		ES_AUTOVSCROLL, 360, 0, 150, 100, hwnd, (HMENU)ID_TEXTBOX4, hInstance, NULL);

	SendMessage(textBox1, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SetWindowText(textBox1, TEXT("WS_BORDER"));
	SetWindowText(textBox2, TEXT("WS_EX_CLIENTEDGE"));
	SetWindowText(textBox3, TEXT("ES_MULTILINE"));
	SetWindowText(textBox4, TEXT("WS_VSCROLL\nES_AUTOVSCROLL"));

	ListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 360, 110, 150, 100, hwnd,
		(HMENU)ID_LISTBOX, hInstance, NULL);

	SendMessage(ListBox1, LB_ADDSTRING, 0, (LPARAM)TEXT("ListBox 1"));
	SendMessage(ListBox1, LB_ADDSTRING, 0, (LPARAM)TEXT("Element 2"));

	ComboBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 360, 210,
		150, 100, hwnd, (HMENU)ID_COMBOBOX1, hInstance, NULL);

	SendMessage(ComboBox1, CB_ADDSTRING, 0, (LPARAM)TEXT("ComboBox 1"));
	SendMessage(ComboBox1, CB_ADDSTRING, 0, (LPARAM)TEXT("Element 2"));

	ComboBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST, 360,
		250, 150, 100, hwnd, (HMENU)ID_COMBOBOX2, hInstance, NULL);

	SendMessage(ComboBox2, CB_ADDSTRING, 0, (LPARAM)TEXT("ComboBox 2"));
	SendMessage(ComboBox2, CB_ADDSTRING, 0, (LPARAM)TEXT("Element 2"));

	ScrollBar1 = CreateWindowEx(0, TEXT("SCROLLBAR"), NULL, WS_CHILD | WS_VISIBLE, 360, 290, 150, 20, hwnd, (HMENU)ID_SCROLLBAR1,
		hInstance, NULL);
	ScrollBar2 = CreateWindowEx(0, TEXT("SCROLLBAR"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SBS_VERT, 360, 320, 20, 150, hwnd,
		(HMENU)ID_SCROLLBAR2, hInstance, NULL);

	Static1 = CreateWindowEx(0, TEXT("Static"), NULL, WS_CHILD | WS_VISIBLE, 400, 320, 110, 150, hwnd, (HMENU)ID_STATIC1,
		hInstance, NULL);

	SetWindowText(Static1, TEXT("********************* Pole statyczne bez ramki"));

	Static2 = CreateWindowEx(0, TEXT("Static"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 360, 490, 150, 60, hwnd, (HMENU)ID_STATIC2,
		hInstance, NULL);

	SetWindowText(Static2, TEXT("**************************** Pole statyczne\nz ramk¹"));

	Static3 = CreateWindowEx(0, TEXT("Static"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 520, 0, 150, 60, hwnd,
		(HMENU)ID_STATIC3, hInstance, NULL);

	SetWindowText(Static3, TEXT("**************************** Pole statyczne ze stylem SS_LEFT"));

	Static4 = CreateWindowEx(0, TEXT("Static"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, 520, 70, 150, 60, hwnd,
		(HMENU)ID_STATIC4, hInstance, NULL);

	SetWindowText(Static4, TEXT("**************************** Pole statyczne ze stylem SS_CENTER"));

	Static5 = CreateWindowEx(0, TEXT("Static"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | SS_RIGHT, 520, 140, 150, 60, hwnd,
		(HMENU)ID_STATIC5, hInstance, NULL);

	SetWindowText(Static5, TEXT("**************************** Pole statyczne ze stylem SS_RIGHT"));

	RadioButton5 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Radiobutton"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 680, 0, 150, 30,
		hwnd, (HMENU)ID_RADIOBUTTON5, hInstance, NULL);
	RadioButton6 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Radiobutton"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 680, 30, 150, 30,
		hwnd, (HMENU)ID_RADIOBUTTON6, hInstance, NULL);
	RadioButton7 = CreateWindowEx(NULL, TEXT("BUTTON"), TEXT("Radiobutton"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 680, 60, 150, 30,
		hwnd, (HMENU)ID_RADIOBUTTON7, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);		// wyswietlenie okna
	UpdateWindow(hwnd);				// wymusza aktualizacje (odswiezenie) okna

	while (GetMessage(&msg, NULL, 0, 0))		// GetMessage - pobiera komunikat z kolejki komunikatow
	{
		TranslateMessage(&msg);					// Tlumaczy niektore komunikaty wysylane z klawiatury
		DispatchMessage(&msg);					// Wysyla komunikat do procedury okna
	}

	if (RegisterResult)
	{
		UnregisterClass($ClassName, hInstance);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:;
		hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		break;

	case WM_PAINT:;
		$Draw(hwnd);
		break;

	case WM_MOVE:;
		InvalidateRect(hwnd, &rect, FALSE); // Odswieza okreslony obszar okna, TRUE - tl	o jest usuwane podczas wywolywania funkcji
		break;

	case WM_COMMAND:;
		if (HIWORD(wParam) == CBN_SELCHANGE)		// jesli zmieniono stan ComboBoxa
		{
			if ((HWND)lParam == ComboBox1)
			{
				UINT indexComboBox = ComboBox_GetCurSel(ComboBox1);			// #include <Windowsx.h>
				TCHAR caption[30];
				swprintf(caption, sizeof(caption), TEXT("Wybrano pozycje o indeksie: %i"), indexComboBox);

				MessageBox(hwnd, caption, $AppName, MB_ICONINFORMATION);				// wyswietlenie wybranego indeksu
				SendMessage(ComboBox1, CB_SETCURSEL, (WPARAM)indexComboBox, 0);			// ustawienie ComboBoxa

				UINT lengthComboBox = ComboBox_GetTextLength(ComboBox1);
				LPSTR captionComboBox = (LPSTR)GlobalAlloc(GPTR, lengthComboBox + 1);		// alokacja pamieci

				//ComboBox_GetText(ComboBox1, captionComboBox, lengthComboBox + 1);	// pobranie wybranego tekstu

//				MessageBox(hwnd, captionComboBox, $AppName, MB_ICONINFORMATION);	// wyswietlenie wybranego tekstu

//				GlobalFree(captionComboBox); captionComboBox = NULL;		// zwolnienie pamieci
			}
		}
		else
			switch (wParam)
			{
			case ID_BUTTON1:
				MessageBox(hwnd, TEXT("Nacisnieto przycisk 'Button'"), $AppName, MB_ICONINFORMATION);

				static DWORD dlugosc;
				dlugosc = GetWindowTextLength(textBox4);				// pobiera dlugosc napisu w polu tekstowym 1

				static TCHAR liczba[44];
				swprintf(liczba, 44, TEXT("Liczba znakow w polu tekstowym 4 wynosi: %i"), dlugosc);

				MessageBox(hwnd, liczba, $AppName, MB_ICONINFORMATION);
				break;

			case ID_CHECKBOX2:
				if (IsDlgButtonChecked(hwnd, ID_CHECKBOX2) == BST_CHECKED)
					CheckDlgButton(hwnd, ID_CHECKBOX2, BST_UNCHECKED);
				else
					CheckDlgButton(hwnd, ID_CHECKBOX2, BST_CHECKED);

				MessageBox(hwnd, TEXT("Nacisnieto przycisk 'Checkbox'"), $AppName, MB_ICONINFORMATION);
				break;

			case ID_RADIOBUTTON3:
				MessageBox(hwnd, TEXT("Nacisnieto przycisk 'Radiobutton'"), $AppName, MB_ICONINFORMATION);
				break;

			case ID_RADIOBUTTON5:
				CheckRadioButton(hwnd, ID_RADIOBUTTON5, ID_RADIOBUTTON7, ID_RADIOBUTTON5);
				break;

			case ID_RADIOBUTTON6:
				CheckRadioButton(hwnd, ID_RADIOBUTTON5, ID_RADIOBUTTON7, ID_RADIOBUTTON6);
				break;

			case ID_RADIOBUTTON7:
				CheckRadioButton(hwnd, ID_RADIOBUTTON5, ID_RADIOBUTTON7, ID_RADIOBUTTON7);
				break;
			}

	case WM_LBUTTONDOWN:
		break;

	case WM_RBUTTONDOWN:
		break;

	case WM_CLOSE:
		if (MessageBox(NULL, TEXT("Czy na pewno chcesz zakoñczyæ dzia³anie aplikacji?"), $AppName, MB_YESNO) == IDYES)
		{
			DestroyWindow(hwnd);
			DestroyWindow(button1); DestroyWindow(button2); DestroyWindow(button3); DestroyWindow(button4);
			DestroyWindow(RadioButton5); DestroyWindow(RadioButton6); DestroyWindow(RadioButton7);
			DestroyWindow(textBox1); DestroyWindow(textBox2); DestroyWindow(textBox3); DestroyWindow(textBox4);
			DestroyWindow(ListBox1);
			DestroyWindow(ComboBox1); DestroyWindow(ComboBox2);
			DestroyWindow(ScrollBar1); DestroyWindow(ScrollBar2);
			DestroyWindow(Static1); DestroyWindow(Static2); DestroyWindow(Static3); DestroyWindow(Static4); DestroyWindow(Static5);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);							// wstawia do kolejki komunikat "quit" o zakonczeniu programu
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int $RegisterClass(WNDCLASSEX wC, HINSTANCE hInstance)
{
	wC.lpfnWndProc = WndProc;							// Wskaznik do procedury obslugujacej okno (poczatkowo wpisujemy WndProc).
	wC.hInstance = hInstance;							// Identyfikator aplikacji, ktora ma byc wlascicielem okna 
	wC.cbSize = sizeof(WNDCLASSEX);						// Rozmiar struktury w bajtach. Nalezy tu wpisac sizeof (WNDCLASSEX).
	wC.lpszClassName = $ClassName;						// Nazwa klasy, ktora tworzymy. 
	wC.lpszMenuName = NULL;								// Nazwa identyfikujaca menu naszego okna w pliku zasobow.
	wC.cbClsExtra = 0;
	wC.cbWndExtra = 0;									// Dodatkowe bajty pamieci dla klasy (mozna ustawic na 0).
	wC.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// LoadIcon - wczytuje do pamieci ikone uzywana przez program
	wC.hIconSm = LoadIcon(NULL, IDI_APPLICATION);		// Mala ikonka aplikacji w rogu okienka oraz na pasku zadan. 
	wC.hCursor = LoadCursor(NULL, IDC_ARROW);			// LoadCursor - wczytuje do pamieci kursor myszy uzywany przez program
	wC.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		// Tlo naszego okienka, czyli jego kolor i wzor. 
	wC.style = CS_VREDRAW | CS_HREDRAW;					// Style klasy.

	if (!RegisterClassEx(&wC))
	{
		return -1;				// Jezeli sie nie udalo.
	}
	return 0;					// Jezeli sie udalo.
}

static void $Draw(const HWND Window)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(Window, &ps);
	GetClientRect(Window, &rect);			// pobiera rozmiary obszaru okna roboczego

	TCHAR  $bufferDOWN[50];

	swprintf($bufferDOWN, 50, TEXT("Rozmiar: %i x %i"), rect.right, rect.bottom);
	TextOut(hdc, 5, 5, $bufferDOWN, wcslen($bufferDOWN));

	EndPaint(Window, &ps);
}
