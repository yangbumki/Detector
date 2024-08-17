#include "Masker.h"

MASKER::MASKER() {
}

MASKER::~MASKER() {
}

void MASKER::GetRectWithMouseCallBack(int event, int x, int y, int flags, void* args) {
	vector<Point>* points = (vector<Point>*)args;

	/*cout << "EVENT : " << event << endl;*/

	switch (event) {
	case cv::MouseEventTypes::EVENT_LBUTTONUP:
		points->push_back(Point(x, y));
		break;
	}
}

void MASKER::WarningMessage(const string& msg) {
	cout << "===MASKER===\n";
	cout << msg << endl;
}

Mat MASKER::to_mat(vector<uchar>& image, const Size& size) {
	Mat img(size, CV_8UC4, image.data());

	if (img.empty()) {
		WarningMessage("Failed to convert mat");
		return {};
	}

	Mat invImg;

	flip(img, invImg, 0);

	return invImg;
}

void MASKER::GetHSVRangeMaskWithGUI(vector<uchar>& image, const Size& size) {
	Mat src = to_mat(image, size);
	if (!src.empty()) {
		namedWindow("CHECK_MASK");

		int hue1 = 0, hue2 = 0, sat1 = 0, sat2 = 0;

		createTrackbar("LOW_HUE1", "CHECK_MASK", &hue1, 255, nullptr);
		createTrackbar("LOW_SAT1", "CHECK_MASK", &hue2, 255, nullptr);
		createTrackbar("LOW_HUE2", "CHECK_MASK", &sat1, 255, nullptr);
		createTrackbar("LOW_SAT2", "CHECK_MASK", &sat2, 255, nullptr);

		Mat tmpMask{};

		cvtColor(src, src, COLOR_BGR2HSV);


		while (1) {
			inRange(src, Scalar(hue1, sat1, 0), Scalar(hue2, sat2, 255), tmpMask);
			imshow("CHECK_MASK", tmpMask);
			waitKey(1);
			if (cv::getWindowProperty("CHECK_MASK", WND_PROP_VISIBLE) < 1) break;
		}
		//destroyAllWindows();
		//destroyWindow("CHECK_MASK");
	}
}

void MASKER::GetHSVRangeMaskWithGUI(const Mat& src) {
	if (!src.empty()) {
		namedWindow("CHECK_MASK");

		int hue1 = 0, hue2 = 0, sat1 = 0, sat2 = 0, val1 = 0, val2 = 0;

		createTrackbar("LOW_HUE", "CHECK_MASK", &hue1, 255, nullptr);
		createTrackbar("LOW_SAT", "CHECK_MASK", &sat1, 255, nullptr);
		createTrackbar("LOW_VAL", "CHECK_MASK", &val1, 255, nullptr);
		createTrackbar("HIGH_HUE", "CHECK_MASK", &hue2, 255, nullptr);
		createTrackbar("HIGH_SAT", "CHECK_MASK", &sat2, 255, nullptr);
		createTrackbar("HIGH_VAL", "CHECK_MASK", &val2, 255, nullptr);
		//createButton("SAVE", nullptr, NULL, QT_PUSH_BUTTON, 1);
		//createButton("Save", nullptr);

		Mat tmpMask{};
		Mat tmpSrc{};

		cvtColor(src, tmpSrc, COLOR_BGR2HSV);

		while (1) {
			inRange(tmpSrc, Scalar(hue1, sat1, val1), Scalar(hue2, sat2, val2), tmpMask);

			imshow("CHECK_MASK", tmpMask);
			waitKey(1);
			if (cv::getWindowProperty("CHECK_MASK", WND_PROP_VISIBLE) < 1) break;
		}
	}
}

void MASKER::GetRectRangeMaskWithGUI(std::vector<uchar>& image, const Size& size) {
	Mat src = to_mat(image, size);

	if (!src.empty()) {
		namedWindow("MASK");

		vector<Point> pointers;

		setMouseCallback("MASK", GetRectWithMouseCallBack, (void*)&pointers);


		while (waitKey(1) != 27) {
			imshow("MASK", src);

			if (!pointers.empty()) {
				for (auto& point : pointers) {
					circle(src, point, 20, Scalar(0, 0, 255));
				}
				/*pointers.clear();
				pointers.resize(0);*/
			}
		}

		if (pointers.empty()) {
			destroyAllWindows();
			return;
		}

		cout << "===POINT1=== \n";
		cout << "pointers[0].x : " << pointers[0].x << endl;
		cout << "pointers[0].y : " << pointers[0].y << endl;
		cout << "===POINT2=== \n";
		cout << "pointers[1].x : " << pointers[1].x << endl;
		cout << "pointers[1].y : " << pointers[1].y << endl;
	}

	destroyAllWindows();
}

void MASKER::GetRectRangeMaskWithGUI(const cv::Mat& src) {
	if (!src.empty()) {
		namedWindow("MASK");

		vector<Point> pointers;

		setMouseCallback("MASK", GetRectWithMouseCallBack, (void*)&pointers);


		while (waitKey(1) != 27) {
			imshow("MASK", src);

			if (!pointers.empty()) {
				for (auto& point : pointers) {
					circle(src, point, 20, Scalar(0, 0, 255));
				}
				/*pointers.clear();
				pointers.resize(0);*/
			}
		}

		cout << "===POINT1=== \n";
		cout << "pointers[0].x : " << pointers[0].x << endl;
		cout << "pointers[0].y : " << pointers[0].y << endl;
		cout << "===POINT2=== \n";
		cout << "pointers[1].x : " << pointers[1].x << endl;
		cout << "pointers[1].y : " << pointers[1].y << endl;
	}

	destroyAllWindows();
}

bool MASKER::CreateMask(vector<uchar>& image, const Size& size, const unsigned int hue1, const unsigned int sat1, const unsigned int val1, const unsigned int hue2, const unsigned int sat2, const unsigned int val2) {
	Mat src = to_mat(image, size);
	Mat mask{};

	Scalar lowerb(hue1, sat1, val1);
	Scalar upperb(hue2, sat2, val2);

	if (src.empty()) {
		WarningMessage("Failed to create mask");
		return false;
	}

	inRange(src, lowerb, upperb, mask);

	masks.push_back(mask);
	this->mask = &masks.at(masks.size() - 1);

	return true;
}

bool MASKER::CreateMask(const Mat* src, const unsigned int hue1, const unsigned int sat1, const unsigned int val1, const unsigned int hue2, const unsigned int sat2, const unsigned int val2) {
	Mat mask{};

	Scalar lowerb(hue1, sat1, val1);
	Scalar upperb(hue2, sat2, val2);

	if (src == nullptr) {
		WarningMessage("Failed to create mask");
		return false;
	}

	inRange(*src, lowerb, upperb, mask);

	masks.push_back(mask);
	this->mask = &masks.at(masks.size() - 1);

	return true;
}

bool MASKER::CreateMask(std::vector<uchar>& image, const Size& size, Rect rect) {
	Mat src = to_mat(image, size);

	if (src.empty()) {
		WarningMessage("Failed to create mask");
		return false;
	}

	Mat mask = src(rect);

	/*imshow("SRC", src);
	imshow("Mask", mask);
	waitKey();*/

	masks.push_back(mask);
	this->mask = &masks.at(masks.size() - 1);

	rects.push_back(rect);
	this->rect = &rects.at(rects.size() - 1);

	return true;
}

bool MASKER::CreateMask(const Mat& src, Rect rect) {
	if (src.empty()) {
		WarningMessage("Failed to create mask");
		return false;
	}

	Mat mask = src(rect);

	masks.push_back(mask);
	this->mask = &masks.at(masks.size() - 1);

	rects.push_back(rect);
	this->rect = &rects.at(rects.size() - 1);

	return true;
}

bool MASKER::SaveMask(string fileName) {
	string path = "..\\Images\\Masks\\";

	path.append(fileName);

	if (mask->empty()) {
		WarningMessage("Failed to save mask");
		return false;
	}

	/*if (!fileName.compare("*.png") != 0) {
		WarningMessage("Failed to save mask");
		return false;
	}*/

	if (!imwrite(fileName, *mask)) {
		WarningMessage("Failed to save mask");
		return false;
	}

	return true;
}

bool MASKER::LoadMask(string fileName) {
	string path = "..\\Images\\Masks\\";

	path.append(fileName);

	Mat loadMask = imread(path, IMREAD_GRAYSCALE);
	if (loadMask.empty()) {
		WarningMessage("Failed to load mask");
		return false;
	}

	masks.push_back(loadMask);

	return true;
}

bool MASKER::ResetMask() {
	if (masks.empty()) {
		WarningMessage("Aleady nothing exist masks");
		return false;
	}

	masks.clear();
	masks.resize(0);

	return true;
}

Mat MASKER::GetCurrentMask() {
	if (mask == nullptr) {
		WarningMessage("Failed to get current mask");
		return {};
	}
	return mask->clone();
}

bool MASKER::SetMask(const int idx) {
	auto maxIdx = masks.size();

	if (idx < 0 && idx > maxIdx) {
		WarningMessage("Failed to select mask");
		return false;
	}

	mask = &masks[idx];

	return true;
}

int MASKER::GetMasksCount() {
	return masks.size();
}

Rect MASKER::GetCurrenRect() {
	return *rect;
}

bool MASKER::SetRect(const unsigned int idx) {
	if (idx > rects.size()) {
		WarningMessage("Failed to set rect");
		return false;
	}

	rect = &rects[idx];

	return true;
}