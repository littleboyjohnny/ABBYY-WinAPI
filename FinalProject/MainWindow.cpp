#include "MainWindow.h"
#include <windows.h>

#ifndef GAME_MSG_CRASH
#define GAME_MSG_CRASH 1
#endif

#ifndef GAME_MSG_NEW_GAME
#define GAME_MSG_NEW_GAME 2
#endif

#ifndef GAME_MSG_CONTINUE
#define GAME_MSG_CONTINUE 3
#endif

#ifndef GAME_MSG_PAUSE
#define GAME_MSG_PAUSE 4
#endif

MainWindow::MainWindow() {
	handle = 0;
	menu = 0;
}

MainWindow::~MainWindow() {}

bool MainWindow::RegisterClass() {
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = MainWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"MainWindow";
	windowClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	return (::RegisterClassEx(&windowClass) != 0);
}

bool MainWindow::Create() {
	HWND hwndA = CreateWindowEx(0, L"MainWindow", L"Car Racing", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, GetModuleHandle(0), this);
	menu = CreateMenu();
	AppendMenu(menu, MF_STRING, 1, L"New");
	AppendMenu(menu, MF_STRING, 2, L"Continue");
	AppendMenu(menu, MF_STRING, 3, L"Pause");
	SetMenu(hwndA, menu);
	return (handle != 0);
}

void MainWindow::Show(int cmdShow) {
	ShowWindow(this->handle, SW_SHOWDEFAULT);
	childWindow.Show(cmdShow);
	//for (unsigned int i = 0; i < childWindows.size(); i++) {
	//	childWindows[i].Show(cmdShow);
	//}
}

void MainWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void MainWindow::OnCreate()
{
	//CEllipseWindow::RegisterClass();
	//for (int i = 0; i < childWindows.size(); i++) {
	//	childWindows[i].Create(handle);
	//}
	GameZone::RegisterClass();
	childWindow.Create(handle);
}

void MainWindow::OnSize()
{
	RECT rect;
	int middleX, middleY, nWidth, nHeight;
	::GetClientRect(handle, &rect);
	/*
	if (rect.right - rect.left < 500) {
		if (rect.bottom - rect.top < 500)
			SetWindowPos(handle, HWND_TOP, rect.left, rect.top, 500, 500, 0);
		else
			SetWindowPos(handle, HWND_TOP, rect.left, rect.top, 500, rect.bottom - rect.top, 0);
	}
	::GetClientRect(handle, &rect);
	*/
	//middleX = (rect.left + rect.right) / 2;
	//middleY = (rect.top + rect.bottom) / 2;
	nWidth = 400;// (rect.right - rect.left) / 2;
	nHeight = (rect.bottom - rect.top);
	SetWindowPos(childWindow.getHandle(), HWND_TOP, rect.left, rect.top, nWidth, nHeight, 0);
	//SetWindowPos(childWindows[1].getHandle(), HWND_TOP, middleX, rect.top, nWidth, nHeight, 0);
	//SetWindowPos(childWindows[2].getHandle(), HWND_TOP, rect.left, middleY, nWidth, nHeight, 0);
	//SetWindowPos(childWindows[3].getHandle(), HWND_TOP, middleX, middleY, nWidth, nHeight, 0);
}

void MainWindow::OnDestroy()
{
}

bool MainWindow::OnClose() {
	int msgBox = MessageBox(handle, L"Вы действительно хотите выйти?", L"Завершение работы", MB_YESNO);
	switch (msgBox)
	{
	case IDYES:
	{
		return true;
	}
	case IDNO:
	{
		return false;
	}
	}
	return false;
}

void MainWindow::OnNewMessage(int msg) {
	COPYDATASTRUCT answer_pcds;
	::ZeroMemory(&answer_pcds, sizeof(answer_pcds));
	answer_pcds.dwData = msg;
	answer_pcds.cbData = 0;
	answer_pcds.lpData = NULL;
	SendMessage(childWindow.getHandle(), WM_COPYDATA, (WPARAM)handle, (LPARAM)&answer_pcds);
}

LRESULT __stdcall MainWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE:
	{
		MainWindow* window = (MainWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		if (GetLastError() != 0) {
			return GetLastError();
		}
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CLOSE:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (window->OnClose()) {
			return DefWindowProc(handle, message, wParam, lParam);
		}
		else
			return 0;
	}
	case WM_CREATE:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_COPYDATA:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
		switch (pcds->dwData)
		{
		case GAME_MSG_CRASH:
		{
			int msgBox = MessageBox(handle, L"LoL! You died! Again?", L"You Lose", MB_YESNO);
			switch (msgBox)
			{
			case IDYES:
			{
				window->OnNewMessage(GAME_MSG_NEW_GAME);
				break;
			}
			case IDNO:
			{
				SendMessage(handle, WM_CLOSE, 0, 0);
				break;
			}
			}
			break;
		}
		default:
			break;
		}
		return 0;
	}
	case WM_COMMAND:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);

		switch (LOWORD(wParam)) {
		case 1:
		{
			window->OnNewMessage(GAME_MSG_NEW_GAME);
			break;
		}
		case 2:
		{
			window->OnNewMessage(GAME_MSG_CONTINUE);
			break;
		}
		case 3:
		{
			window->OnNewMessage(GAME_MSG_PAUSE);
			break;
		}
		default:
		{
			break;
		}
		}

		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_SIZE:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_GETMINMAXINFO:
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		(*(MINMAXINFO*)lParam).ptMinTrackSize.x = 700;
		(*(MINMAXINFO*)lParam).ptMinTrackSize.y = 500;
		(*(MINMAXINFO*)lParam).ptMaxTrackSize.y = 500;
		(*(MINMAXINFO*)lParam).ptMaxTrackSize.x = 1000;
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}