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
	int Handle(CPlayer* _player);

	void Handle_Login(CPlayer* _player, char* _buffer, USHORT _size);
	void Handle_Logout(CPlayer* _player);
	void Handle_PlayerList();

	void SetLobby(CLobby* _pLobby);
public:
	static CPacketHandler* GetIstance();
	static void DeleteInstance();
};