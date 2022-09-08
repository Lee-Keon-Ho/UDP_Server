#pragma once
#include "session.h"

#define PLAYER_NAME_MAX 64

class CPlayer : public CSession
{
private:
	char m_name[PLAYER_NAME_MAX];

public:
	CPlayer();
	CPlayer(SOCKET _socket, SOCKADDR_IN _addr);
	CPlayer(SOCKET _socket, SOCKADDR_IN _addr, char* _name);
	~CPlayer();

	void RecvEvent(int _size) override;
};