#include "UdpThread.h"
#include <WS2tcpip.h>
#include <process.h>
#include <stdio.h>

CUdpThread::CUdpThread()
{
}

CUdpThread::~CUdpThread()
{
}

bool CUdpThread::Start(SOCKET _socket)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CUdpThread::UdpFunc, &_socket, 0, NULL);
	return true;
}

unsigned int _stdcall CUdpThread::UdpFunc(void* _pArgs)
{
	int recvSize;
	char recvData[255];
	char sendBuffer[255];
	SOCKET socket = (SOCKET)_pArgs;
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



		printf("recv %d message : %s \n", recvSize, recvData);

		
		//sendAll
		sendto(socket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	}
}

void CUdpThread::RunLoop()
{
}
