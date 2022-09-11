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
	case CS_PT_ROOMLIST:
		Handle_RoomList();
		break;
	case CS_PT_CHAT:

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

void CPacketHandler::Handle_RoomList()
{
	char buffer[1000];
	char* tempBuffer = buffer;

	CLobby::roomList_t roomList = m_pLobby->GetRoomList();

	int roomSize = roomList.size();

	int len = sizeof(CRoom::stRoom);

	*(USHORT*)tempBuffer = 6 + (len * roomSize);
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_ROOMLIST;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = roomSize;
	tempBuffer += sizeof(USHORT);

	std::list<CRoom*>::iterator iter = roomList.begin();
	std::list<CRoom*>::iterator iterEnd = roomList.end();

	for (; iter != iterEnd; iter++)
	{
		memcpy(tempBuffer, (*iter)->GetRoomInfo(), len);
		tempBuffer += len;
	}

	m_pLobby->SendAll(buffer, tempBuffer - buffer);
}

void CPacketHandler::Handle_Chatting(CPlayer* _player, char* _buffer, int _chatSize)
{
	char chatStr[100];
	memset(chatStr, 0, 100);
	char* tempBuffer = chatStr;

	int nameSize = strlen(_player->GetName()) * sizeof(USHORT);

	memcpy(tempBuffer, _player->GetName(), nameSize);
	tempBuffer += nameSize;
	memcpy(tempBuffer, L" : ", wcslen(L" : ") * sizeof(USHORT));
	tempBuffer += (wcslen(L" : ") * sizeof(USHORT));
	memcpy(tempBuffer, _buffer, _chatSize - sizeof(int));
	tempBuffer += (_chatSize - sizeof(int));

	char sendBuffer[1000];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = sizeof(int) + 100; // 100 chat Size;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_CHAT;
	tempBuffer += sizeof(USHORT);
	memcpy(tempBuffer, chatStr, 100);
	tempBuffer += 100;

	m_pLobby->SendAll(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_CreateRoom(CPlayer* _player, char* _buffer, int _size)
{
	char* tempBuffer = _buffer;
	int number = m_pLobby->GetRoomSize() + 1;
	int state = 1;
	int ok = 0;

	CRoom* room = new CRoom(number, tempBuffer, _size - sizeof(int), 0, state);
	if (room != nullptr)
	{
		_player->SetPlayerInfo(0);
		_player->SetRoom(room);

		m_pLobby->AddRoom(room);
		ok = 1;
	}
	
	// Handle_PlayerInfo(_player);

	char sendBuffer[10];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = sizeof(USHORT) + sizeof(USHORT) + sizeof(USHORT);
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_CREATEROOM;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = ok;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);

	//Handle_RoomList();
	//Handle_RoomState(_player);
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
