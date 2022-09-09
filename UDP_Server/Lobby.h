#pragma once
#include "player.h"
#include <list>

class CLobby
{
public:
	typedef std::list<CPlayer*> playerList_t;

private:
	playerList_t m_playerList;

	CRITICAL_SECTION m_cs_ap;
	CRITICAL_SECTION m_cs_rp;
	CRITICAL_SECTION m_cs_ar;
	CRITICAL_SECTION m_cs_rr;
public:
	CLobby();
	~CLobby();

	void AddPlayer(CPlayer* _player);
	void RemovePlayer(CPlayer* _player);

	void SendAll(char* _buffer, int _size);

	playerList_t GetPlayerList() { return m_playerList; }
};