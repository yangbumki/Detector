#include "Capture.h"

CAPTURE::CAPTURE() {
	//2024-08-12 다중 디스플레이 위한 내부 함수 생성자에서 호출
	DetectScreen();

	totalScreens = rects.size();
}

CAPTURE::~CAPTURE() {
}

BOOL CAPTURE::DetectScreenFunc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM args) {
	CAPTURE* cp = (CAPTURE*)args;

	cp->PushRect(*rect);

	return true;
}

RECT CAPTURE::GetRect(int idx) {
	return rects.at(idx);
}

void CAPTURE::PushRect(const RECT& rect) {
	rects.push_back(rect);
}

void CAPTURE::WarningMessage(const string msg) {
	cout << endl;
	cout << "===CAPTURE===" << endl;
	cerr << msg << endl;
}

void CAPTURE::DetectScreen() {
	//while (EnumDisplayMonitors(NULL, NULL, DetectScreenFunc, (LPARAM)this));
	EnumDisplayMonitors(NULL, NULL, DetectScreenFunc, (LPARAM)this);
}

CaptureInfo CAPTURE::CaptureScreen(const HWND appWnd) {
	// Get the device context of the screen
	HDC hdcScreen = appWnd == nullptr ? GetDC(nullptr) : GetDC(appWnd);

	HDC hdcMem = CreateCompatibleDC(hdcScreen);

	RECT rect{};

	unsigned int width, height;

	//2024-08-09 : 복사자체는 디바이스 크기로 해야 안깨짐
	/*if (GetWindowRect(appWnd, &rect)) {
		width = rect.right;
		height = rect.top;
	}
	else {
		width = GetDeviceCaps(hdcScreen, HORZRES);
		height = GetDeviceCaps(hdcScreen, VERTRES);
	}*/

	/*width = GetDeviceCaps(hdcScreen, HORZRES);
	height = GetDeviceCaps(hdcScreen, VERTRES);*/

	//2024-08-12 다중디스플레이 지원을 위한 코드수정
	rect = GetRect(GetScreen());

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;


	// Create a compatible bitmap
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

	// BitBlt from screen to memory DC
	//BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
	//2024-08-12 다중디스플레이 지원을 위한 코드수정
	BitBlt(hdcMem, 0, 0, width, height, hdcScreen, rect.left, rect.top, SRCCOPY);

	BITMAPINFO bitmapInfo{};
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	GetDIBits(hdcMem, hBitmap, 0, height, nullptr, &bitmapInfo, DIB_RGB_COLORS);

	vector<byte> bitmapDatas(bitmapInfo.bmiHeader.biSizeImage);

	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	GetDIBits(hdcMem, hBitmap, 0, height, bitmapDatas.data(), &bitmapInfo, DIB_RGB_COLORS);

	CaptureInfo ci{};

	/*if (GetWindowRect(appWnd, &rect)) {
		width = rect.right;
		height = rect.top;
	}*/
	//2024-08-12 다중디스플레이 지원을 위한 코드수정

	ci.width = width;
	ci.height = height;
	ci.bitmapDatas = bitmapDatas;

	// Clean up
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(appWnd, hdcScreen);

	return ci;
}

HWND CAPTURE::GetAppWindow(const string title) {
	HWND hwnd = FindWindowA(NULL, title.c_str());

	return hwnd;
}

HWND CAPTURE::GetAppWindow(const DWORD pid) {

	HWND topWnd = GetTopWindow(nullptr);
	if (topWnd == NULL) {
		WarningMessage("Failed to get app window");
		return NULL;
	}

	HWND cWnd;
	DWORD cPid = 0;

	cWnd = topWnd;
	do {
		GetWindowThreadProcessId(cWnd, &cPid);
		if (cPid == pid) {
			return cWnd;

			break;
		}

	} while (GetNextWindow(cWnd, GW_HWNDNEXT));

	return NULL;
}

int CAPTURE::GetScreen() {
	return screen;
}

bool CAPTURE::SetScreen(unsigned int idx) {
	if (idx > totalScreens) {
		WarningMessage("Failed to set screen");
		return false;
	}
	screen = idx;
}

int CAPTURE::GetScreensCount() {
	return totalScreens;
}