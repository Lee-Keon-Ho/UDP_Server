#include "UdpThread.h"
#include "Lobby.h"
#include "Packethandler.h"
#include <process.h>

CUdpThread::CUdpThread()
{
}

CUdpThread::~CUdpThread()
{
}

bool CUdpThread::Start(SOCKET _socket)
{
	m_socket = _socket;

	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CUdpThread::UdpFunc, this, 0, NULL);
	Sleep(100);
	return true;
}

unsigned int _stdcall CUdpThread::UdpFunc(void* _pArgs)
{
	CUdpThread* tread = (CUdpThread*)_pArgs;

	tread->RunLoop();

	return 0;
}

void CUdpThread::RunLoop()
{
	int recvSize;
	int sendSize;
	char recvData[255];
	char sendBuffer[255];
	memset(sendBuffer, 0, 255);
	sockaddr_in clientAddr;
	CPacketHandler* packetHandler = CPacketHandler::GetInstance();
	int clientAddrSize = sizeof(clientAddr);

	SOCKET socket = m_socket;

	while (true)
	{
		CLobby* pLobby = packetHandler->GetLobby();

		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		if (recvSize == -1) // 우선 여기다 test 하지만 여기가 아니다
		{
			printf("recvfrom() Error \n");
		}

		char* tempBuffer = recvData;

		int size = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int type = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int clientSocket = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);	

		CPlayer* pPlayer = pLobby->SearchSocket(clientSocket);

		if (pPlayer == nullptr) continue;

		pPlayer->SetAddr(clientAddr);
		printf("socket :  %d  port : %d\n", pPlayer->GetSocket(), ntohs(clientAddr.sin_port));
		pPlayer->SetUdp(true);

		tempBuffer = sendBuffer;
		*(USHORT*)tempBuffer = 4;
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = 1;
		tempBuffer += sizeof(USHORT);

		sendSize = sendto(socket, sendBuffer, tempBuffer - sendBuffer, 0, (sockaddr*)&clientAddr, clientAddrSize);

		packetHandler->Handle_SockAddr(pPlayer); // 수정

		packetHandler->Handle_AddressAll(pPlayer);
	}
}