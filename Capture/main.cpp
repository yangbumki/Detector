#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

#include "opencv2/opencv.hpp"

#pragma comment(lib, "H:\\opencv\\build\\x64\\vc16\\lib\\opencv_world4100.lib")
#pragma comment(lib, "H:\\opencv\\build\\x64\\vc16\\lib\\opencv_world4100d.lib")

using namespace std;
using namespace cv;

BOOL Monitorenumproc(HMONITOR hMonitor,HDC hdc, LPRECT rect, LPARAM args) {
	vector<RECT>* rects = (vector<RECT>*)args;

	/*cout<<"rect->left  : "<<rect->left<<endl;
	cout<<"rect->right : "<<rect->right<<endl;
	cout<<"rect->top : "<<rect->top<<endl;
	cout<<"rect->bottom : "<<rect->bottom<<endl;*/

	rects->push_back(*rect);

	return true;
}


int main() {
	vector<RECT> rects;

	vector<vector<byte>> bitmapDatas;

	EnumDisplayMonitors(NULL, NULL, Monitorenumproc, (LPARAM)&rects);

	for (auto& rect : rects) {
		HDC hdc = GetDC(NULL);
		HDC hdcMem = CreateCompatibleDC(hdc);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP hBmold = (HBITMAP)SelectObject(hdcMem, hBitmap);

		BitBlt(hdcMem, 0, 0, width, height, hdc, rect.left, rect.top, SRCCOPY);

		BITMAPINFO bi{};

		ZeroMemory(&bi, sizeof(BITMAPINFO));
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		GetDIBits(hdcMem, hBitmap, 0, height, nullptr, &bi, DIB_RGB_COLORS);

		vector<byte> bitmapData(bi.bmiHeader.biSizeImage);

		bi.bmiHeader.biCompression = BI_RGB;
		GetDIBits(hdcMem, hBitmap, 0, height, bitmapData.data(), &bi, DIB_RGB_COLORS);

		bitmapDatas.push_back(bitmapData);

		SelectObject(hdcMem, hBmold);
		DeleteObject(hBitmap);
		DeleteDC(hdcMem);
		ReleaseDC(NULL, hdc);
	}

	for (const auto& bData : bitmapDatas) {
		int width = rects[0].right - rects[0].left;  // 첫 번째 모니터의 크기 사용
		int height = rects[0].bottom - rects[0].top;

		Mat img(height, width, CV_8UC4, (void*)bData.data());

		if (img.empty()) {
			cerr << "Failed to convert\n";
			return -1;
		}

		Mat invImg;
		flip(img, invImg, 0);

		static int idx = 0;
		imshow("Image " + to_string(idx++), invImg);
	}

	while (waitKey(0) != 27) {  // ESC 키를 누르면 종료
		// 대기
	}
	
	return 0;
}