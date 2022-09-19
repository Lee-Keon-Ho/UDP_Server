#pragma once
#include "UdpThread.h"

class CRoom;

class CUserUdpThread : public CUdpThread
{
public:
	struct stUdp
	{
		SOCKET socket;
		CRoom* room;
	};

	stUdp* m_stUdp;
public:
	CUserUdpThread();
	~CUserUdpThread();

	bool Start(SOCKET _socket, CRoom* _room);
	static unsigned int _stdcall UdpFunc(void* _pArgs);
};