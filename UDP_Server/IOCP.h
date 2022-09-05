#pragma once
#include <WinSock2.h>

class CIocp
{
private:
	static CIocp pInstance;
	CIocp();
	~CIocp();

public:
	static CIocp* GetInstance();
	static void DeleteInstance();

private:
	HANDLE m_iocp;

public:
};