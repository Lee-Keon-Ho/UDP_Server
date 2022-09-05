#include "PacketHandler.h"
#include "Lobby.h"

CPacketHandler* CPacketHandler::pInstance = nullptr;

void CPacketHandler::SetLobby(CLobby* _pLobby)
{
	m_pLobby = _pLobby;
}

int CPacketHandler::Handle(sockaddr_in _addr, char* _buffer)
{
	char* tempBuffer = _buffer;

	m_pLobby->push_back(_addr);

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
