#include "Alarm.h"

using namespace std;
using namespace ALARM_SPACE;

DWORD WINAPI ALARM::CommThreadFunc(void* args) {
	ALARM* al = (ALARM*)args;

	char recvBuf[1024]{};
	string echoData;

	while (1) {
		Sleep(al->GetInterval());

		switch (al->GetStatus()) {
		case STATUS::ERR:
		case STATUS::CLOSE:
			goto END;

		case STATUS::CONNECT:
			if (recv(al->GetServerSocket(), recvBuf, sizeof(recvBuf), 0) == 0) {
				cerr << "Failed to recv data\n";
				cerr << "The server is closed.\n";
				goto END;
			}
			
			al->SetData(recvBuf);

			echoData = al->GetData();

			if(send(al->GetServerSocket(), echoData.c_str(), echoData.size(), 0) == SOCKET_ERROR) {
				cerr << "Failed to send data\n";
				cerr << "The server is closed. \n";
				goto END;
			}
		}
	}

END:
	return 0;
}



ALARM::ALARM(const unsigned int time)
	:interval(time) {
}

ALARM::~ALARM() {
	if(status != CLOSE) 
		AlarmOff();
}

SOCKET ALARM::GetServerSocket() {
	return serverSocket;
}

void ALARM::WarningMessage(const string msg) {
	cerr << "===ALARAM=== \n";
	cerr << msg << endl;
}

bool ALARM::InitConnect(const string ip, const byte port) {
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == SOCKET_ERROR) {
		WarningMessage("Failed to call socket\n");
		return false;
	}

	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);

	if (connect(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		WarningMessage("Failed to connect");
		return false;
	}

	
}
bool ALARM::AlarmOn(const std::string ip, const byte port) {
	if (!InitConnect(ip, port)) {
		WarningMessage("Failed to init connect");
		return false;
	}

	CommThreadHandle = CreateThread(NULL, 0, CommThreadFunc, this, NULL, NULL);
	if (CommThreadHandle == NULL) {
		WarningMessage("Failed to create thread");
		return false;
	}

	return true;
}

void ALARM::AlarmOff() {
	this->status = STATUS::CLOSE;

	WaitForSingleObject(CommThreadHandle, INFINITE);
	CloseHandle(CommThreadHandle);

	closesocket(serverSocket);
	WSACleanup();
}

int ALARM::GetStatus() {
	return status;
}

string ALARM::GetData() {
	return data;
}

bool ALARM::SetData(const string& str) {
	data = str;

	return true;
}

int ALARM::GetInterval() {
	return interval;
}

bool ALARM::SetInterval(const unsigned int value) {
	this->interval = value;

	return true;
}