#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUdpListener
{
protected:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

public:
	CUdpListener();
	~CUdpListener();

	bool Init(PCSTR _ip, u_short _port);
	void Loop();
	void CloseSocket();
};