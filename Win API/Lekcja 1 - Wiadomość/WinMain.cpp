// PLIKI NAGLOWOKOWE

#include <windows.h>
#include <tchar.h>

// WIN_MAIN

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 	LPSTR lpCmdLine, int nShowCmd)
{
	MessageBox(0, _T("Witam! Czego chcesz kurwiu?"), _T("Pytanie"), MB_OK | MB_ICONQUESTION);
	return 0;
}