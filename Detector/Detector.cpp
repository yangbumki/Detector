#include "Detector.h"

DETECTOR::DETECTOR() {
}

DETECTOR::~DETECTOR() {
}

void DETECTOR::WarningMessage(const string& msg) {
	cout << endl;
	cout << "===DETECTOR===" << endl;
	cerr << msg << endl;
}

Mat DETECTOR::to_mat(vector<uchar>& image, const Size& size) {
	Mat img(size, CV_8UC4, image.data());

	if (img.empty()) {
		WarningMessage("Failed to convert mat");
		return {};
	}

	Mat invImg;

	flip(img, invImg, 0);

	return invImg;
}

bool DETECTOR::LoadTarget(const string& fileName) {
	//2024-08-09 폴더 이름 변경
	//string path = "..\\Images\\Detection\\";
	string path = "..\\Images\\Targets\\";

	path.append(fileName);

	Mat targetImage = imread(path, IMREAD_UNCHANGED);
	if (targetImage.empty()) {
		WarningMessage("Failed to register target image");
		return false;
	}

	targets.push_back(targetImage);
	target = &targets.at(targets.size()-1);

	return true;
}

bool DETECTOR::CompareTarget(vector<uchar>& image, const Mat* mask,  const Size& size, float threadShold, bool show) {
	if (targets.empty()) {
		WarningMessage("Not exist target images");
		return false;
	}

	Mat screenshot = to_mat(image, size);

	Mat ret{};

	if (target->type() != screenshot.type()) {
		cout << "target->type() : " << target->type() << endl;
		cout << "screenshot.type() : " << screenshot.type() << endl;

		return false;
	}

	mask == nullptr ? matchTemplate(screenshot, *target, ret, TM_CCOEFF_NORMED) : matchTemplate(screenshot, *target, ret, TM_CCOEFF_NORMED, *mask);

	for (int row = 0; row < ret.rows; row++) {
		for (int col = 0; col < ret.cols; col++) {
			if (ret.at<float>(row, col) > threadShold) {
				rectangle(screenshot, Point(col, row), Point(col + target->cols, row + target->rows), Scalar(0, 0, 255), 1);
			}
		}
	}
	//윈도우 크기 변경 후 Mat 반영 시, Mat 크기로 조정됨
	/*namedWindow("RESULT");
	resizeWindow("RESULT", 1024, 768);*/

	/*resize(screenshot, screenshot, Size(1024, 768));
	*/

	if (show) {
		imshow("RESULT", screenshot);
		waitKey();
	}

	return true;
}

int DETECTOR::CompareTarget(const Mat* src, const Mat* mask, float threadShold, bool show) {
	if (src->empty()) {
		WarningMessage("Failed to compare target.");
		return -1;
	}

	//UMat uSrc{}, uMask{}, uTarget{}, uRet{};

	//uSrc = src->getUMat(ACCESS_READ);
	///*uMask = mask->getUMat(ACCESS_READ);*/
	//uTarget = target->getUMat(ACCESS_READ);

	//mask == nullptr ? matchTemplate(uSrc, uTarget, uRet, TM_CCOEFF_NORMED) : matchTemplate(uSrc, uTarget, uRet, TM_CCOEFF_NORMED, uMask);

	Mat ret{};

	/*ret = uRet.getMat(ACCESS_READ);*/

	mask == nullptr ? matchTemplate(*src, *target, ret, TM_CCOEFF_NORMED) : matchTemplate(*src, *target, ret, TM_CCOEFF_NORMED, *mask);

	int cnt = 0;

	vector<Rect> rects;
	bool dup = false;

	for (int row = 0; row < ret.rows; row++) {
		for (int col = 0; col < ret.cols; col++) {
			if (ret.at<float>(row, col) > threadShold) {
				Rect rc{};

				rc.x = col;
				rc.y = row;
				rc.width = 10;
				rc.height = 10;

				for (const auto& rect : rects) {
					//2024-08-17 겹칠경우 무조건 x 점이 교차하게되어있음
					if (rect.x <= rc.x && rc.x <= (rect.x + rect.width)) {
						if (rect.y <= rc.y && rc.y <= (rect.y + rect.height)) {
							WarningMessage("This target aleady been detected");
							dup = true;
							break;
						}
					}
				}

				if (!dup) {
					rectangle(*src, rc, Scalar(0, 0, 255));
					rects.push_back(rc);
					cnt++;
				}

				dup = false;
			}
		}
	}

	if (show) {
		imshow("Image", *src);
		waitKey();
		destroyAllWindows();
	}

	return cnt;
}

Mat DETECTOR::GetCurrentTarget() {
	if (target == nullptr) {
		return {};
	}

	return *target;
}

bool DETECTOR::SelectTarget(const int idx) {
	auto maxIdx = targets.size();

	if (idx < 0 && idx > maxIdx) {
		WarningMessage("Failed to select mask");
		return false;
	}

	target = &targets[idx];

	return true;
}

int DETECTOR::GetTargetsCount() {
	return targets.size();
}