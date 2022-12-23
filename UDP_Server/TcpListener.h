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
	SOCKET m_listenSocket; // class가 주어가 되기 때문에 그냥 socket이라고 해도 된다.
	sockaddr_in m_addr;

public:
	CTcpListener();
	virtual ~CTcpListener();

	bool Init(PCSTR _ip, u_short _port);

	//2022-10-18 수업
	SOCKET OnAccept(); // return 값을 ACCEPT_SOKET_INFO
};