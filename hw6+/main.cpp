#include <windows.h>
#include "COW.h"
#include <iostream>

/*LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return (0);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX windowClass;
	MSG message;
	BOOL MsgRes = 0;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"Test";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	const ATOM atom = ::RegisterClassEx(&windowClass);
	if (!atom)
		return -1;

	HWND wnd = CreateWindowEx(WS_EX_ACCEPTFILES, MAKEINTATOM(atom), TEXT("TestWindow"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	if (!wnd)
		return -2;

	bool sw = ShowWindow(wnd, SW_SHOWDEFAULT);
	if (sw == 0) {
		return -3;
	}
	//UpdateWindow(wnd);
	
	while ((MsgRes = ::GetMessage(&message, NULL, 0, 0)) != 0) {
		if (MsgRes == -1) {
			return -4;
		}
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
	return 0;
}*/

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR commandLine, int cmdShow)
{
	if (!COverlappedWindow::RegisterClass()) {
		return -1;
	}
	COverlappedWindow window;
	if (!window.Create()) {
		return -1;
	}
	window.Show(cmdShow);
	MSG message;
	BOOL getMessageResult = 0;
	while ((getMessageResult = ::GetMessage(&message, 0, 0, 0)) != 0) {
		if (getMessageResult == -1) {
			return -1;
		}
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
	return 0;
}