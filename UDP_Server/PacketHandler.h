#pragma once
#include <WinSock2.h>

class CLobby;
class CPlayer;

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

	int Handle(CPlayer* _player);
public:
	static CPacketHandler* GetIstance();
	static void DeleteInstance();
};