#pragma once
#include <WinSock2.h>

class CLobby;

class CPacketHandler
{
private:
	static CPacketHandler* pInstance;
	CPacketHandler();
	~CPacketHandler();

private:
	CLobby* m_pLobby;

public:
	void SetLobby(CLobby* _pLobby);

	int Handle(sockaddr_in _addr, char* _buffer);
public:
	static CPacketHandler* GetIstance();
	static void DeleteInstance();
};