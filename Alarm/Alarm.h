#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

namespace ALARM_SPACE {
	enum STATUS {
		ERR = -1,
		CONNECT,
		CLOSE
	};
};

typedef class ALARM {
public:
	

private:
	SOCKET serverSocket{};
	sockaddr_in serverAddr{};

	HANDLE CommThreadHandle;
	
	int status;

	unsigned int interval = 0;

	std::string data;

	static DWORD WINAPI CommThreadFunc(void* args);

	void WarningMessage(const std::string msg);
	bool InitConnect(const std::string ip,const byte port);

protected:
	SOCKET GetServerSocket();

public:
	ALARM(const unsigned int time = 1000);
	~ALARM();


	bool AlarmOn(const std::string ip, const byte port);
	void AlarmOff();

	int GetStatus();

	std::string GetData();
	bool SetData(const std::string& data);

	int GetInterval();
	bool SetInterval(const unsigned int time);

}Alarm;