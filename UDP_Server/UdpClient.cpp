#include "UdpClient.h"
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

CUdpClient::CUdpClient()
{

}

CUdpClient::~CUdpClient()
{
	closesocket(m_socket);
	if (m_pUdpThread) { delete m_pUdpThread; m_pUdpThread = nullptr; }
}

bool CUdpClient::Init(PCSTR _ip, u_short _port) // 
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

	m_pUdpThread = new CUdpThread() ;

	m_pUdpThread->Start(m_socket); 

	printf("udp server start...\n");

	return true;
}