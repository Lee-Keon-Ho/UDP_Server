#include "Room.h"

#define PLAYER_MAX 8

CRoom::CRoom()
{
}

CRoom::CRoom(int _num, char* _name, int _nameSize, int _playerCount, int _state)
	: m_teamA_Count(0), m_teamB_Count(0), m_pUdpListener(nullptr)
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

bool CRoom::InPlayer(CPlayer* _player)
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
		LeaveCriticalSection(&m_cs_ip);
		return true;
	}
	LeaveCriticalSection(&m_cs_ip);
	return false;
}

bool CRoom::OutPlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_op);
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.begin();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter) == _player) m_player.erase(iter);
	}

	int size = m_player.size();

	if (m_player.size() != 0)
	{
		std::vector<CPlayer*>::iterator iter = m_player.begin();
		std::vector<CPlayer*>::iterator iterEnd = m_player.end();

		if (_player->GetBoss() == 0)
		{
			(*iter)->SetBoss(0);
		}
		for (int i = 1; iter != iterEnd; i++)
		{
			(*iter)->SetNumber(i);
		}
	}

	if (_player->GetTeam() == 0)
	{
		m_teamA_Count--;
	}
	else
	{
		m_teamB_Count--;
	}

	m_room.playerCount--;

	LeaveCriticalSection(&m_cs_op);
	if (size == 0) return false;

	return true;
}

void CRoom::OnStart()
{
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	CPlayer* boss = nullptr;
	bool start = true;

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

	if (start) boss->SetReady(1);
	else boss->SetReady(0);
}

bool CRoom::UdpInit(PCSTR _ip, u_short _port)
{
	m_pUdpListener = new CUserUdpListener();

	m_pUdpListener->Init(_ip, _port, this); // 각 방마다 포트번호가 달라야 한다...

	return true;
}

void CRoom::SendAll(char* _packet, USHORT _size)
{
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Send(_packet, _size);
	}
}

bool CRoom::CompareAddr(SOCKADDR_IN _addr, int _number)
{
	std::vector<CPlayer*>::iterator iter = m_player.begin();
	std::vector<CPlayer*>::iterator iterEnd = m_player.end();

	for (iter; iter != iterEnd; iter++)
	{
		if ((*iter)->GetNumber() == _number)
		{
			(*iter)->SetAddr(_addr);
			(*iter)->SetUdp(true);
			return true;
		}
	}
	return false;
}