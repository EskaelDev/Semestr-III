// PLIKI NAGLOWOKOWE

#include <windows.h>
#include <tchar.h>

// WIN_MAIN

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 	LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(0, _T("Witam! Czego chcesz kurwiu?"), _T("Pytanie"), MB_CANCELTRYCONTINUE | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TOPMOST);
	return 0;
}