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