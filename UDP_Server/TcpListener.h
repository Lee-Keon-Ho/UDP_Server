#pragma once
#include <WinSock2.h>

// 2022-10-18
struct ACCEPT_SOCKET_INFO
{
	SOCKET		socket;
	SOCKADDR	addr;
};

class CTcpListener
{
protected:
	SOCKET m_listenSocket;
	SOCKET m_clientSocket;
	sockaddr_in m_addr;
	sockaddr_in m_addrClient;

public:
	CTcpListener();
	virtual ~CTcpListener();

	bool Init(PCSTR _ip, u_short _port);

	bool onAccept();

	//2022-10-18 ����
	SOCKET OnAccept1();
};