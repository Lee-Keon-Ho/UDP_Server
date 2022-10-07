#include "LobbyServerApp.h"
#include "IOCP.h"
#include "PacketHandler.h"
#include "Lobby.h"
#include <stdio.h>

CLobbyServerApp::CLobbyServerApp() : m_pListener(nullptr), m_pLobby(nullptr)
{
}

CLobbyServerApp::~CLobbyServerApp()
{
	if (m_pLobby) { delete m_pLobby; m_pLobby = nullptr; }
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
	CPacketHandler::DeleteInstance();
	CIocp::DeleteInstance();
}

bool CLobbyServerApp::Initialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CLobbyServerApp::CreateInstance()
{
	CIocp::GetInstance();
	CPacketHandler::GetIstance();

	m_pListener = new CUserTcpListener();
	if (!m_pListener) return false;
	m_pUdp = new CUdpListener();
	if (!m_pUdp) return false;
	m_pLobby = new CLobby();
	if (!m_pLobby) return false;

	return true;
}

bool CLobbyServerApp::StartInstance()
{
	CPacketHandler::GetIstance()->SetLobby(m_pLobby);
	if (!m_pListener->Init("112.184.241.149", 30002)) return false;
	if (!m_pUdp->Init("112.184.241.149", 30001)) return false;

	return true;
}

void CLobbyServerApp::RunLoop()
{
	while (true)
	{
		m_pListener->onAccept();
		m_pListener->OnAssociate();

		/*CPlayer* pPlayer = new CPlayer(m_clientSocket);
		CIocp::GetInstance()->Associate(m_clientSocket, (CSession*)pPlayer);
		if (!pPlayer->WsaRecv())*/

	}
}

void CLobbyServerApp::DeleteInstance()
{
	if (m_pLobby) { delete m_pLobby; m_pLobby = nullptr; }
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
	WSACleanup();
}
