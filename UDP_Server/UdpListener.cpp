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
	SOCKADDR_IN remoteaddr;
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET, _ip, &m_addr.sin_addr);
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
	int clientAddrSize = sizeof(m_addr);

	while (true)
	{
		recvSize = recvfrom(m_socket, recvData, sizeof(recvData), 0, (sockaddr*)&m_addr, &clientAddrSize);

		if (recvSize == -1)
		{
			printf("recv() Error \n");
			break;
		}

		printf("Socket : %d recv %d message : %s \n", m_socket, recvSize, recvData);
		printf("%d %d %d %d : %d\n\n",m_addr.sin_addr.S_un.S_un_b.s_b1,
			m_addr.sin_addr.S_un.S_un_b.s_b2,
			m_addr.sin_addr.S_un.S_un_b.s_b3,
			m_addr.sin_addr.S_un.S_un_b.s_b4,
			m_addr.sin_port);

		sendto(m_socket, recvData, recvSize, 0, (sockaddr*)&m_addr, clientAddrSize);
	}

}

void CUdpListener::CloseSocket()
{
	closesocket(m_socket);
}
