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
	sockaddr_in m_addr;

public:
	CTcpListener();
	virtual ~CTcpListener();

	bool Init(PCSTR _ip, u_short _port);

	//2022-10-18 ¼ö¾÷
	SOCKET OnAccept();
};