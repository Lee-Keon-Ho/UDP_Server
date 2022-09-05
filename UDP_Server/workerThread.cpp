#include "workerThread.h"
#include "UdpListener.h"
#include <WS2tcpip.h>
#include <process.h>
#include <stdio.h>

CWorkerThread::CWorkerThread()
{
}

CWorkerThread::~CWorkerThread()
{
}

bool CWorkerThread::Start()
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CWorkerThread::ThreadFunc, this, 0, NULL);
	return true;
}

unsigned int _stdcall CWorkerThread::ThreadFunc(void* _pArgs)
{
	CWorkerThread* thread = (CWorkerThread*)_pArgs;

	thread->RunLoop();

	return 0;
}

void CWorkerThread::RunLoop()
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

		printf("Socket : %d recv %d message : %s \n", socket, recvSize, recvData);
		printf("%d %d %d %d : %d\n\n", clientAddr.sin_addr.S_un.S_un_b.s_b1,
			clientAddr.sin_addr.S_un.S_un_b.s_b2,
			clientAddr.sin_addr.S_un.S_un_b.s_b3,
			clientAddr.sin_addr.S_un.S_un_b.s_b4,
			clientAddr.sin_port);

		sendto(socket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	}
}
