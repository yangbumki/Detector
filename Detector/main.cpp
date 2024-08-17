#include <iostream>
#include <conio.h>

#include <windows.h>

#include "Capture.h"
#include "Detector.h"
#include "Masker.h"

#define ESC			27

#pragma comment(lib, "opencv_world4100.lib")

int main() {
	Capture capture;
	Detector detector;
	Masker masker;

	CaptureInfo capInfo{};

	string input;

	while (1) {
		cout << "===DETECTOR==== \n";
		cout << "1. Capture \n";
		cout << "2. Get mask Value \n";
		cout << "3. Save mask \n";
		cout << "4. Load mask \n";
		cout << "5. Load target \n";
		cout << "6. Start detection \n";
		cout << "9. Exit \n";
		cout << "INPUT : ";

		cin >> input;

		switch (atoi(input.c_str())) {
		case 1:
			cout << "===Monitor lists=== \n";

			for (int idx = 0; idx < capture.GetScreensCount(); idx++)
				cout << to_string(idx + 1).append(". Monitor") << endl;

			cout << "INPUT : ";
			cin >> input;

			if (capture.SetScreen(atoi(input.c_str()) - 1)) {
				capInfo = capture.CaptureScreen();
				cout << "Success to capture \n";
			}
			break;
		case 2:
			cout << "===Mask algorithm=== \n";
			cout << "1. HSV \n";
			cout << "2. Rect \n";

			cout << "INPUT : ";
			cin >> input;

			switch (atoi(input.c_str())) {
			case 1: {
				masker.GetHSVRangeMaskWithGUI(detector.GetCurrentTarget());

				byte lowerb[3];
				byte upperb[3];

				cout << "===HSV=== \n";
				cout << "Hue1: "; cin >> lowerb[0];
				cout << "Sat1: "; cin >> lowerb[1];
				cout << "Val1: "; cin >> lowerb[2];
				cout << "========= \n";
				cout << "Hue2: "; cin >> upperb[0];
				cout << "Sat2: "; cin >> upperb[1];
				cout << "Val2: "; cin >> upperb[2];

				auto target = detector.GetCurrentTarget();

				masker.CreateMask(&target, lowerb[0], lowerb[1], lowerb[2], upperb[0], upperb[1], upperb[2]);
			}
				  break;
			case 2:
				masker.GetRectRangeMaskWithGUI(capInfo.bitmapDatas);

				Rect rect{};

				Point pt1,
					pt2;

				cout << "===Point1=== \n";
				cout << "Point1.x: ";  cin >> pt1.x;
				cout << "Point1.y : "; cin >> pt1.y;
				cout << "===Point2=== \n";
				cout << "Point2.x: ";  cin >> pt2.x;
				cout << "Point2.y : "; cin >> pt2.y;


				rect.width = pt2.x >= pt1.x ? pt2.x - pt1.x : pt1.x - pt2.x;
				rect.height = pt2.y >= pt1.y ? pt2.y - pt1.y : pt1.y - pt2.y;

				rect.x = pt2.x <= pt1.x ? pt2.x : pt1.x;
				rect.y = pt2.y <= pt1.y ? pt2.y : pt1.y;

				masker.CreateMask(capInfo.bitmapDatas, Size(capInfo.width, capInfo.height), rect);
				break;
			}
			break;
		case 3:
			cout << "===Save Mask===\n";
			cout << "Enter the file name. \n";
			cin >> input;

			if (masker.SaveMask(input)) {
				cout << "Success to save mask\n";
			}
			break;
		case 4:
			cout << "===Load Mask===\n";
			cout << "Enter the file name. \n";

			cout << "INPUT :";
			cin >> input;

			if (masker.LoadMask(input)) {
				cout << "Success to load mask\n";
			}
			break;
		case 5:
			cout << "===Load Target===\n";
			cout << "Enter the file name. \n";

			cout << "INPUT :";
			cin >> input;

			if (detector.LoadTarget(input)) {
				cout << "Success to load target. \n";
			}
			break;
		case 6:
			cout << "===Start detection===\n";
			cout << "1. Full Screen\n";
			cout << "2. Map\n";

			cout << "INPUT : ";
			cin >> input;

			switch (atoi(input.c_str())) {
			case 1: {
				auto screen = capture.CaptureScreen();

				if (detector.CompareTarget(screen.bitmapDatas)) {
					cout << "Detected\n";
				}
				break;
			}
			case 2: {
				int cnt = 0;
				int targetCnt = detector.GetTargetsCount();

				float* prescision = new float[targetCnt];
				float tmpPrescision;

				unsigned int* maxCnt = nullptr;
				maxCnt = new unsigned int[targetCnt];
				
				//정확도 설정
				for (int idx = 0; idx < detector.GetTargetsCount(); idx++) {
					detector.SelectTarget(idx);

					while (1) {
						cout << "===Prescision calibrate=== \n";
						cout << "If you want to end, enter the -1" << endl;
						cout << "0.0 ~ 1.0\n";

						cout << "INPUT : " << endl;
						cin >> tmpPrescision;

						if (tmpPrescision <= -1.0f) break;
						prescision[idx] = tmpPrescision;

						Mat mask = masker.GetCurrentMask();

						cnt = detector.CompareTarget(&mask, nullptr, prescision[idx], true);

						cout << "Count : " << cnt << endl;
					}

					cout << "===Count Setting=== \n";
					cout << "Enter the max count of this item\n";

					cout << "INPUT : " << endl; 
					cin >> maxCnt[idx];

					cout << "precision[" << idx << "] :" << prescision[idx] << endl;
					cout << "maxCnt[" << idx << "] :" << maxCnt[idx] << endl;
				}

				//실제 탐지
				while (1) {
					Sleep(100);

					capInfo = capture.CaptureScreen();
					masker.CreateMask(capInfo.bitmapDatas, Size(capInfo.width, capInfo.height), masker.GetCurrenRect());

					for (int idx = 0; idx < detector.GetTargetsCount(); idx++) {
						detector.SelectTarget(idx);

						Mat mask = masker.GetCurrentMask();

						cnt = detector.CompareTarget(&mask, nullptr, prescision[idx]);

						if (cnt >= maxCnt[idx]) {
							cout << "Detected!!!" << endl;
							
							MessageBeep(-1);
							MessageBoxA(NULL, "DETECTED", "DETECTED", NULL);

						}

						cout << "Count : " << cnt << endl;
						cout << "Max Count : " << maxCnt[idx]<< endl;
					}

					if (!masker.ResetMask()) {
						cerr << "Failed to reset mask\n";
					}

					if (_kbhit()) {
						
						if (_getch() == ESC) {
							delete(maxCnt);
							delete(prescision);
							break;
						}
					}
				}
				break;
			}
			default:
				break;
			}
			break;

		case 9:
			goto EXIT;
			break;
		default:
			cout << "Unknown values. \n";
		}
	}


EXIT:
	return 0;
};