#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

class CUdpListener
{
private:
	static CUdpListener* pInstance;
	CUdpListener();
	~CUdpListener();

public:
	static CUdpListener* GetInstance();
	static void DeleteInstance();

private:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

public:
	bool Init(PCSTR _ip, u_short _port);
	void Loop();

	SOCKET GetSocket() { return m_socket; }
};