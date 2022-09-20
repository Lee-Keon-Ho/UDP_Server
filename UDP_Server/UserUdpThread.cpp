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
	memset(sendBuffer, 0, 255);
	stUdp* studp = (stUdp*)_pArgs;
	SOCKET socket = studp->socket;
	CRoom* room = studp->room;
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);


	int inCount = 0;
	int playerCount = room->GetPlayerList().size();
	bool bUdp = false;

	while (true)
	{
		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		if (recvSize == -1)
		{
			printf("recvfrom() Error \n");
			break;
		}

		for (int i = 0; i < recvSize; i++)
		{
			printf("%d ", recvData[i]);
		}
		printf("\n");

		char* tempBuffer = recvData;

		int size = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int number = *(USHORT*)tempBuffer;

		if (room->CompareAddr(clientAddr, number))
		{
			inCount = 0;

			CRoom::player_t playerList = room->GetPlayerList();

			CRoom::player_t::iterator iter = playerList.begin();
			CRoom::player_t::iterator iterEnd = playerList.end();

			for (; iter != iterEnd; iter++)
			{
				if ((*iter)->GetUdp())
				{
					inCount += 1;
				}
			}
		}

		if (inCount == playerCount)
		{
			CRoom::player_t playerList = room->GetPlayerList();

			tempBuffer = sendBuffer;
			*(USHORT*)tempBuffer = 4 + ((2 + sizeof(clientAddr.sin_addr) + sizeof(clientAddr.sin_port)) * inCount);
			tempBuffer += 2;
			*(USHORT*)tempBuffer = 1;
			tempBuffer += 2;

			CRoom::player_t::iterator iter = playerList.begin();
			CRoom::player_t::iterator iterEnd = playerList.end();

			for (; iter != iterEnd; iter++) // tcp
			{
				SOCKADDR_IN addr = (*iter)->GetAddr();
				USHORT port = ntohs(addr.sin_port);

				*(USHORT*)tempBuffer = (*iter)->GetNumber();
				tempBuffer += sizeof(USHORT);
				memcpy(tempBuffer, &addr.sin_addr, sizeof(addr.sin_addr));
				tempBuffer += sizeof(addr.sin_addr);
				memcpy(tempBuffer, &port, sizeof(port));
				tempBuffer += sizeof(addr.sin_port);
			}

			iter = playerList.begin();

			for (; iter != iterEnd; iter++)
			{
				SOCKADDR_IN addr = (*iter)->GetAddr();
				sendto(socket, sendBuffer, tempBuffer - sendBuffer, 0, (sockaddr*)&addr, sizeof(sockaddr_in	));
			}
		}
	}

	// delete stUdp
	return 0;
}