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
	m_pLobby = new CLobby();
	if (!m_pLobby) return false;

	return true;
}

bool CLobbyServerApp::StartInstance()
{
	CPacketHandler::GetIstance()->SetLobby(m_pLobby);
	if (!m_pListener->Init("221.144.254.21", 30002)) return false;

	return true;
}

void CLobbyServerApp::RunLoop()
{
	while (true)
	{
		m_pListener->onAccept();
		m_pListener->OnAssociate();
	}
	//SOCKET socket = CUdpListener::GetInstance()->GetSocket();
	//int recvSize;
	//char recvData[255];
	//sockaddr_in clientAddr;
	//int clientAddrSize = sizeof(clientAddr);

	//while (true)
	//{
	//	recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

	//	if (recvSize == -1)
	//	{
	//		printf("recv() Error \n");
	//		break;
	//	}

	//	// 기존에 있던 주소와 어떻게 비교할 것인가
	//	CPacketHandler::GetIstance()->Handle(clientAddr, recvData);

	//	printf("Socket : %d recv %d message : %s \n", socket, recvSize, recvData);
	//	printf("%d %d %d %d : %d\n\n", clientAddr.sin_addr.S_un.S_un_b.s_b1,
	//		clientAddr.sin_addr.S_un.S_un_b.s_b2,
	//		clientAddr.sin_addr.S_un.S_un_b.s_b3,
	//		clientAddr.sin_addr.S_un.S_un_b.s_b4,
	//		clientAddr.sin_port);

	//	sendto(socket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	//}
}

void CLobbyServerApp::DeleteInstance()
{
	WSACleanup();
}
