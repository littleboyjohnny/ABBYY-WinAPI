#pragma once
#include <windows.h>
#include <array>

class GameZone {
public:
	GameZone();
	~GameZone();
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
	void OnNewGame();
	void OnPause();
	void OnUnpause();

private:
	class CarPos {
	public:
		CarPos() {}
		CarPos(int top, int left, int bottom, int right): top(top), left(left), bottom(bottom), right(right) {}
		~CarPos() {}

		int top;
		int bottom;
		int right;
		int left;
	};

	HWND handle;
	//HWND parentHandle;
	UINT_PTR timerId;
	int dx;
	int ddx;
	int dy;
	int ddy;
	int leftBorder = 50;
	int rightBoarder = 50;
	bool isNew;
	bool play;
	/*
	int topMainCar;
	int bottomMainCar;
	int leftMainCar;
	int rightMainCar;
	*/
	CarPos mainCar;
	std::array<CarPos, 3> subCars;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void drawZone(HDC targetDC, RECT clientRect);
	void drawCar(HDC targetDC, CarPos car);
	bool isCrash(CarPos car1, CarPos car2);
};