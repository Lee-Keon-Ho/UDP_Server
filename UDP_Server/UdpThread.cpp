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

	//handle�� ���и� �� ��� Ȯ�� ���ϰ� �ִ�.
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

	CLobby* pLobby = packetHandler->GetLobby(); // �̺κ��� ��ü������ Ȯ�� 


	while (true)
	{
		recvSize = recvfrom(socket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);
		
		if (recvSize == -1) // �켱 ����� test ������ ���Ⱑ �ƴϴ�
		{
			printf("recvfrom() Error \n");
		}

		char* tempBuffer = recvData;

		int size = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int type = *(USHORT*)tempBuffer;
		tempBuffer += sizeof(USHORT);
		int clientSocket = *(USHORT*)tempBuffer; // �̺κ��� Ŭ���̾�Ʈ���� �ö� ���� ����� ��
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
			CPlayer* pPlayer = pLobby->SearchSocket(clientSocket); // index�� �����ؼ� ���� ���

			if (pPlayer == nullptr) continue;

			pPlayer->SetAddr(clientAddr); // udp �ּҿ� ��Ʈ
			pPlayer->SetSourceAddr(sourceAddr);

			// ntohs��뿡 ����
			printf("socket :  %d port : %d : %d\n", pPlayer->GetSocket(), ntohs(clientAddr.sin_port), sourceAddr);
			pPlayer->SetUdp(true);

			packetHandler->Handle_SockAddr(pPlayer); // ����

			packetHandler->Handle_AddressAll(pPlayer);
		}
	}
}