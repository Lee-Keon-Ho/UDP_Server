#include "UdpListener.h"
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

CUdpListener::CUdpListener()
{

}

CUdpListener::~CUdpListener()
{

}

bool CUdpListener::Init(PCSTR _ip, u_short _port)
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return false;
	}

	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	inet_pton(AF_INET, _ip, &m_addr.sin_addr);
	m_addr.sin_port = htons(_port);

	if (bind(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();

		printf("binedingError %d\n", err);
		return false;
	}

	// thread

	printf("server start...\n");
	
	return true;
}

void CUdpListener::Loop()
{
	int recvSize;
	char recvData[255];
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	while (true)
	{
		recvSize = recvfrom(m_socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		if (recvSize == -1)
		{
			printf("recv() Error \n");
			break;
		}

		printf("Socket : %d recv %d message : %s \n", m_socket, recvSize, recvData);
		printf("%d %d %d %d : %d\n\n",clientAddr.sin_addr.S_un.S_un_b.s_b1,
			clientAddr.sin_addr.S_un.S_un_b.s_b2,
			clientAddr.sin_addr.S_un.S_un_b.s_b3,
			clientAddr.sin_addr.S_un.S_un_b.s_b4,
			clientAddr.sin_port);

		sendto(m_socket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	}

}

void CUdpListener::CloseSocket()
{
	closesocket(m_socket);
}
