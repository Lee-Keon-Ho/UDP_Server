#pragma once
#include <WinSock2.h>

class CSession;

class CIocp
{
private:
	static CIocp* pInstance;
	CIocp();
	~CIocp();

public:
	static CIocp* GetInstance();
	static void DeleteInstance();

private:
	HANDLE m_iocp;

public:
	HANDLE Associate(SOCKET _socket, CSession* _session);
	HANDLE GetHandle() { return m_iocp; }
};