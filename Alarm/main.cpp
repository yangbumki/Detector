#include "Alarm.h"

#include <Windows.h>

#include <iostream>
#include <conio.h>

#define ESC				27

int main() {
	Alarm al;

	if (al.AlarmOn("127.0.0.1", 8986)) {
		std::cout << "===ALARM START===\n";
	}
	else {
		std::cerr << "Failed to alarm on\n";
		return -1;
	}
	
	while (1) {
		Sleep(1000);

		if (al.GetData().compare("FIND") == 0) {
			MessageBeep(-1);
			MessageBoxA(NULL, "FINDED", "ALARM", NULL);
		}

		std::cout << "Recved : " << al.GetData() << std::endl;

		if (_kbhit()) {
			if (_getch() == ESC) break;
		}
	}
}