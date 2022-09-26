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
	CPacketHandler* packetHandler = CPacketHandler::GetIstance();
	int clientAddrSize = sizeof(clientAddr);


	int inCount = 0;
	//int playerCount = room->GetPlayerList().size();
	bool bUdp = false;

	SOCKET socket = m_socket;

	while (true)
	{
		CLobby* pLobby = packetHandler->GetLobby();

		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);

		char* tempBuffer = sendBuffer;

		if (recvSize == -1) // 우선 여기다 test 하지만 여기가 아니다
		{
			printf("recvfrom() Error \n");

			*(USHORT*)tempBuffer = 0;
			tempBuffer += sizeof(USHORT);
		}
		else
		{

			for (int i = 0; i < recvSize; i++)
			{
				printf("%d ", recvData[i]);
			}
			printf("\n");

			*(USHORT*)tempBuffer = 1;
			tempBuffer += sizeof(USHORT);
		}

		sendSize = sendto(socket, sendBuffer, tempBuffer - sendBuffer, 0, (sockaddr*)&clientAddr, clientAddrSize);
		

		// peer에 저장된 socket을 보내고 socket을 찾아서 addr값을 저장한다.
		// udp는 send를 할 이유가 없다.

		tempBuffer = recvData;

		int size = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int socket = *(USHORT*)tempBuffer;

		CPlayer* pPlayer = pLobby->SearchSocket(socket);

		pPlayer->SetAddr(clientAddr);
		pPlayer->SetUdp(true);

		packetHandler->Handle_SockAddr(pPlayer);

		packetHandler->Test(pPlayer);
		/*if (room->CompareAddr(clientAddr, number))
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
		}*/

		//if (inCount == playerCount)
		//{
		//	CRoom::player_t playerList = room->GetPlayerList();

		//	tempBuffer = sendBuffer;
		//	*(USHORT*)tempBuffer = 4 + ((2 + sizeof(clientAddr.sin_addr) + sizeof(clientAddr.sin_port)) * inCount);
		//	tempBuffer += 2;
		//	*(USHORT*)tempBuffer = 1;
		//	tempBuffer += 2;

		//	CRoom::player_t::iterator iter = playerList.begin();
		//	CRoom::player_t::iterator iterEnd = playerList.end();

		//	for (; iter != iterEnd; iter++) // tcp
		//	{
		//		SOCKADDR_IN addr = (*iter)->GetAddr();
		//		USHORT port = ntohs(addr.sin_port);

		//		*(USHORT*)tempBuffer = (*iter)->GetNumber();
		//		tempBuffer += sizeof(USHORT);
		//		memcpy(tempBuffer, &addr.sin_addr, sizeof(addr.sin_addr));
		//		tempBuffer += sizeof(addr.sin_addr);
		//		memcpy(tempBuffer, &port, sizeof(port));
		//		tempBuffer += sizeof(addr.sin_port);
		//	}

		//	iter = playerList.begin();

		//	for (; iter != iterEnd; iter++)
		//	{
		//		SOCKADDR_IN addr = (*iter)->GetAddr();
		//		sendto(socket, sendBuffer, tempBuffer - sendBuffer, 0, (sockaddr*)&addr, sizeof(sockaddr_in	));
		//	}
		//}
	}

	// delete stUdp
}
