#include "UserUdpThread.h"
#include "Room.h"
#include <process.h>

CUserUdpThread::CUserUdpThread()
{
}

CUserUdpThread::~CUserUdpThread()
{
}

bool CUserUdpThread::Start(SOCKET _socket, CRoom* _room)
{
	m_stUdp = new stUdp;
	m_stUdp->socket = _socket;
	m_stUdp->room = _room;

	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CUserUdpThread::UdpFunc, m_stUdp, 0, NULL);
	return true;
}

unsigned int _stdcall CUserUdpThread::UdpFunc(void* _pArgs)
{
	int recvSize;
	char recvData[255];
	char sendBuffer[255];
	stUdp* studp = (stUdp*)_pArgs;
	SOCKET socket = studp->socket;
	CRoom* room = studp->room;
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);


	int inCount = 0;
	int playerCount = room->GetPlayerList().size();

	if (inCount == playerCount)
	{
		// 게임 시작
	}

	while (true)
	{
		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		if (recvSize == -1)
		{
			printf("recvfrom() Error \n");
			break;
		}

		printf("recvfrom %d message : %s \n", recvSize, recvData);

		char* tempBuffer = recvData;

		int size = *(USHORT*)tempBuffer;
		int number = *(USHORT*)tempBuffer;

		if (room->CompareAddr(clientAddr, number)) inCount += 1; // 나중에 조건 문을 바꾸자

		if (inCount == playerCount)
		{
			CRoom::player_t playerList = room->GetPlayerList();

			tempBuffer = sendBuffer;
			*(USHORT*)tempBuffer = 4 + ((sizeof(clientAddr.sin_addr) + sizeof(clientAddr.sin_port)) * inCount);
			tempBuffer += 2;
			*(USHORT*)tempBuffer = 1;
			tempBuffer += 2;

			CRoom::player_t::iterator iter = playerList.begin();
			CRoom::player_t::iterator iterEnd = playerList.end();

			for (; iter != iterEnd; iter++)
			{
				SOCKADDR_IN addr = (*iter)->GetAddr();
				memcpy(tempBuffer, &addr.sin_addr, sizeof(addr.sin_addr));
				tempBuffer += sizeof(addr.sin_addr);
				memcpy(tempBuffer, &addr.sin_port, sizeof(addr.sin_port));
				tempBuffer += sizeof(addr.sin_port);
			}

			iter = playerList.begin();

			for (; iter != iterEnd; iter++)
			{
				SOCKADDR_IN addr = (*iter)->GetAddr();
				sendto(socket, sendBuffer, tempBuffer - sendBuffer, 0, (sockaddr*)&addr, sizeof(sockaddr));
			}
		}
	}

	// delete stUdp
	return 0;
}