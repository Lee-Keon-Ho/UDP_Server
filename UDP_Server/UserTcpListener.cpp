#include "UserTcpListener.h"
#include "IOCP.h"
#include "Player.h"
#include <stdio.h>

CUserTcpListener::CUserTcpListener()
{
}

CUserTcpListener::~CUserTcpListener()
{
}

bool CUserTcpListener::OnAssociate()
{
	CPlayer* pPlayer = new CPlayer(m_clientSocket);

	CIocp::GetInstance()->Associate(m_clientSocket, (CSession*)pPlayer);

	Recv(pPlayer);

	return false;
}

void CUserTcpListener::Recv(CPlayer* _player)
{
	if (_player->WsaRecv())
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("wsarecv error %d\n", WSAGetLastError());
		}
	}
}
