#include "Lobby.h"

CLobby::CLobby()
{
	InitializeCriticalSection(&m_cs_player);
	InitializeCriticalSection(&m_cs_room);
}

CLobby::~CLobby()
{
	DeleteCriticalSection(&m_cs_player);
	DeleteCriticalSection(&m_cs_room);
}

void CLobby::AddPlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_player);
	m_playerList.push_back(_player);
	LeaveCriticalSection(&m_cs_player);
}

void CLobby::RemovePlayer(CPlayer* _player)
{
	EnterCriticalSection(&m_cs_player);
	m_playerList.remove(_player);
	LeaveCriticalSection(&m_cs_player);
}

void CLobby::AddRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs_room);
	m_roomList.push_back(_room);
	LeaveCriticalSection(&m_cs_room);
}

CRoom* CLobby::RoomIn(CPlayer* _player, int _num)
{
	EnterCriticalSection(&m_cs_room);
	std::list<CRoom*>::iterator iter = m_roomList.begin();
	std::list<CRoom*>::iterator iterEnd = m_roomList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetNumber() == _num)
		{
			if ((*iter)->InPlayer(_player))
			{
				LeaveCriticalSection(&m_cs_room);
				return *iter; // return 보다는 break좋다 실수를 방지할 수 있다.
			}
			else
			{
				LeaveCriticalSection(&m_cs_room);
				return nullptr;
			}
		}
	}
	LeaveCriticalSection(&m_cs_room);
	return nullptr;
}

void CLobby::RemoveRoom(CRoom* _room)
{
	EnterCriticalSection(&m_cs_room);
	m_roomList.remove(_room);
	LeaveCriticalSection(&m_cs_room);
}

CPlayer* CLobby::SearchSocket(SOCKET _socket)
{
	std::list<CPlayer*>::iterator iter = m_playerList.begin();
	std::list<CPlayer*>::iterator iterEnd = m_playerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetSocket() == _socket)
		{
			return *iter;
		}
	}

	return nullptr;
}

void CLobby::SendAll(char* _buffer, int _size)
{
	EnterCriticalSection(&m_cs_player);
	std::list<CPlayer*>::iterator iter = m_playerList.begin();
	std::list<CPlayer*>::iterator iterEnd = m_playerList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->Send(_buffer, _size);
	}
	LeaveCriticalSection(&m_cs_player);
}
