#include "Lobby.h"

CLobby::CLobby()
{
	InitializeCriticalSection(&m_cs_ap);
	InitializeCriticalSection(&m_cs_rp);
	InitializeCriticalSection(&m_cs_ar);
	InitializeCriticalSection(&m_cs_rr);
}

CLobby::~CLobby()
{
	DeleteCriticalSection(&m_cs_rr);
	DeleteCriticalSection(&m_cs_ar);
	DeleteCriticalSection(&m_cs_rp);
	DeleteCriticalSection(&m_cs_ap);
}

void CLobby::AddPlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_ap);
	m_playerList.push_back(_player);
	LeaveCriticalSection(&m_cs_ap);
}

void CLobby::RemovePlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_rp);
	m_playerList.remove(_player);
	LeaveCriticalSection(&m_cs_rp);
}

void CLobby::AddRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs_ar);
	m_roomList.remove(_room);
	LeaveCriticalSection(&m_cs_ar);
}

CRoom* CLobby::RoomIn(CPlayer* _player, int _num)
{
	std::list<CRoom*>::iterator iter = m_roomList.begin();
	std::list<CRoom*>::iterator iterEnd = m_roomList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetNumber() == _num)
		{
			if ((*iter)->InPlayer(_player)) return *iter;
			else return nullptr;
		}
	}
	return nullptr;
}

void CLobby::RemoveRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs_rr);
	m_roomList.remove(_room);
	LeaveCriticalSection(&m_cs_rr);
}

void CLobby::SendAll(char* _buffer, int _size)
{
	std::list<CPlayer*>::iterator iter = m_playerList.begin();
	std::list<CPlayer*>::iterator iterEnd = m_playerList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Send(_buffer, _size);
	}
}
