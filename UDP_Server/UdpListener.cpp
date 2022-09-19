#include "UdpListener.h"
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

CUdpListener::CUdpListener()
{

}

CUdpListener::~CUdpListener()
{
	closesocket(m_socket);
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

		printf("bind Error %d\n", err);
		return false;
	}
	
	return true;
}