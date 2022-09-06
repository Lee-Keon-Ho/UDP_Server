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

	bool bUser = true;

	CSession* session = new CSession();
	session->SetAddr(_addr);

	if (m_userList.size() == 0)
	{
		m_userList.push_back(session);
		printf("In User\n");
	}
	else
	{
		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->Compare(_addr))
			{
				bUser = false;
				break;
			}
		}

		if (bUser)
		{
			m_userList.push_back(session);
			printf("In User\n");
		}
	}
}