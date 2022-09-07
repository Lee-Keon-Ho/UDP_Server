#pragma once
#include <WinSock2.h>

class CSession
{
protected:
	WSABUF m_dataBuf;
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

	WSAOVERLAPPED m_overlapped;

public:
	CSession();
	virtual ~CSession();


};