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
	void Handle_RoomList();
	void Handle_Chatting(CPlayer* _player, char* _buffer, int _chatSize);
	void Handle_CreateRoom(CPlayer* _player, char* _buffer, int _size);
	void Handle_RoomIn(CPlayer* _player, char* _buffer);
	void Handle_RoomOut(CPlayer* _player);
	void Handle_RoomState(CPlayer* _player);
	void Handle_TeamChange(CPlayer* _player, char* _buffer);
	void Handle_Ready(CPlayer* _player);
	void Handle_Start(CPlayer* _player);
	void Handle_PlayerInfo(CPlayer* _player);
	void Handle_SockAddr(CPlayer* _player);

	void Test(CPlayer* _player);

	void SetLobby(CLobby* _pLobby);
	CLobby* GetLobby();
public:
	static CPacketHandler* GetIstance();
	static void DeleteInstance();
};