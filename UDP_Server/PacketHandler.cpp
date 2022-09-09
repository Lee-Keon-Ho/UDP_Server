#include "PacketHandler.h"
#include "UdpListener.h"
#include "Lobby.h"
#include "PacketType.h"

CPacketHandler* CPacketHandler::pInstance = nullptr;

int CPacketHandler::Handle(CPlayer* _player)
{
	CRingBuffer* ringBuffer = _player->GetRingBuffer();

	char* readBuffer = ringBuffer->GetReadBuffer();
	int read_EndBuf = ringBuffer->GetRemainSize_EndBuffer(readBuffer);
	int readSize = ringBuffer->GetReadSize();

	if (readSize > read_EndBuf)
	{
		char tempBuffer[1000];
		memcpy(tempBuffer, ringBuffer->GetReadBuffer(), read_EndBuf);
		memcpy(tempBuffer + read_EndBuf, ringBuffer->GetBuffer(), readSize - read_EndBuf);
		readBuffer = tempBuffer;
	}

	char* tempBuf = readBuffer;
	USHORT size = *(USHORT*)tempBuf;
	tempBuf += sizeof(USHORT);
	USHORT type = *(USHORT*)tempBuf;
	tempBuf += sizeof(USHORT);

	switch (type)
	{
	case CS_PT_LOGIN:
		Handle_Login(_player, tempBuf, size);
		break;
	case CS_PT_LOGOUT:
		Handle_Logout(_player);
		break;
	case CS_PT_USERLIST:
		Handle_PlayerList();
		break;
	}

	return 0;
}

void CPacketHandler::Handle_Login(CPlayer* _player, char* _buffer, USHORT _size)
{
	char* tempBuffer = _buffer;

	int nameSize = _size - sizeof(int);

	_player->SetPlayerInfo(tempBuffer);

	m_pLobby->AddPlayer(_player);

	char sendBuffer[10];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = 6; // size
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_LOGIN;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = true;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_Logout(CPlayer* _player)
{
	m_pLobby->RemovePlayer(_player);

	char sendBuffer[10];
	char* tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = 6; // size
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_LOGOUT;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = true;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_PlayerList()
{
	char sendBuffer[2000];
	char* tempBuffer = sendBuffer;

	CLobby::playerList_t playerList = m_pLobby->GetPlayerList();

	int listSize = playerList.size();

	int len = sizeof(char) * PLAYER_NAME_MAX;

	*(USHORT*)tempBuffer = 6 + ((len + 2) * listSize);
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_USERLIST;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = listSize;
	tempBuffer += sizeof(USHORT);

	std::list<CPlayer*>::iterator iter = playerList.begin();
	std::list<CPlayer*>::iterator iterEnd = playerList.end();

	for (; iter != iterEnd; iter++)
	{
		*(USHORT*)tempBuffer = (*iter)->GetSocket();
		tempBuffer += sizeof(USHORT);
		memcpy(tempBuffer, (*iter)->GetName(), len);
		tempBuffer += len;
	}

	m_pLobby->SendAll(sendBuffer, tempBuffer - sendBuffer);
}

CPacketHandler* CPacketHandler::GetIstance()
{
	if (pInstance == nullptr) { pInstance = new CPacketHandler(); }
	return pInstance;
}

void CPacketHandler::SetLobby(CLobby* _pLobby)
{
	m_pLobby = _pLobby;
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
