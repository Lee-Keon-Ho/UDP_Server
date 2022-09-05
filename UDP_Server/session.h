#pragma once
#include <WinSock2.h>

class CSession
{
protected:
	SOCKADDR_IN m_addr;

public:
	CSession();
	~CSession();

	bool Compare(SOCKADDR_IN _addr);

	void SetAddr(SOCKADDR_IN _addr);
	SOCKADDR_IN GetAddr() { return m_addr; }
};