#include <Windows.h>
#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR commandLine, int cmdShow)
{
	if (!MainWindow::RegisterClass()) {
		return -1;
	}
	MainWindow window;
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