#pragma once
#include <windows.h>
#include "GameZone.h"

class MainWindow {
public:
	MainWindow();
	~MainWindow();
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
protected:
	void OnDestroy();
	void OnCreate();
	void OnNCCreate(const HWND handle);
	void OnSize();
	bool OnClose();
	void OnNewMessage(int msg);

private:
	//std::array<CEllipseWindow, 4> childWindows;
	GameZone childWindow;
	HMENU menu;
	HWND handle;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};