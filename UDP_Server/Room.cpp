#include "Room.h"

#define PLAYER_MAX 8

CRoom::CRoom()
{
}

CRoom::CRoom(int _num, char* _name, int _nameSize, int _playerCount, int _state)
	: m_teamA_Count(0), m_teamB_Count(0), m_start(0)
{
	InitializeCriticalSection(&m_cs_ip);
	InitializeCriticalSection(&m_cs_op);
	memset(m_room.name, 0, ROOM_NAME_MAX);
	m_room.number = _num;
	memcpy(m_room.name, _name, _nameSize);
	m_room.playerCount = _playerCount;
	m_room.state = _state;
	m_player.reserve(PLAYER_MAX);
}

CRoom::~CRoom()
{
	DeleteCriticalSection(&m_cs_op);
	DeleteCriticalSection(&m_cs_ip);
}

void CRoom::InPlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_ip);
	int size = m_player.size();

	if (size < PLAYER_MAX)
	{
		if (m_teamA_Count <= m_teamB_Count)
		{
			_player->SetPlayerInfo(size + 1, 1, 0);
			m_teamA_Count++;
		}
		else
		{
			_player->SetPlayerInfo(size + 1, 1, 1);
			m_teamB_Count++;
		}

		m_room.playerCount++;
		m_player.push_back(_player);
	}

	LeaveCriticalSection(&m_cs_ip);
}
