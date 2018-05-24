#pragma once
#include "CEW.h"
#include <windows.h>
#include <array>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
protected:
	void OnDestroy();
	void OnCreate();
	void OnNCCreate(const HWND handle);
	void OnSize();
private:
	std::array<CEllipseWindow, 4> childWindows;
	HWND handle; 
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};