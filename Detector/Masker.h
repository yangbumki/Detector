#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "H:\opencv\sources\include\opencv2\opencv.hpp"

using namespace std;
using namespace cv;



typedef class MASKER {
private:
	vector<Mat> masks{};	
	Mat* mask = nullptr;

	vector<Rect> rects{};
	Rect* rect;

	static void GetRectWithMouseCallBack(int event, int x, int y, int flags, void* args);

	void WarningMessage(const string& msg);
	Mat to_mat(vector<uchar>& image, const Size& size);


public:
	MASKER();
	~MASKER();

	void GetHSVRangeMaskWithGUI(std::vector<uchar>& image, const Size& size = Size(1920, 1080));
	void GetHSVRangeMaskWithGUI(const cv::Mat& src);

	void GetRectRangeMaskWithGUI(std::vector<uchar>& image, const Size& size = Size(1920, 1080));
	void GetRectRangeMaskWithGUI(const cv::Mat& src);

	bool CreateMask(std::vector<uchar>& image, const Size& size, const unsigned int hue1, const unsigned int sat1, const unsigned int val1, const unsigned int hue2, const unsigned int sat2, const unsigned int val2);
	bool CreateMask(const Mat* src, const unsigned int hue1, const unsigned int sat1, const unsigned int val1, const unsigned int hue2, const unsigned int sat2, const unsigned int val2);

	bool CreateMask(std::vector<uchar>& image, const Size& size, Rect rect);
	bool CreateMask(const Mat& src, Rect rect);

	bool SaveMask(string fileName);
	bool LoadMask(string fileName);
	bool ResetMask();
	Mat GetCurrentMask();
	bool SetMask(const int idx);
	int GetMasksCount();

	Rect GetCurrenRect();
	bool SetRect(const unsigned int idx = 0);

}Masker;