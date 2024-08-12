#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "H:\opencv\sources\include\opencv2\opencv.hpp"

using namespace std;
using namespace cv;

typedef class DETECTOR {
private:
	vector<Mat> targets{};
	Mat* target = nullptr;

	void WarningMessage(const string& msg);

	Mat to_mat(vector<uchar>& image, const Size& size);

public:
	DETECTOR();
	~DETECTOR();

	bool LoadTarget(const string& fileName);
	bool CompareTarget(vector<uchar>& image, const Mat* mask = nullptr, const Size& size = Size(1920, 1080), float threadShold = 0.9, bool show = false);
	int CompareTarget(const Mat* src, const Mat* mask = nullptr, float threadShold = 0.9, bool show = false);
	Mat GetCurrentTarget();
	bool SelectTarget(const int idx);
	int GetTargetsCount();

}Detector;