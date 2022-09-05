//#include "TcpListener.h"
//#include <WS2tcpip.h>
//#include <stdio.h>
//
//#pragma comment (lib, "ws2_32.lib")
//
//CTcpListener::CTcpListener()
//{
//
//}
//
//CTcpListener::~CTcpListener()
//{
//
//}
//
//bool CTcpListener::Init(PCSTR _ip, u_short _port)
//{
//	m_listenSocket = WSASocketW(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//
//	m_addr.sin_family = AF_INET;
//	inet_pton(AF_INET, _ip, &m_addr.sin_addr);
//	m_addr.sin_port = htons(_port);
//
//	if (bind(m_listenSocket, (sockaddr*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR)
//	{
//		printf("bind() Error\n");
//		return false;
//	}
//
//	if (listen(m_listenSocket, 5) == SOCKET_ERROR)
//	{
//		printf("listen() Error\n");
//		return false;
//	}
//
//	printf("server start\n");
//
//	return true;
//}
//
//bool CTcpListener::Loop()
//{
//	DWORD recvBytes = 0;
//	DWORD flags = 0;
//	DWORD err;
//
//	if (WSARecv(m_socket, &m_dataBuf, 1, &recvBytes, &flags, &m_overlapped, NULL) == SOCKET_ERROR)
//	{
//		if (err = WSAGetLastError() != WSA_IO_PENDING)
//		{
//			printf("Error WSARecv : %d \n", err);
//		}
//	}
//	return false;
//}
//
//bool CTcpListener::OnAccept()
//{
//	int size = sizeof(m_addrClient);
//	DWORD recvBytes = 0;
//	DWORD flags = 0;
//
//	m_clientSocket = accept(m_listenSocket, (sockaddr*)&m_addrClient, &size);
//	if (m_clientSocket == INVALID_SOCKET)
//	{
//		return false;
//	}
//
//	printf("client socket : %ld\n", m_clientSocket);
//
//	return true;
//}