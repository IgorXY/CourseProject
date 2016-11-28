// CourseV1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CourseV1.h"
#include <commdlg.h>
#include <string>
#include <CommCtrl.h>
#include <Windows.h>


using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HWND hWnd, Bt1, Bt2, bSave, bOpen, 
secLabel, minLabel, hourLabel, secEdit, minEdit, hourEdit,
repEdit, repLabel, helpLabel,
soundCB;
HINSTANCE hInst;                                // current instance
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
UINT_PTR tmr, repTmr, PlayTmr;
POINT CrPos;
int posX, posY, sizeOF, pos, repeat, sound = 100;
HANDLE hFile;
DWORD nBytesRead;
SYSTEMTIME startTime, lastClickTime, clickTime;
LONG difTime, delayTime;
string fileName = "temp.m";


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void TimerStart();
void TimerStop();
void RepeatTimerStart();
void RepeatTimerStop();
void _stdcall TimerPrc(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam);
void _stdcall RepeatTimerPrc(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam);
void FilePRWrite();
void FileStart();
void FileStop();
void FilePlayOpen();
void _stdcall FilePlay(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam);
void FilePlayClose();
string GetFilepathDialog(HWND hWnd, bool save);
void GetDelayTime(HWND hWnd);
void EnableButtons(BOOL state);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDC_COURSEV1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEV1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEV1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNSHADOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COURSEV1);
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
   char *title = "Tigorware Mouse Recorder";
    hWnd = CreateWindowW(szWindowClass, (LPCWSTR)title, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 500, nullptr, nullptr, hInstance, nullptr);
	//buttons
   Bt1 = CreateWindow("BUTTON", "Start record", WS_CHILD | WS_VISIBLE, 
	   80, 50, 120, 40, hWnd, NULL, hInst, NULL);
   Bt2 = CreateWindow("BUTTON", "Play", WS_CHILD | WS_VISIBLE, 
	   80, 200, 120, 40, hWnd, NULL, hInst, NULL);
   bSave = CreateWindow("BUTTON", "Record to...", WS_CHILD | WS_VISIBLE, 
	   80, 100, 120, 40, hWnd, NULL, hInst, NULL);
   bOpen = CreateWindow("BUTTON", "Open file", WS_CHILD | WS_VISIBLE, 
	   80, 150, 120, 40, hWnd, NULL, hInst, NULL);
   //edits
   secEdit = CreateWindowEx(WS_EX_CLIENTEDGE , "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
	   150, 330, 50, 20, hWnd, (HMENU)1001, hInst, NULL);
   minEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
	   150, 360, 50, 20, hWnd, (HMENU)1002, hInst, NULL);
   hourEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
	   150, 390, 50, 20, hWnd, (HMENU)1003, hInst, NULL);
   repEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_NUMBER,
	   150, 300, 50, 20, hWnd, (HMENU)1004, hInst, NULL);
   //labels
   secLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "seconds:", WS_CHILD | WS_VISIBLE,
	   80, 330, 70, 20, hWnd, NULL, hInst, NULL);
   minLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "minutes:", WS_CHILD | WS_VISIBLE,
	   80, 360, 70, 20, hWnd, NULL, hInst, NULL);
   hourLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "hours:", WS_CHILD | WS_VISIBLE,
	   80, 390, 70, 20, hWnd, NULL, hInst, NULL);
   repLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "repeat:", WS_CHILD | WS_VISIBLE,
	   80, 300, 70, 20, hWnd, NULL, hInst, NULL);
   helpLabel = CreateWindowEx(WS_EX_TRANSPARENT, "STATIC", "Press ESC to stop recording or playing", WS_CHILD | WS_VISIBLE ,
	   20, 20, 260, 40, hWnd, (HMENU)1010, hInst, NULL);
   //checkbox
   soundCB = CreateWindow(TEXT("button"), TEXT("Sound"),  WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 
	   120, 255, 185, 35,	   hWnd, (HMENU)1005, hInst, NULL);
   CheckDlgButton(hWnd, 1005, BST_CHECKED);
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
    switch (message)
    {
	case WM_CREATE:
		ShowWindow(helpLabel, SW_HIDE);
		CheckDlgButton(hWnd, 1005, BST_CHECKED);
		break;
    case WM_COMMAND:
        {
			if (soundCB == (HWND)lParam)
			{
				BOOL checked = IsDlgButtonChecked(hWnd, 1005);
				if (checked)
				{
					CheckDlgButton(hWnd, 1005, BST_UNCHECKED);
					sound = 0;
				}
				else
				{
					CheckDlgButton(hWnd, 1005, BST_CHECKED);
					sound = 100;
				}
			}

			int wmId = LOWORD(wParam);

			if ((Bt1 == (HWND)lParam)||(wmId == IDM_REC))
			{
				EnableButtons(0);
				FileStart();
				GetSystemTime(&lastClickTime);
				TimerStart();
			}
			if ((Bt2 == (HWND)lParam) || (wmId == IDM_PLAY))
			{
				EnableButtons(0);
				GetSystemTime(&startTime);
				GetDelayTime(hWnd);
				FilePlayOpen();
			}
			if ((bSave == (HWND)lParam) || (wmId == IDM_SAVE))
			{
				fileName = GetFilepathDialog(hWnd, 1);
			}
			if ((bOpen == (HWND)lParam) || (wmId == IDM_OPEN))
			{
				fileName = GetFilepathDialog(hWnd, 0);
			}
            
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }

			
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
		
	}

    return (INT_PTR)FALSE;
}

void EnableButtons(BOOL state) {
	EnableWindow(Bt1, state);
	EnableWindow(Bt2, state);
	EnableWindow(bSave, state);
	EnableWindow(bOpen, state);

}

void TimerStart()
{
	tmr = SetTimer(NULL, NULL, 1, TimerPrc);
}

void TimerStop()
{
	KillTimer(NULL, tmr);
}

void RepeatTimerStart()
{
	repTmr = SetTimer(NULL, NULL, delayTime, RepeatTimerPrc);
}

void RepeatTimerStop()
{
	KillTimer(NULL, repTmr);
}


void _stdcall TimerPrc(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam)
{
	if (GetAsyncKeyState(VK_ESCAPE) != 0)
	{
		TimerStop();
		FileStop();
		EnableButtons(1);
	}

	if (GetAsyncKeyState(VK_LBUTTON) != 0)
	{
		TimerStop();
		GetSystemTime(&clickTime);
		Beep(100, sound);
		GetCursorPos(&CrPos);
		posX = CrPos.x;
		posY = CrPos.y;
		FilePRWrite();
		Sleep(100);
		TimerStart();
	}
}

void _stdcall RepeatTimerPrc(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam)
{
	//Sleep(delayTime);
	FilePlayOpen();
}

string GetFilepathDialog(HWND hWnd, bool save) {
	static OPENFILENAME ofn;
	static char fullpath[255], filename[256], dir[256];
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst; // дескриптор копии приложения
	ofn.lpstrFilter = (LPCSTR)"Mouse input file (*.m)\0*.m\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = (LPSTR)fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = (LPSTR)filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = (LPCSTR)dir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_EXPLORER;
	if (save) {
		ofn.lpstrTitle = (LPCSTR)"Save file as...";
		GetSaveFileName(&ofn);
	}
	else {
		ofn.lpstrTitle = (LPCSTR)"Open file";
		GetOpenFileName(&ofn);
	}
	return fullpath;
}

void FileStart()
{
	sizeOF = 0;
	hFile = CreateFile((LPCSTR)fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//WriteFile(hFile, &Sigbature, 4, &nBytesRead, NULL);
}

void FilePRWrite()
{
	if (sizeOF != 0) //Пропускаем первый клик по кнопке Начать запись
	{
		WriteFile(hFile, &posX, sizeof(posX), &nBytesRead, NULL);
		WriteFile(hFile, &posY, sizeof(posY), &nBytesRead, NULL);
		difTime = (clickTime.wMinute * 60 + clickTime.wSecond) * 1000 + clickTime.wMilliseconds -
			(lastClickTime.wMinute * 60 + lastClickTime.wSecond) * 1000 - lastClickTime.wMilliseconds;
		WriteFile(hFile, &difTime, sizeof(difTime), &nBytesRead, NULL);
		lastClickTime = clickTime;
	}
	sizeOF++;
}

void FileStop()
{
	sizeOF--;
	WriteFile(hFile, &sizeOF, sizeof(sizeOF), &nBytesRead, NULL);
	CloseHandle(hFile);
}

void FilePlayOpen()
{
	//Структура файла: ( Signature> 0 0 0 0 intX> 0 0 0 0 intY> 0 0 0 0 ..... End_Size_Click> 0 0 0 0 )
	hFile = CreateFile((LPCSTR)fileName.c_str(), GENERIC_READ, FILE_READ_DATA, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, "File not found", "Error", MB_ICONERROR);
		EnableButtons(0);
		return;
	}
	pos = GetFileSize(hFile, NULL);
	pos -= 4; //Размернось int 4 байта
	SetFilePointer(hFile, pos, NULL, FILE_BEGIN); // Ставим позицию для считывания указателя размерности SizeOF (сколько раз нажимали мышью)
	ReadFile(hFile, &sizeOF, 4, &nBytesRead, NULL); // Получаем число
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN); // Возвращаем позицию для считывания последующих данных
	pos = 0;
	PlayTmr = SetTimer(NULL, NULL, 1, FilePlay);
}

void _stdcall FilePlay(HWND hWnd, UINT message, UINT_PTR rparam, DWORD lparam)
{
	int x=0;
	if (GetAsyncKeyState(VK_ESCAPE) != 0)
	{
		FilePlayClose();
		EnableButtons(1);
		return;
	}


	if (pos >= sizeOF)
	{
		
		FilePlayClose();
		if (repeat > 0)
		{
			repeat--;
			//RepeatTimerStart();
			GetSystemTime(&startTime);
			
			do {
				GetSystemTime(&clickTime);
				x= (clickTime.wHour * 60 + clickTime.wMinute * 60 + clickTime.wSecond) 
					* 1000 + clickTime.wMilliseconds
					- (startTime.wHour * 60 + startTime.wMinute * 60 + startTime.wSecond) * 1000
					- startTime.wMilliseconds - delayTime;
			} while ((x < 0) && (GetAsyncKeyState(VK_ESCAPE) == 0));
			//Sleep(delayTime);
			if(x>=0)
				FilePlayOpen();
			else
			{
				EnableButtons(1);
			}
			return;
		}
		else {
			EnableButtons(1);
		}
		return;
	}
	

	ReadFile(hFile, &posX, 4, &nBytesRead, NULL);
	ReadFile(hFile, &posY, 4, &nBytesRead, NULL);
	ReadFile(hFile, &difTime, 4, &nBytesRead, NULL);
	pos++;
	//Sleep(difTime);
	GetSystemTime(&startTime);
	do {
		GetSystemTime(&clickTime);
		x = (clickTime.wHour * 60 + clickTime.wMinute * 60 + clickTime.wSecond)
			* 1000 + clickTime.wMilliseconds
			- (startTime.wHour * 60 + startTime.wMinute * 60 + startTime.wSecond) * 1000
			- startTime.wMilliseconds - difTime;
	} while ((x < 0) && (GetAsyncKeyState(VK_ESCAPE) == 0));
	if (x >= 0)
	{
		SetCursorPos(posX, posY);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(10);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Beep(150, sound);
	}
	else
		return;
}


void FilePlayClose()
{
	CloseHandle(hFile);
	KillTimer(NULL, PlayTmr);
}

void GetDelayTime(HWND hWnd) {
	char sec[6], min[6], hour[6], rep[6];
	GetWindowText(GetDlgItem(hWnd, 1001), sec, 6);
	GetWindowText(GetDlgItem(hWnd, 1002), min, 6); 
	GetWindowText(GetDlgItem(hWnd, 1003), hour, 6);
	GetWindowText(GetDlgItem(hWnd, 1004), rep, 6);
	delayTime = (atoi(hour) * 60 + atoi(min) * 60 + atoi(sec))*1000;
	repeat = atoi(rep);
}
