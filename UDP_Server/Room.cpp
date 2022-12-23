#include "Room.h"

#define PLAYER_MAX 8

CRoom::CRoom()
{
}

CRoom::CRoom(int _num, char* _name, int _nameSize, int _playerCount, int _state)
{
	InitializeCriticalSection(&m_cs_player);
	memset(m_room.name, 0, ROOM_NAME_MAX);
	m_room.number = _num;
	memcpy(m_room.name, _name, _nameSize);
	m_room.playerCount = _playerCount;
	m_room.state = _state;
	m_player.reserve(PLAYER_MAX);
}

CRoom::~CRoom()
{
	DeleteCriticalSection(&m_cs_player);
}

bool CRoom::InPlayer(CPlayer* _player)
{
	int size = m_player.size();

	if (size < PLAYER_MAX)
	{
		EnterCriticalSection(&m_cs_player);

		_player->SetPlayerInfo(size + 1, 1);

		m_room.playerCount++;
		m_player.push_back(_player);
		LeaveCriticalSection(&m_cs_player);
		return true;
	}
	return false;
}

bool CRoom::OutPlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_player);
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter) == _player)
		{
			m_player.erase(iter);
			break;
		}
	}

	int size = m_player.size();

	if (size != 0)
	{
		std::vector<CPlayer*>::iterator iter = m_player.begin();
		std::vector<CPlayer*>::iterator iterEnd = m_player.end();

		if (_player->GetBoss() == 0)
		{
			(*iter)->SetBoss(0);
			(*iter)->SetReady(0);
		}
		for (int i = 1; iter != iterEnd; iter++)
		{
			(*iter)->SetNumber(i);
			(*iter)->SetReady(0);
		}
	}

	m_room.playerCount--;

	LeaveCriticalSection(&m_cs_player);
	if (size == 0) return false;

	return true;
}

void CRoom::OnStart()
{
	CPlayer* boss = nullptr;
	bool start = true;

	EnterCriticalSection(&m_cs_player);
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetBoss() == 0)
		{
			boss = (*iter);
		}
		else
		{
			if ((*iter)->GetReady() != 1)
			{
				start = false;
				break;
			}
		}
	}

	LeaveCriticalSection(&m_cs_player);

	if (start) boss->SetReady(1);
	else boss->SetReady(0);
}

void CRoom::SendAll(char* _packet, USHORT _size)
{
	EnterCriticalSection(&m_cs_player);
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Send(_packet, _size);
	}
	LeaveCriticalSection(&m_cs_player);
}

bool CRoom::AllAddress()
{
	int count = 0;
	EnterCriticalSection(&m_cs_player);
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (iter; iter != iterEnd; iter++)
	{
		if ((*iter)->GetUdp())
		{
			count++;
		}
	}

	LeaveCriticalSection(&m_cs_player);

	if (count == m_room.playerCount) return true;
	
	return false;
}
