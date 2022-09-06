#include "PacketHandler.h"
#include "UdpListener.h"
#include "Lobby.h"

CPacketHandler* CPacketHandler::pInstance = nullptr;

void CPacketHandler::SetLobby(CLobby* _pLobby)
{
	m_pLobby = _pLobby;
}

int CPacketHandler::Handle(sockaddr_in _addr, char* _buffer)
{
	char* tempBuffer = _buffer;

	USHORT test = *(USHORT*)tempBuffer;

	if (test == 1)
	{
		char buffer[6];
		char* tempBuffer = buffer;
		*(USHORT*)tempBuffer = 6;
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = 1;
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = 1;
		tempBuffer += sizeof(USHORT);

		sendto(CUdpListener::GetInstance()->GetSocket(), buffer, tempBuffer - buffer, 0, (sockaddr*)&_addr, sizeof(_addr));
	}

	return 0;
}

CPacketHandler* CPacketHandler::GetIstance()
{
	if (pInstance == nullptr) { pInstance = new CPacketHandler(); }
	return pInstance;
}

void CPacketHandler::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

CPacketHandler::CPacketHandler()
{
}

CPacketHandler::~CPacketHandler()
{
}
