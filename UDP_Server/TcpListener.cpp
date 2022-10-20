#include "TcpListener.h"
#include "workerThread.h"
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "ws2_32.lib")

CTcpListener::CTcpListener()
{

}

CTcpListener::~CTcpListener()
{

}

bool CTcpListener::Init(PCSTR _ip, u_short _port)
{
	m_listenSocket = WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	m_addr.sin_family = AF_INET;
	inet_pton(AF_INET, _ip, &m_addr.sin_addr);
	m_addr.sin_port = htons(_port);

	if (bind(m_listenSocket, (sockaddr*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
	{
		printf("bind() Error\n");
		return false;
	}

	if (listen(m_listenSocket, 5) == SOCKET_ERROR)
	{
		printf("listen() Error\n");
		return false;
	}

	CWorkerThread workerThread;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	for (unsigned int i = 0; i < si.dwNumberOfProcessors * 2; ++i)
	{
		workerThread.Start();
	}

	printf("server start\n");

	return true;
}

SOCKET CTcpListener::OnAccept()
{
	int size = sizeof(sockaddr_in);
	DWORD recvBytes = 0;
	DWORD flags = 0;

	sockaddr_in clientAddr;
	SOCKET clientSocket;

	clientSocket = accept(m_listenSocket, (sockaddr*)&clientAddr, &size);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("accept Error() \n");
		return false;
	}

	printf("client socket : %ld\n", clientSocket);

	return clientSocket;
}
