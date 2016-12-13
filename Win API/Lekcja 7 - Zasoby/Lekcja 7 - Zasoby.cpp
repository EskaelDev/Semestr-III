// Lekcja 7 - Zasoby.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lekcja 7 - Zasoby.h"
#include "resource.h"
#include <vector>

#define MAX_LOADSTRING 100
#define CELL_W 110
#define CELL_H 20
#define CELL_WIDTH 175
#define CELL_HEIGHT 50

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND informacje[4][10];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		DlgProc(HWND, UINT, WPARAM, LPARAM);

//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


struct Osoba
{
	TCHAR Imie[64];
	TCHAR Nazwisko[64];
	TCHAR Godzina[64];
	TCHAR Rodzaj[64];
};

std::vector<Osoba> Wpisy;

BOOL Numer_w_Stringu(TCHAR string[])
{
	BOOL wynik = TRUE;

	if (wcslen(string) >= 1)
	{
		for (size_t i = 32; i < 65; i++)
			if (wcschr(string, i) != NULL)
			{
				wynik = FALSE;
				break;
			}

		for (size_t i = 91; i < 96; i++)
			if (wcschr(string, i) != NULL)
			{
				wynik = FALSE;
				break;
			}

		for (size_t i = 123; i < 128; i++)
			if (wcschr(string, i) != NULL)
			{
				wynik = FALSE;
				break;
			}
	}
	else wynik = FALSE;

	return wynik;
}

void Usun_Wszystkich()
{
	for (size_t j = 0; j < 10; j++)
	{
		for (size_t i = 0; i < 4; i++)
		{
			SetWindowText(informacje[i][j], NULL);
		}
	}
	Wpisy.clear();
}

void Aktualizuj_Komorki()
{
	size_t licznik = 0;

	if(Wpisy.size()>0)
		for (std::vector<Osoba>::reverse_iterator it = Wpisy.rbegin(); it != Wpisy.rend(); it++)
		{
			if (licznik < 10)
			{
				SetWindowText(informacje[0][licznik], it->Imie);
				SetWindowText(informacje[1][licznik], it->Nazwisko);
				SetWindowText(informacje[2][licznik], it->Rodzaj);
				SetWindowText(informacje[3][licznik], it->Godzina);

				licznik++;
			}
		}
}


void SprawdzInformacje(HWND hWnd)
{
	Osoba wpis;
	bool CzyMogeDodacNowa = true;

	TCHAR buff[64];
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_I), buff, 64);

	TCHAR Msg[64];
	LoadString(GetModuleHandle(NULL), IDS_STRING132, Msg, sizeof(Msg) / sizeof(TCHAR));

	if (!Numer_w_Stringu(buff))
	{
		MessageBox(hWnd, Msg, TEXT("Error"), MB_OK);
		CzyMogeDodacNowa = false;
	}
	else
		wcscpy(wpis.Imie, buff);

	/////////////////////////////////////////////////

	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_N), buff, 64);

	
	LoadString(GetModuleHandle(NULL), IDS_STRING133, Msg, sizeof(Msg) / sizeof(TCHAR));

	if (!Numer_w_Stringu(buff))
	{
		MessageBox(hWnd, Msg, TEXT("Error"), MB_OK);
		CzyMogeDodacNowa = false;
	}
	else
		wcscpy(wpis.Nazwisko, buff);

	/////////////////////////////////////////////////

	LoadString(GetModuleHandle(NULL), IDS_STRING134, Msg, sizeof(Msg) / sizeof(TCHAR));

	if (IsDlgButtonChecked(hWnd, IDC_RADIO_I) == BST_UNCHECKED && IsDlgButtonChecked(hWnd, IDC_RADIO_O) == BST_UNCHECKED)
	{
		MessageBox(hWnd, Msg, TEXT("ERROR"), MB_OK);
		CzyMogeDodacNowa = false;
	}
	else
		if (IsDlgButtonChecked(hWnd, IDC_RADIO_I)==BST_CHECKED)
		{
			wcscpy(wpis.Rodzaj, TEXT("Wchodzi"));
		}
		else if (IsDlgButtonChecked(hWnd, IDC_RADIO_O)==BST_CHECKED)
		{
			wcscpy(wpis.Rodzaj, TEXT("Wychodzi"));
		}

	/////////////////////////////////////////////////

	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_H), buff, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING135, Msg, sizeof(Msg) / sizeof(TCHAR));
	
	if (!_wtoi(buff) || _wtoi(buff) > 24 || _wtoi(buff) <= 0)
	{
		MessageBox(hWnd, Msg, TEXT("Error"), MB_OK);
		CzyMogeDodacNowa = false;
	}

	TCHAR min[64];
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_M), min, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING136, Msg, sizeof(Msg) / sizeof(TCHAR));

	if (!_wtoi(min) || _wtoi(min) >= 60 || _wtoi(min) <= 0) {
		MessageBox(hWnd, Msg, TEXT("Error"), MB_OK);
		CzyMogeDodacNowa = false;
	}

	/////////////////////////////////////////////////

	TCHAR sec[64];
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_S), sec, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING137, Msg, sizeof(Msg) / sizeof(TCHAR));

	if (!_wtoi(sec) || _wtoi(sec) >= 60 || _wtoi(sec) <= 0) {
		MessageBox(hWnd, Msg, TEXT("Error"), MB_OK);
		CzyMogeDodacNowa = false;
	}

	if (CzyMogeDodacNowa) {
		_swprintf(wpis.Godzina, TEXT("%d:%d:%d"), _wtoi(buff), _wtoi(min), _wtoi(sec));

		Wpisy.push_back(wpis);
		
		

	}


}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LEKCJA7ZASOBY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEKCJA7ZASOBY));

	HACCEL hAccel1 = LoadAccelerators(hInstance, (LPCTSTR)IDR_ACCELERATOR1);

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccel1, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEKCJA7ZASOBY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)CreateSolidBrush(0x2c2c2c);
    wcex.lpszMenuName   = (LPCTSTR)IDR_GLOWNE_MENU;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND Tytul[4];
	static std::vector<TCHAR*> Tytuly;

    switch (message)
    {
	case WM_CREATE:
	{
		Tytuly.push_back(TEXT("Imie_tytuly"));
		Tytuly.push_back(TEXT("Nazwisko_tytuly"));
		Tytuly.push_back(TEXT("Czynnosc_tytuly"));
		Tytuly.push_back(TEXT("Czas_tytuly"));

		HINSTANCE &hInstance = ((LPCREATESTRUCT)lParam)->hInstance;


		for (size_t i = 0; i < 4; i++) {
			Tytul[i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), Tytuly.at(i), WS_VISIBLE | WS_CHILD, CELL_WIDTH * i, 0, CELL_WIDTH, CELL_HEIGHT, hWnd, 0, hInstance, 0);
			if (Tytul[i] == NULL) {
				MessageBox(NULL, TEXT("Some problem with CreateWindowEx by title"), szWindowClass, MB_OK);
				return 1;
			}
		}

		for (size_t j = 1; j <= 10; j++)
			for (size_t i = 0; i < 4; i++) {
				informacje[i][j - 1] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL, WS_VISIBLE | WS_CHILD, CELL_WIDTH * i, CELL_HEIGHT * j, CELL_WIDTH, CELL_HEIGHT, hWnd, 0, hInstance, 0);
				if (informacje[i][j - 1] == NULL) {
					MessageBox(NULL, TEXT("Some problem with CreateWindowEx by info"), szWindowClass, MB_OK);
					return 1;
				}
			}
	}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
          /*  case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;*/
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_ZAKONCZ:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			case ID_DODAJ:
				DialogBox(GetModuleHandle(NULL), (LPCTSTR)IDD_DIALOG1, hWnd, DlgProc);
				 
				break;
			case ID_USUN:
				Usun_Wszystkich();
				

				break;
			case ID_KONIEC:
				SendMessage(hWnd, WM_CLOSE, 0, 0);

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
	
			HICON WE = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
			
			for (std::vector<Osoba>::reverse_iterator it1 = Wpisy.rbegin(); it1 != Wpisy.rend(); it1++)
			{
				DrawIconEx(hdc, 1000, 1000, WE, 32, 32, 0, NULL, DI_NORMAL);

				OutputDebugString(it1->Rodzaj);
				if (_tcscmp(it1->Rodzaj, TEXT("Wchodzi")))
				DrawIconEx(hdc, 800, 500, WE, 32, 32, 0, NULL, DI_NORMAL);
				if (_tcscmp(it1->Rodzaj, TEXT("Wszed³")))
					DrawIconEx(hdc, 800, 500, WE, 32, 32, 0, NULL, DI_NORMAL);
			}
			//OutputDebugStringA((LPCSTR)it->Rodzaj);

            // TODO: Add any drawing code that uses hdc here...
				DrawIconEx(hdc, 700,500, WE, 32, 32, 0, NULL, DI_NORMAL);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
//
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
//////////////////////
BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	//OutputDebugStringA(" Wszed³em do DlgProc 1");
	switch (Msg) {
	case WM_COMMAND:
		//OutputDebugStringA(" WM_COMMAND ");

		switch (LOWORD(wParam))
		{
		case IDC_BDODAJ:
			SprawdzInformacje(hwnd);
			Aktualizuj_Komorki();
		break;
		case IDC_BANULUJ:
			EndDialog(hwnd, IDCANCEL);
			break;
		}

	break;

	case WM_CLOSE:
		//OutputDebugStringA(" WM_CLOSE ");

		EndDialog(hwnd, IDC_BUTTON2);
	break;

	default:
		//OutputDebugStringA(" default ");

		return FALSE;
	}
	//OutputDebugStringA(" Koniec ");

	return TRUE;
}
