// 입출력 헤더
#include <iostream>
//opencv 헤더
#include <opencv2/opencv.hpp>

//모드 지시어
#define DEBUGGING_MODE		1
#define RELEASE_MDOE		0

//전체 라이브러리 링크
//디버깅 라이브러리 링크
#if DEBUGGING_MODE
#pragma comment(lib, "opencv_world4100d.lib")
#elif RELEASE_MODE
//릴리즈 라이브러리 링크
#pragma comment(lib, "opencv_world4100.lib")
#endif

using namespace cv;
using namespace std;

//typedef struct RANGE_INFO {
//	int hue1;
//	int hue2;
//	int sat1;
//	int sat2;
//}RagneInfo;
//
//Mat colorHsv{};
//Mat mask;
//Mat colorImage = imread("Test.png");
//
//RagneInfo ri{};
//
//void hsvRangeChanged(int, void*) {
//	Scalar lowerb(ri.hue1, ri.sat1, 0);
//	Scalar upperb(ri.hue2, ri.sat2, 0);
//
//	inRange(colorImage, lowerb, upperb, mask);
//
//	imshow("mask", mask);
//}
//
//int main() {
//	Mat grayImage = imread("Test.png", IMREAD_GRAYSCALE);
//
//	
//	cvtColor(colorImage, colorHsv, COLOR_BGR2HSV);
//
//	imshow("src", colorImage);
//
//	namedWindow("mask");
//
//	createTrackbar("MASK1", "mask", &ri.hue1, 180, hsvRangeChanged);
//	createTrackbar("MASK2", "mask", &ri.hue2, 180, hsvRangeChanged);
//	createTrackbar("MASK3", "mask", &ri.sat1, 255, hsvRangeChanged);
//	createTrackbar("MASK4", "mask", &ri.sat2, 255, hsvRangeChanged);
//	
//	waitKey();
//	destroyAllWindows();Z
//}

int main() {
	Mat src = imread("..\\Images\\Targets\\Test.png", IMREAD_UNCHANGED);
	if (src.empty()) {
		cerr << "Failed to read src \n";
		return -1;
	}

	Mat dst(src.size(), src.type());

	if (src.type() != CV_8UC4) {
		cout << "src.type : " << src.type() << endl;
		return -1;
	}

	vector<Mat> channels(4);
	split(src, channels);

	

	imshow("Alpha", srcAlpha);
	waitKey();
}


