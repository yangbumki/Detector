#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

using namespace std;

typedef struct CAPTURE_INFO {
	u_int width;
	u_int height;
	vector<byte> bitmapDatas;
}CaptureInfo;

typedef class CAPTURE {
private:
	unsigned int totalScreens = 0;
	unsigned int screen = 0;
	
	vector<RECT> rects;

	//다중 디스플레이 감지 콜백함수
	 static BOOL DetectScreenFunc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM args);

	 RECT GetRect(int idx = 0);
	 void PushRect(const RECT& rect);

	void WarningMessage(const string msg);
	void DetectScreen();

public:
	CAPTURE();
	~CAPTURE();

	CaptureInfo CaptureScreen(const HWND appWnd = nullptr);

	HWND GetAppWindow(const string title);
	HWND GetAppWindow(const DWORD pid);

	int GetScreen();
	bool SetScreen(unsigned int idx = 0);
	int GetScreensCount();

}Capture;