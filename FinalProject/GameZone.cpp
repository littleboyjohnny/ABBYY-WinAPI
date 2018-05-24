#include "GameZone.h"
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


GameZone::GameZone() {
	handle = 0;
	//parentHandle = 0;
	timerId = 0;
	isNew = true;
	play = true;
	dx = 0;
	dy = 4;
	ddx = 0;
}

GameZone::~GameZone () {}

bool GameZone::RegisterClass() {
	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = GameZone::windowProc;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"GameZone";
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	return (::RegisterClassEx(&windowClass) != 0);
}

bool GameZone::Create(HWND hWndParent)
{
	CreateWindowEx(0, L"GameZone", L"ChildWindow", WS_CHILD | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWndParent,
		0, GetModuleHandle(0), this);
	//parentHandle = hWndParent;
	return (handle != 0);
}

void GameZone::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND GameZone::getHandle()
{
	return handle;
}

void GameZone::OnNCCreate(HWND _handle)
{
	handle = _handle;
}

void GameZone::OnCreate()
{
	dx = 0;
	dy = 4;
	ddx = 0;
}

void GameZone::drawCar(HDC targetDC, CarPos car) {
	HGDIOBJ oldPen, oldBrush;
	HBRUSH innBrush;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	innBrush = CreateSolidBrush(RGB(0, 0, 0));
	oldPen = SelectObject(targetDC, pen);
	oldBrush = SelectObject(targetDC, innBrush);

	Rectangle(targetDC, car.left, car.top, car.right, car.bottom);
	pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	innBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(targetDC, pen);
	SelectObject(targetDC, innBrush);
	Rectangle(targetDC, car.left, car.top + 5, car.left + 17, car.top + 33);
	Rectangle(targetDC, car.right - 17, car.top + 5, car.right, car.top + 33);
	Rectangle(targetDC, car.left + 17, car.top + 66, car.right - 17, car.bottom);

	SelectObject(targetDC, oldPen);
	SelectObject(targetDC, oldBrush);
	DeleteObject(innBrush);
	DeleteObject(pen);
}

bool GameZone::isCrash(CarPos car1, CarPos car2) {
	if ((car2.top <= car1.bottom && car1.bottom <= car2.bottom || car2.top <= car1.top && car2.bottom > car1.top) &&
		(car2.right >= car1.left && car2.left <= car1.left || car2.right >= car1.right && car2.left <= car1.right))
		return true;
	else
		return false;
}

void GameZone::drawZone(HDC targetDC, RECT clientRect) {
	HGDIOBJ oldPen, oldBrush;
	HBRUSH innBrush;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	innBrush = CreateSolidBrush(RGB(0, 0, 0));
	oldPen = SelectObject(targetDC, pen);
	oldBrush = SelectObject(targetDC, innBrush);

	Rectangle(targetDC, clientRect.left, clientRect.top, clientRect.left + leftBorder, clientRect.bottom);
	Rectangle(targetDC, clientRect.left + 400 - rightBoarder, clientRect.top, clientRect.right, clientRect.bottom);

	mainCar.left += ddx;
	mainCar.right += ddx;
	if (mainCar.right > clientRect.left + 400 - rightBoarder || mainCar.left < clientRect.left + leftBorder) {
		mainCar.left -= ddx;
		mainCar.right -= ddx;
	}
	drawCar(targetDC, mainCar);
	ddx = 0;

	for (int i = 0; i < subCars.size(); i++) {
		subCars[i].top += dy;
		subCars[i].bottom += dy;
		if (subCars[i].top > clientRect.bottom) {
			int place = rand() % 250 + 1;
			subCars[i] = CarPos(clientRect.top - 100, clientRect.left + leftBorder + place, clientRect.top, clientRect.left + leftBorder + place + 50);
		}
		drawCar(targetDC, subCars[i]);
	}
	bool crash = false;
	for (int i = 0; i < subCars.size(); i++) {
		if (crash = isCrash(mainCar, subCars[i]))
			break;
	}
	if (crash) {
		SetFocus(GetParent(handle));
		play = false;
		COPYDATASTRUCT pcds;
		::ZeroMemory(&pcds, sizeof(pcds));
		pcds.dwData = 1;
		pcds.cbData = 0;
		pcds.lpData = NULL;
		SendMessage(GetParent(handle), WM_COPYDATA, (WPARAM)handle, (LPARAM)&pcds);
	}

	SelectObject(targetDC, oldPen);
	SelectObject(targetDC, oldBrush);
	DeleteObject(innBrush);
	DeleteObject(pen);
}

void GameZone::OnPaint()
{
	RECT clientRect;
	PAINTSTRUCT paintStruct;
	HDC windowDC = ::BeginPaint(handle, &paintStruct);
	::GetClientRect(handle, &clientRect);

	if (isNew) {
		mainCar = CarPos(clientRect.bottom - 100, clientRect.left + 200 - 25,
			clientRect.bottom, clientRect.left + 200 + 25);
		for (int i = 0; i < subCars.size(); i++) {
			int place = rand() % 250 + 1;
			subCars[i] = CarPos(clientRect.top - 100 - 200 * i, clientRect.left + leftBorder + place,
				clientRect.top - 200 * i, clientRect.left + leftBorder + place + 50);
		}
		isNew = false;
	}

	HDC displayBufferDC = CreateCompatibleDC(windowDC);
	HBITMAP displayBuffer = CreateCompatibleBitmap(windowDC, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top);
	HGDIOBJ oldDisplayBuffer = SelectObject(displayBufferDC, displayBuffer);
	FillRect(displayBufferDC, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	drawZone(displayBufferDC, clientRect);
	BitBlt(windowDC, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayBufferDC, 0, 0, SRCCOPY);
	SelectObject(displayBufferDC, oldDisplayBuffer);
	DeleteObject(displayBuffer);
	DeleteDC(displayBufferDC);
	::EndPaint(handle, &paintStruct);
}

void GameZone::OnTimer()
{
	if (play) {
		RECT rect;
		::GetClientRect(handle, &rect);
		InvalidateRect(handle, &rect, FALSE);
	}
}

void GameZone::OnDestroy()
{
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void GameZone::OnLButtonDown()
{
	SetFocus(handle);
	/*
	RECT rect;
	::GetClientRect(handle, &rect);
	mainCar = CarPos(rect.bottom - 100, rect.left + 200 - 25, rect.bottom, rect.left + 200 + 25);
	for (int i = 0; i < subCars.size(); i++) {
		int place = rand() % 250 + 1;
		subCars[i] = CarPos(rect.top - 100 - 200*i, rect.left + leftBorder + place, rect.top - 200 * i, rect.left + leftBorder + place + 50);
	}
	*/
	//timerId = SetTimer(handle, 0, 50, 0);
}

void GameZone::OnNewGame() {
	SetFocus(handle);
	isNew = true;
	play = true;
	timerId = SetTimer(handle, 0, 50, 0);
}

void GameZone::OnPause() {
	play = false;
}

void GameZone::OnUnpause() {
	SetFocus(handle);
	play = true;
}

LRESULT GameZone::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_NCCREATE:
	{
		GameZone* window = (GameZone*)((CREATESTRUCT*)lParam)->lpCreateParams;
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
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnPaint();
		return 0;
	}
	case WM_COPYDATA:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
		switch (pcds->dwData)
		{
		case GAME_MSG_NEW_GAME:
		{
			window->OnNewGame();
			break;
		}
		case GAME_MSG_PAUSE:
		{
			window->OnPause();
			break;
		}
		case GAME_MSG_CONTINUE:
		{
			window->OnUnpause();
			break;
		}
		default:
			break;
		}
		return 0;
	}
	case WM_TIMER:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		//if (::GetFocus() == handle)
		window->OnTimer();
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnLButtonDown();
		return 0;
	}
	case WM_KEYDOWN:
	{
		GameZone* window = (GameZone*)GetWindowLongPtr(handle, GWLP_USERDATA);
		switch (wParam) {
		case VK_LEFT:
		{
			window->ddx = -4;
			return 0;
		}
		case VK_RIGHT:
		{
			window->ddx = 4;
			return 0;
		}
		}
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}