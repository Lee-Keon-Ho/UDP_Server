#pragma once
#include "session.h"

#define PLAYER_NAME_MAX 64

class CRoom;

class CPlayer : public CSession
{
private:
	char m_name[PLAYER_NAME_MAX];

	int m_state;
	int m_number;
	int m_team;
	int m_ready;
	int m_boss;

	CRoom* m_pRoom;
public:
	CPlayer();
	CPlayer(SOCKET _socket, SOCKADDR_IN _addr);
	CPlayer(SOCKET _socket, SOCKADDR_IN _addr, char* _name);
	~CPlayer();

	void RecvEvent(int _size) override;

	void SetPlayerInfo(char* _name);
	void SetPlayerInfo(int _number, int _state, int _team);
	void SetPlayerInfo(int _boss);

	void SetRoom(CRoom* _room);

	char* GetName() { return m_name; }
	int GetState() { return m_state; }
};