#include "InterNetwork.h"

using namespace std;
using namespace inter_network;

DWORD WINAPI INTER_NETWORK::CommThreadFunc(void* args) {
	InterNetwork* iw = (InterNetwork*)args;
	
	while (1) {
		switch (iw->GetStatus()) {
		case STATUS::ERR:
		case STATUS::CLOSE:
			goto END;
			break;
		case STATUS::INIT:
			continue;
			break;
		case STATUS::STOP:
			while (iw->GetStatus() != STATUS::STOP);
			break;
		case STATUS::OPEN:
			Sleep(iw->GetInterval());

			if (!send(iw->clientSock, iw->GetData().c_str(), iw->GetData().size(), 0)) {
				cerr << "===COMM THREAD===\n";
				cerr << "Failed to send data\n";
			}

			char recvBuf[1024]{};

			if (recv(iw->clientSock, recvBuf, sizeof(recvBuf), 0) == 0) {
				cerr << "===COMM THREAD===\n";
				cerr << "Failed to recv data\n";
				//비정상 접속 종료
				goto END;
			}
			break;
		}
	}

END:

	return 0;
}

INTER_NETWORK::INTER_NETWORK(unsigned int time) {
	status = STATUS::INIT;

	SetInterval(time);
}

INTER_NETWORK::~INTER_NETWORK() {
	Close();
}



void INTER_NETWORK::WarningMessage(const string msg) {
	cerr << "===INTER_NETWORK=== \n";
	cerr << msg << endl;
}

bool INTER_NETWORK::InitSocket(const string ip, const byte port) {
	if (socket != NULL) {
		WarningMessage("Aleady exist socket");
		return false;
	}

	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket == NULL) {
		WarningMessage("Failed to call socket");
		return false;
	}

	sockaddr_in serverAddr{};

	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);
	
	if (bind(this->socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
		WarningMessage("Failed to bind socket");
		return false;
	}

	if (listen(this->socket, 0) != 0) {
		WarningMessage("Failed to listen socket");
		return false;
	}

	return true;
}

bool INTER_NETWORK::Open(const string ip, const byte port) {
	if (!InitSocket(ip, port)) {
		WarningMessage("Failed to init socket");
		return false;
	}

	int addrLen = sizeof(clientAddr);

	clientSock = accept(this->socket, (sockaddr*)&clientAddr, &addrLen);
	if (clientSock == SOCKET_ERROR) {
		WarningMessage("Failed to accept socket");
		return false;
	}

	status = STATUS::OPEN;

	CommThreadHandle = CreateThread(NULL, 0, CommThreadFunc, this, NULL, NULL);
	if (CommThreadHandle == NULL) {
		WarningMessage("Failed to create thread");
		return false;
	}

	return true;
}

bool INTER_NETWORK::Stop() {
	if (status == STATUS::STOP) {
		WarningMessage("Aleady status stopped");
		return false;
	}

	status = STATUS::STOP;

	return true;
}

void INTER_NETWORK::Close() {
	status = STATUS::CLOSE;

	WaitForSingleObject(CommThreadHandle, INFINITE);
	CloseHandle(CommThreadHandle);

	closesocket(socket);
	closesocket(clientSock);

	WSACleanup();
}

int INTER_NETWORK::GetStatus() {
	return this->status;
}

bool INTER_NETWORK::SetData(const string msg) {
	buffer = msg;

	return true;
}

string INTER_NETWORK::GetData() {
	return buffer;
}

bool INTER_NETWORK::SetInterval(const unsigned int time) {
	intervalTime = time;

	return true;
}

int INTER_NETWORK::GetInterval() {
	return this->intervalTime;
}