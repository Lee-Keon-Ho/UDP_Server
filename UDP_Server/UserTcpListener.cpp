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

void CUserTcpListener::OnAssociate()
{
	CPlayer* pPlayer = new CPlayer(m_clientSocket);

	CIocp::GetInstance()->Associate(m_clientSocket, (CSession*)pPlayer);

	if (!pPlayer->WsaRecv())
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("wsarecv error %d\n", WSAGetLastError());
		}
	}
}