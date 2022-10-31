#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "UdpThread.h"

class CUdpClient
{
protected:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

	CUdpThread* m_pUdpThread; // ¿Ã∏ß¿ª readThread 
public:
	CUdpClient();
	virtual ~CUdpClient();

	bool Init(PCSTR _ip, u_short _port);

	SOCKET GetSocket() { return m_socket; }
};