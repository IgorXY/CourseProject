#include <windows.h>
HINSTANCE hInst;
MSG msg;
WNDCLASSEX wc;
HWND hWnd, Bt1, Bt2, m1, m2, m3, m4, m5;
UINT_PTR tmr, PlayTmr;
char Sigbature[] = { 'M','P','V','G' };
POINT CrPos;
int PositionX, PositionY, SizeOF, Pos;
HANDLE hFile;
DWORD nBytesRead;


void Run();
long _stdcall WndPrc(HWND v0, UINT v1, WPARAM v2, LPARAM v3);
void TimerStart();
void TimerStop();
void _stdcall TimerPrc(HWND v0, UINT v1, UINT_PTR v2, DWORD v3);
void FilePRWrite();
void FileStart();
void FileStop();
void FilePlayOpen();
void _stdcall FilePlay(HWND v0, UINT v1, UINT_PTR v2, DWORD v3);
void FilePlayClose();



void Run() // Точка старта программы /это мой стиль программирование просьба не обсуждать/
{
	hInst = GetModuleHandle(NULL);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)5;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = 0;
	wc.hIconSm = 0;
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndPrc;
	wc.lpszClassName = "MGSWND";
	wc.lpszMenuName = 0;
	wc.style = CS_DROPSHADOW;
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(0L, wc.lpszClassName, "Mouse Get Set By TRANSFORMER-V 2009", WS_SYSMENU, 200, 200, 800, 700, NULL, NULL, hInst, NULL);
	Bt1 = CreateWindowEx(WS_EX_DLGMODALFRAME, "BUTTON", "Начать Запись", WS_CHILD | WS_VISIBLE, 0, 0, 150, 100, hWnd, NULL, hInst, NULL);
	Bt2 = CreateWindowEx(WS_EX_DLGMODALFRAME, "BUTTON", "Проиграть", WS_CHILD | WS_VISIBLE, 0, 150, 150, 100, hWnd, NULL, hInst, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "НАЖМИ КЛАВИШУ <Esc> чтоб выйти из режима записи или проигрования", WS_CHILD | WS_VISIBLE, 210, 0, 550, 100, hWnd, NULL, hInst, NULL);

	m1 = CreateWindowEx(0L, "BUTTON", "Dummy 1", WS_CHILD | WS_VISIBLE, 200, 150, 150, 100, hWnd, NULL, hInst, NULL);
	m2 = CreateWindowEx(0L, "BUTTON", "Dummy 2", WS_CHILD | WS_VISIBLE, 380, 150, 150, 100, hWnd, NULL, hInst, NULL);
	m3 = CreateWindowEx(0L, "BUTTON", "Dummy 3", WS_CHILD | WS_VISIBLE, 270, 350, 150, 100, hWnd, NULL, hInst, NULL);
	m4 = CreateWindowEx(0L, "BUTTON", "Dummy 4", WS_CHILD | WS_VISIBLE, 340, 500, 150, 100, hWnd, NULL, hInst, NULL);
	m5 = CreateWindowEx(0L, "BUTTON", "Dummy 5", WS_CHILD | WS_VISIBLE, 200, 550, 150, 100, hWnd, NULL, hInst, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);
}


long _stdcall WndPrc(HWND v0, UINT v1, WPARAM v2, LPARAM v3)
{
	switch (v1)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
		if (Bt1 == (HWND)v3)
		{
			EnableWindow(Bt1, 0);
			EnableWindow(Bt2, 0);
			FileStart();
			TimerStart();
		}

		if (Bt2 == (HWND)v3)
		{
			EnableWindow(Bt1, 0);
			EnableWindow(Bt2, 0);
			FilePlayOpen();
		}

		if (m1 == (HWND)v3)
		{
			MessageBox(hWnd, "DUMMY 1", "", MB_ICONEXCLAMATION);
		}

		if (m2 == (HWND)v3)
		{
			MessageBox(hWnd, "DUMMY 2", "", MB_ICONEXCLAMATION);
		}

		if (m3 == (HWND)v3)
		{
			MessageBox(hWnd, "DUMMY 3", "", MB_ICONEXCLAMATION);
		}

		if (m4 == (HWND)v3)
		{
			MessageBox(hWnd, "DUMMY 4", "", MB_ICONEXCLAMATION);
		}

		if (m5 == (HWND)v3)
		{
			MessageBox(hWnd, "DUMMY 5", "", MB_ICONEXCLAMATION);
		}
		return 0;
	default:
		return DefWindowProc(v0, v1, v2, v3);
	}
	return 0;
}

void TimerStart()
{
	tmr = SetTimer(NULL, NULL, 1, TimerPrc);
}

void TimerStop()
{
	KillTimer(NULL, tmr);
}


void _stdcall TimerPrc(HWND v0, UINT v1, UINT_PTR v2, DWORD v3)
{
	if (GetAsyncKeyState(VK_ESCAPE) != 0)
	{
		TimerStop();
		FileStop();
		EnableWindow(Bt1, 1);
		EnableWindow(Bt2, 1);
	}

	if (GetAsyncKeyState(VK_LBUTTON) != 0)
	{
		TimerStop();
		Beep(100, 100);
		GetCursorPos(&CrPos);
		PositionX = CrPos.x;
		PositionY = CrPos.y;
		FilePRWrite();
		TimerStart();
	}
}

void FileStart()
{
	SizeOF = 0;
	hFile = CreateFile("MousePosition.bin", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &Sigbature, 4, &nBytesRead, NULL);
}

void FilePRWrite()
{
	WriteFile(hFile, &PositionX, sizeof(PositionX), &nBytesRead, NULL);
	WriteFile(hFile, &PositionY, sizeof(PositionY), &nBytesRead, NULL);
	SizeOF++;
}

void FileStop()
{
	WriteFile(hFile, &SizeOF, sizeof(SizeOF), &nBytesRead, NULL);
	CloseHandle(hFile);
}

void FilePlayOpen()
{
	//Структура файла: ( Signature> 0 0 0 0 intX> 0 0 0 0 intY> 0 0 0 0 ..... End_Size_Click> 0 0 0 0 )
	hFile = CreateFile("MousePosition.bin", GENERIC_READ, FILE_READ_DATA, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "Файла нету", "Ошибка", MB_ICONERROR);
		EnableWindow(Bt1, 1);
		EnableWindow(Bt2, 1);
		return;
	}
	Pos = GetFileSize(hFile, NULL);
	Pos -= 4; //Размернось int 4 байта
	SetFilePointer(hFile, Pos, NULL, FILE_BEGIN); // Ставим позицию для считывания указателя размерности SizeOF (сколько раз нажимали мышью)
	ReadFile(hFile, &SizeOF, 4, &nBytesRead, NULL); // Получаем число
	SetFilePointer(hFile, 4, NULL, FILE_BEGIN); // Возвращаем позицию для считывания последующих данных
	Pos = 0;
	PlayTmr = SetTimer(NULL, NULL, 1000, FilePlay);
}

void _stdcall FilePlay(HWND v0, UINT v1, UINT_PTR v2, DWORD v3)
{

	if (GetAsyncKeyState(VK_ESCAPE) != 0)
	{
		FilePlayClose();
		EnableWindow(Bt1, 1);
		EnableWindow(Bt2, 1);
		return;
	}


	if (Pos >= SizeOF)
	{
		FilePlayClose();
		EnableWindow(Bt1, 1);
		EnableWindow(Bt2, 1);
		return;
	}
	ReadFile(hFile, &PositionX, 4, &nBytesRead, NULL);
	ReadFile(hFile, &PositionY, 4, &nBytesRead, NULL);
	Pos++;
	SetCursorPos(PositionX, PositionY);
	Sleep(200);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Beep(150, 100);
}


void FilePlayClose()
{
	KillTimer(NULL, PlayTmr);
	CloseHandle(hFile);
}