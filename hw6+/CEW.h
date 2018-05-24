#pragma once
#include <Windows.h>

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();
	static bool RegisterClass();
	bool Create(HWND hWndParent);
	void Show(int cmdShow);
	HWND getHandle();

protected:
	void OnNCCreate(HWND handle);
	void OnCreate();
	void OnPaint();
	void OnTimer();
	void OnDestroy();
	void OnLButtonDown();

private:
	HWND handle; 
	UINT_PTR timerId; 
	static const int r;
	int dx;
	int dy;
	int x;
	int y;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void drawEllipse(HDC targetDC, int x, int y);
};