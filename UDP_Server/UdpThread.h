#pragma once
#include <winsock2.h>

class CUdpThread
{
private:
	SOCKET m_socket;

public:
	CUdpThread();
	~CUdpThread();

	bool Start(SOCKET _socket);
	static unsigned int _stdcall UdpFunc(void* _pArgs);

	void RunLoop();
};