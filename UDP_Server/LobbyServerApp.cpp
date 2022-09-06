#include "UdpListener.h"
#include "LobbyServerApp.h"
#include "PacketHandler.h"
#include "Lobby.h"
#include <stdio.h>

CLobbyServerApp::CLobbyServerApp()
{
}

CLobbyServerApp::~CLobbyServerApp()
{
	
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
	CUdpListener::GetInstance();
	CPacketHandler::GetIstance();

	m_pLobby = new CLobby();
	if (!m_pLobby) return false;

	return true;
}

bool CLobbyServerApp::StartInstance()
{
	if (!CUdpListener::GetInstance()->Init("211.218.197.86", 30002)) return false;
	CPacketHandler::GetIstance()->SetLobby(m_pLobby);
	return true;
}

void CLobbyServerApp::RunLoop()
{
	SOCKET socket = CUdpListener::GetInstance()->GetSocket();
	int recvSize;
	char recvData[255];
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	while (true)
	{
		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		if (recvSize == -1)
		{
			printf("recv() Error \n");
			break;
		}

		// 기존에 있던 주소와 어떻게 비교할 것인가
		CPacketHandler::GetIstance()->Handle(clientAddr, recvData);

		printf("Socket : %d recv %d message : %s \n", socket, recvSize, recvData);
		printf("%d %d %d %d : %d\n\n", clientAddr.sin_addr.S_un.S_un_b.s_b1,
			clientAddr.sin_addr.S_un.S_un_b.s_b2,
			clientAddr.sin_addr.S_un.S_un_b.s_b3,
			clientAddr.sin_addr.S_un.S_un_b.s_b4,
			clientAddr.sin_port);

		sendto(socket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	}
}

void CLobbyServerApp::DeleteInstance()
{
	CUdpListener::DeleteInstance();
	WSACleanup();
}
