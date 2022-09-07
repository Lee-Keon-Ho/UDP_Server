#include "Lobby.h"

CLobby::CLobby()
{
}

CLobby::~CLobby()
{
}

void CLobby::push_back(sockaddr_in _addr)
{
	std::list<CSession*>::iterator iter = m_userList.begin();
	std::list<CSession*>::iterator iterEnd = m_userList.end();

}