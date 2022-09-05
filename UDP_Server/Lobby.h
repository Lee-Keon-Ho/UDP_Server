#pragma once
#include "session.h"
#include <list>

class CLobby
{
public:
	typedef std::list<CSession*> playerList_t;

private:
	playerList_t m_userList;

public:
	CLobby();
	~CLobby();

	void push_back(sockaddr_in _addr);
};