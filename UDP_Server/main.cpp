#include "LobbyServerApp.h"

int main(void)
{
	CLobbyServerApp app;

	if (app.Init())
	{
		app.Run();
	}
	app.Destroy();

	return 1;
	//WSADATA wsaData;
	//SOCKET hSocket;
	//SOCKADDR_IN servAddr;

	////
	//if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	//{
	//	printf("Failed WSAStartup() \n");
	//	return -1;
	//}

	////2. socket ����
	//hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//if (hSocket == INVALID_SOCKET)
	//{
	//	printf("Failed socket() \n");
	//	return -1;
	//}

	////3. ������ ���� �ּ� ����
	//memset(&servAddr, 0, sizeof(servAddr));
	//servAddr.sin_family = AF_INET;
	//inet_pton(AF_INET, "192.168.123.12", &servAddr.sin_addr);
	//servAddr.sin_port = htons(30012);

	//if (bind(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	//{
	//	int err = WSAGetLastError();

	//	printf("bineding Error %d\n", err);
	//	return -1;
	//}

	////4. ������ ���� ��� ���
	//int recvSize;
	//char recvData[255];
	//SOCKADDR_IN clientAddr; // recv ���� Ŭ���̾�Ʈ�� �ּҸ� ����
	//int clientAddrSize = sizeof(clientAddr);
	//while (1)
	//{
	//	recvSize = recvfrom(hSocket, recvData, sizeof(recvData), 0, (sockaddr*)&clientAddr, &clientAddrSize);
	//	
	//	if (recvSize == -1) 
	//	{
	//		printf("recv() Error \n");
	//		break;
	//	}
	//	printf("recv %d messaga : %s \n", recvSize, recvData);

	//	sendto(hSocket, recvData, recvSize, 0, (sockaddr*)&clientAddr, clientAddrSize);
	//}

	////5. ���� ���� -> ���� ����
	//closesocket(hSocket);
	//WSACleanup();
	return 0;
}