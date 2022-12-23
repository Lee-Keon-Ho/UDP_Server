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

	//handle이 실패를 할 경우 확인 안하고 있다.
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &CUdpThread::UdpFunc, this, 0, NULL);
	
	return true;
}

unsigned int _stdcall CUdpThread::UdpFunc(void* _pArgs)
{
	CUdpThread* thread = (CUdpThread*)_pArgs;

	thread->RunLoop();
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

	CLobby* pLobby = packetHandler->GetLobby(); // 이부분을 전체적으로 확인 


	while (true)
	{
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
		int clientSocket = *(USHORT*)tempBuffer; // 이부분이 클라이언트에서 올때 변한 경우의 수
		tempBuffer += sizeof(USHORT);
		UINT sourceAddr = *(UINT*)tempBuffer;

		printf("clientAddr ip : %d.%d.%d.%d : %d\n",
			clientAddr.sin_addr.S_un.S_un_b.s_b1,
			clientAddr.sin_addr.S_un.S_un_b.s_b2,
			clientAddr.sin_addr.S_un.S_un_b.s_b3,
			clientAddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs(clientAddr.sin_port));

		if (recvSize != size) continue;

		if (type == 1)
		{
			CPlayer* pPlayer = pLobby->SearchSocket(clientSocket); // index를 저장해서 쓰는 방법

			if (pPlayer == nullptr) continue;

			pPlayer->SetAddr(clientAddr); // udp 주소와 포트
			pPlayer->SetSourceAddr(sourceAddr);

			// ntohs사용에 주의
			printf("socket :  %d port : %d : %d\n", pPlayer->GetSocket(), ntohs(clientAddr.sin_port), sourceAddr);
			pPlayer->SetUdp(true);

			packetHandler->Handle_SockAddr(pPlayer); // 수정

			packetHandler->Handle_AddressAll(pPlayer);
		}
	}
}