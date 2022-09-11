#pragma once
#include "Player.h"
#include <vector>

#define ROOM_NAME_MAX 64

class CRoom
{
public:
	typedef std::vector<CPlayer*> player_t;

	struct stRoom
	{
		int number;
		char name[ROOM_NAME_MAX];
		int playerCount;
		int state;
	};
private:
	stRoom m_room;

	player_t m_player;

	int m_teamA_Count;
	int m_teamB_Count;
	int m_start;

	CRITICAL_SECTION m_cs_ip;
	CRITICAL_SECTION m_cs_op;

public:
	CRoom();
	CRoom(int _num, char* _name, int _nameSize, int _playerCount, int _state);
	~CRoom();

	void InPlayer(CPlayer* _player);

	stRoom* GetRoomInfo() { return &m_room; }
};