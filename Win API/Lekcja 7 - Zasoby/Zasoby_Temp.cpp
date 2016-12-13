#include <windows.h>
#include <vector>
#include "resource.h"

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 600
#define CELL_WIDTH 175
#define CELL_HEIGHT 50

TCHAR className[] = TEXT("Class");
TCHAR appName[] = TEXT("Menu");

HWND hwnd;
HWND informations[4][10];
MSG msg;

struct Person {
	TCHAR firstname[64];
	TCHAR lastname[64];
	TCHAR state[64];
	TCHAR time[64];
};

std::vector<Person> persons;

BOOL checkNumbersInString(TCHAR phrase[]) {
	BOOL result = TRUE;

	if (wcslen(phrase) >= 1) {
		for (size_t i = 32; i < 65; i++)
			if (wcschr(phrase, i) != NULL) {
				result = FALSE;
				break;
			}

		for (size_t i = 91; i < 96; i++)
			if (wcschr(phrase, i) != NULL) {
				result = FALSE;
				break;
			}

		for (size_t i = 123; i < 128; i++)
			if (wcschr(phrase, i) != NULL) {
				result = FALSE;
				break;
			}
	}
	else
		result = FALSE;

	return result;
}

void removeAll() {
	for (size_t j = 0; j < 10; j++)
		for (size_t i = 0; i < 4; i++)
			SetWindowText(informations[i][j], NULL);

	persons.clear();
}

void updateCells() {
	size_t counter = 0;

	if (persons.size() > 0)
		for (std::vector<Person>::reverse_iterator it = persons.rbegin(); it != persons.rend(); ++it) {
			if (counter < 10) {
				SetWindowText(informations[0][counter], it->firstname);
				SetWindowText(informations[1][counter], it->lastname);
				SetWindowText(informations[2][counter], it->state);
				SetWindowText(informations[3][counter], it->time);

				counter++;
			}
		}
}

void checkInformations(HWND hwnd) {
	Person per;
	bool canAddNewPerson = true;

	TCHAR buf[64];
	GetWindowText(GetDlgItem(hwnd, IDC_EDIT1), buf, 64);

	TCHAR message[64];
	LoadString(GetModuleHandle(NULL), IDS_STRING104, message, sizeof(message) / sizeof(TCHAR));

	if (!checkNumbersInString(buf)) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}
	else
		wcscpy(per.firstname, buf);

	///////////////////////////////////////////////// 

	GetWindowText(GetDlgItem(hwnd, IDC_EDIT2), buf, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING105, message, sizeof(message) / sizeof(TCHAR));

	if (!checkNumbersInString(buf)) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}
	else
		wcscpy(per.lastname, buf);

	/////////////////////////////////////////////////

	LoadString(GetModuleHandle(NULL), IDS_STRING106, message, sizeof(message) / sizeof(TCHAR));

	if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_UNCHECKED &&
		IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_UNCHECKED) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}
	else
		if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED)
			wcscpy(per.state, TEXT("Wchodzi"));
		else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED)
			wcscpy(per.state, TEXT("Wychodzi"));

	/////////////////////////////////////////////////

	GetWindowText(GetDlgItem(hwnd, IDC_EDIT3), buf, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING107, message, sizeof(message) / sizeof(TCHAR));

	if (!_wtoi(buf) || _wtoi(buf) >= 24 || _wtoi(buf) <= 0) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}

	/////////////////////////////////////////////////

	TCHAR min[64];
	GetWindowText(GetDlgItem(hwnd, IDC_EDIT4), min, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING108, message, sizeof(message) / sizeof(TCHAR));

	if (!_wtoi(min) || _wtoi(min) >= 60 || _wtoi(min) <= 0) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}

	/////////////////////////////////////////////////

	TCHAR sec[64];
	GetWindowText(GetDlgItem(hwnd, IDC_EDIT5), sec, 64);

	LoadString(GetModuleHandle(NULL), IDS_STRING109, message, sizeof(message) / sizeof(TCHAR));

	if (!_wtoi(sec) || _wtoi(sec) >= 60 || _wtoi(sec) <= 0) {
		MessageBox(hwnd, message, TEXT("Error"), MB_OK);
		canAddNewPerson = false;
	}

	if (canAddNewPerson) {
		_swprintf(per.time, TEXT("%d:%d:%d"), _wtoi(buf), _wtoi(min), _wtoi(sec));

		persons.push_back(per);
	}
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1: { checkInformations(hwnd); updateCells(); }break;
		case IDC_BUTTON2: EndDialog(hwnd, IDCANCEL); break;
		}
	}break;

	case WM_CLOSE:
		EndDialog(hwnd, IDC_BUTTON2);
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static HWND title[4];
	static std::vector<TCHAR*> titles;

	switch (msg) {
	case WM_CREATE: {
		titles.push_back(TEXT("Firstname"));
		titles.push_back(TEXT("Lastname"));
		titles.push_back(TEXT("Status"));
		titles.push_back(TEXT("Time"));

		HINSTANCE &hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

		for (size_t i = 0; i < 4; i++) {
			title[i] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), titles.at(i), WS_VISIBLE | WS_CHILD, CELL_WIDTH * i, 0, CELL_WIDTH, CELL_HEIGHT, hwnd, 0, hInstance, 0);
			if (title[i] == NULL) {
				MessageBox(NULL, TEXT("Some problem with CreateWindowEx by title"), className, MB_OK);
				return 1;
			}
		}

		for (size_t j = 1; j <= 10; j++)
			for (size_t i = 0; i < 4; i++) {
				informations[i][j - 1] = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL, WS_VISIBLE | WS_CHILD, CELL_WIDTH * i, CELL_HEIGHT * j, CELL_WIDTH, CELL_HEIGHT, hwnd, 0, hInstance, 0);
				if (informations[i][j - 1] == NULL) {
					MessageBox(NULL, TEXT("Some problem with CreateWindowEx by info"), className, MB_OK);
					return 1;
				}
			}

	}break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == ID_ACCELERATOR40005 || LOWORD(wParam) == ID_WPIS_DODAJ)
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PROPPAGE_LARGE), hwnd, DlgProc);

		if (LOWORD(wParam) == ID_ACCELERATOR40006 || LOWORD(wParam) == ID_WPIS_USU40002)
			removeAll();

		if (LOWORD(wParam) == ID_ACCELERATOR40007 || LOWORD(wParam) == ID_WPIS_WYJD40003)
			SendMessage(hwnd, WM_CLOSE, wParam, lParam);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
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

	if (RegisterClassEx(&wc) == 0) {
		MessageBox(NULL, TEXT("Some problem with RegisterClassEx"), className, MB_OK);
		return 1;
	}

	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE, className, appName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, hMenu, hInstance, 0);
	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("Some problem with CreateWindowEx by hwnd"), className, MB_OK);
		return 1;
	}

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	if (!hAccel)
	{
		MessageBox(hwnd, TEXT("Nie mo¿na za³adowaæ akceleratorów!"), NULL, MB_ICONEXCLAMATION);

		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(className, hInstance);

	return msg.wParam;
}
