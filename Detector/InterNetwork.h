#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib , "ws2_32.lib")

namespace inter_network{
	enum STATUS {
		ERR = 1,
		INIT,
		OPEN,
		STOP,
		CLOSE
	};
};

typedef class INTER_NETWORK {
private:
	byte status;
	
	HANDLE CommThreadHandle;

	SOCKET	socket,
			clientSock;
	sockaddr_in clientAddr;
	
	std::string buffer;

	unsigned int intervalTime;

	static DWORD WINAPI CommThreadFunc(void* args);

	void WarningMessage(const std::string msg);
	bool InitSocket(const std::string ip,const byte port);

public:
	INTER_NETWORK(unsigned int time = 1000);
	~INTER_NETWORK();

	bool Open(const std::string ip, const byte port);
	bool Stop();
	void Close();

	int GetStatus();

	bool SetData(const std::string msg);
	std::string GetData();

	bool SetInterval(const unsigned int time);
	int GetInterval();

}InterNetwork;