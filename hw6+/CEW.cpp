#include "CEW.h"
#include <Windows.h>

const int CEllipseWindow::r = 30;

CEllipseWindow::CEllipseWindow()
{
	handle = 0;
	timerId = 0;
	dx = 2;
	dy = 2;
}

CEllipseWindow::~CEllipseWindow()
{
}

bool CEllipseWindow::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = CEllipseWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"Ellipse";
	windowClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	return (::RegisterClassEx(&windowClass) != 0);
}

bool CEllipseWindow::Create(HWND hWndParent)
{
	CreateWindowEx(0, L"Ellipse", L"ChildWindow", WS_CHILD, //| WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent,
		0, GetModuleHandle(0), this);
	return (handle != 0);
}

void CEllipseWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND CEllipseWindow::getHandle()
{
	return handle;
}

void CEllipseWindow::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void CEllipseWindow::OnCreate()
{
	x = r;
	y = r;
}

void CEllipseWindow::drawEllipse(HDC targetDC, int x, int y)
{
	HGDIOBJ oldPen, oldBrush;
	HBRUSH innBrush;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	innBrush = CreateSolidBrush(RGB(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1));
	oldPen = SelectObject(targetDC, pen);
	oldBrush = SelectObject(targetDC, innBrush);
	Ellipse(targetDC, x - r, y - r, x + r, y + r);
	SelectObject(targetDC, oldPen);
	SelectObject(targetDC, oldBrush);
	DeleteObject(innBrush);
	DeleteObject(pen);
}

void CEllipseWindow::OnPaint()
{
	RECT clientRect;
	PAINTSTRUCT paintStruct;
	HDC windowDC = ::BeginPaint(handle, &paintStruct);
	::GetClientRect(handle, &clientRect);
	HDC displayBufferDC = CreateCompatibleDC(windowDC);
	HBITMAP displayBuffer = CreateCompatibleBitmap(windowDC, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top);
	HGDIOBJ oldDisplayBuffer = SelectObject(displayBufferDC, displayBuffer);
	x += dx;
	y += dy;
	if (x + r > clientRect.right || x - r < clientRect.left) {
		dx = -dx;
		x += 2*dx;
	}
	if (y - r < clientRect.top || y + r > clientRect.bottom) {
		dy = -dy;
		y += 2*dy;
	}
	FillRect(displayBufferDC, &clientRect, (HBRUSH)GetStockObject(GRAY_BRUSH));
	drawEllipse(displayBufferDC, x, y);
	BitBlt(windowDC, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayBufferDC, 0, 0, SRCCOPY);
	SelectObject(displayBufferDC, oldDisplayBuffer);
	DeleteObject(displayBuffer);
	DeleteDC(displayBufferDC);
	::EndPaint(handle, &paintStruct);
}

void CEllipseWindow::OnTimer()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	InvalidateRect(handle, &rect, FALSE);
}

void CEllipseWindow::OnDestroy()
{
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void CEllipseWindow::OnLButtonDown()
{
	SetFocus(handle);
	timerId = SetTimer(handle, 0, 50, 0);
}

LRESULT CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_NCCREATE:
	{
		CEllipseWindow* window = (CEllipseWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		if (GetLastError() != 0) {
			return GetLastError();
		}
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CREATE:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnPaint();
		return 0;
	}
	case WM_TIMER:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (::GetFocus() == handle)
			window->OnTimer();
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnLButtonDown();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}