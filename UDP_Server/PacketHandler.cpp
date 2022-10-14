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

	for (int i = 0; i < readSize; i++)
	{
		printf("%d ", readBuffer[i]);
	}
	printf("\n");

	if (readSize == 0) return readSize;

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
		Handle_Chatting(_player, tempBuf, size);
		break;
	case CS_PT_CREATEROOM:
		Handle_CreateRoom(_player, tempBuf, size);
		break;
	case CS_PT_ROOMIN:
		Handle_RoomIn(_player, tempBuf);
		break;
	case CS_PT_ROOMOUT:
		Handle_RoomOut(_player);
		break;
	case CS_PT_ROOMSTATE:
		Handle_RoomState(_player);
		break;
	case CS_PT_TEAMCHANGE:
		Handle_TeamChange(_player, tempBuf);
		break;
	case CS_PT_READY:
		Handle_Ready(_player);
		break;
	case CS_PT_START:
		Handle_Start(_player);
		break;
	case CS_PT_PLAYERINFO:
		Handle_PlayerInfo(_player);
		break;
	default:
		break;
	}

	return readSize;
}

void CPacketHandler::Handle_Login(CPlayer* _player, char* _buffer, USHORT _size)
{
	char* tempBuffer = _buffer;

	_player->SetPlayerInfo(tempBuffer);

	m_pLobby->AddPlayer(_player);

	char sendBuffer[10];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = 8; // size
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_LOGIN;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = true;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = (USHORT)_player->GetSocket();
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

	// 최우선 과제 완료 후 처리 보이는 만큼만
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
		*(USHORT*)tempBuffer = (*iter)->GetState();
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
		_player->SetRoom(room, 0);

		m_pLobby->AddRoom(room);

		room->InPlayer(_player);

		ok = 1;
	}
	
	Handle_PlayerInfo(_player);

	char sendBuffer[10];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = sizeof(USHORT) + sizeof(USHORT) + sizeof(USHORT);
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_CREATEROOM;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = ok;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);

	Handle_RoomList();
	Handle_RoomState(_player);
}

void CPacketHandler::Handle_RoomIn(CPlayer* _player, char* _buffer)
{
	char* tempBuffer = _buffer;

	USHORT ok;
	int roomNum = *(USHORT*)tempBuffer;
	CRoom* room = m_pLobby->RoomIn(_player, roomNum);

	if (room != nullptr) ok = 1;
	else ok = 0;

	_player->SetRoom(room, 1);

	char sendBuffer[10];
	tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = 6;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_ROOMIN;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = ok;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
	
	Handle_PlayerInfo(_player);

	if (ok) Handle_RoomState(_player);
	
	Handle_RoomList();
}

void CPacketHandler::Handle_RoomOut(CPlayer* _player)
{
	CRoom* room = _player->GetRoom();
	if (!room->OutPlayer(_player))
	{
		m_pLobby->RemoveRoom(room);
	}

	Handle_RoomState(_player);

	_player->SetRoom(nullptr, 0);
	_player->SetState(0);

	char sendBuffer[10];
	char* tempBuffer = sendBuffer;
	*(USHORT*)tempBuffer = sizeof(USHORT) + sizeof(USHORT);
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_ROOMOUT;
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_RoomState(CPlayer* _player)
{
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	CRoom* room = _player->GetRoom();

	CRoom::player_t players = room->GetPlayerList();

	int size = players.size();
	int nameLen = PLAYER_NAME_MAX;
	int totalLen = 6 + nameLen; // team, ready, boss, name

	*(USHORT*)tempBuffer = 6 + (totalLen * size); // size + type + playerCount
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_ROOMSTATE;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = size;
	tempBuffer += sizeof(USHORT);

	std::vector<CPlayer*>::iterator iter = players.begin();
	std::vector<CPlayer*>::iterator iterEnd = players.end();

	for (; iter != iterEnd; iter++)
	{
		memcpy(tempBuffer, (*iter)->GetName(), nameLen);
		tempBuffer += nameLen;
		*(USHORT*)tempBuffer = (*iter)->GetTeam();
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = (*iter)->GetReady();
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = (*iter)->GetBoss();
		tempBuffer += sizeof(USHORT);

		Handle_PlayerInfo(*iter);
	}
	room->SendAll(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_TeamChange(CPlayer* _player,char* _buffer)
{
	int team = *(USHORT*)_buffer;

	_player->SetTeam(team);

	Handle_PlayerInfo(_player);
	Handle_RoomState(_player);
}

void CPacketHandler::Handle_Ready(CPlayer* _player)
{
	_player->SetReady();

	CRoom* room = _player->GetRoom();

	room->OnStart();

	Handle_PlayerInfo(_player);
	Handle_RoomState(_player);
}

void CPacketHandler::Handle_Start(CPlayer* _player)
{
	CRoom* room = _player->GetRoom();

	CRoom::player_t playerList = room->GetPlayerList();
	char sendBuffer[1000];
	char* tempBuffer = sendBuffer;

	int size = playerList.size() * sizeof(SOCKADDR_IN);

	*(USHORT*)tempBuffer = 4;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_START;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = room->GetPlayerSize();
	tempBuffer += sizeof(USHORT);

	room->SendAll(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_PlayerInfo(CPlayer* _player)
{
	char sendBuffer[20];
	char* tempBuffer = sendBuffer;

	*(USHORT*)tempBuffer = 10;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = CS_PT_PLAYERINFO;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = _player->GetBoss();
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = _player->GetReady();
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = _player->GetNumber();
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Handle_SockAddr(CPlayer* _player)
{
	char sendBuffer[100];
	char* tempBuffer = sendBuffer;

	SOCKADDR_IN addr = _player->GetAddr();

	*(USHORT*)tempBuffer = 2 + 2 + sizeof(UINT) + 2;
	tempBuffer += sizeof(USHORT);
	*(USHORT*)tempBuffer = 14;
	tempBuffer += sizeof(USHORT);
	*(UINT*)tempBuffer = addr.sin_addr.S_un.S_addr;
	tempBuffer += sizeof(UINT);
	*(USHORT*)tempBuffer = ntohs(addr.sin_port);
	tempBuffer += sizeof(USHORT);

	_player->Send(sendBuffer, tempBuffer - sendBuffer);
}

void CPacketHandler::Test(CPlayer* _player)
{
	CRoom* room = _player->GetRoom();

	if (room->AllAddress())
	{
		char sendBuffer[200];
		char* tempBuffer = sendBuffer;

		CRoom::player_t players = room->GetPlayerList();
		std::vector<CPlayer*>::iterator iter = players.begin();
		std::vector<CPlayer*>::iterator iterEnd = players.end();

		*(USHORT*)tempBuffer = 8 + ((sizeof(UINT) + sizeof(UINT) + sizeof(USHORT)) * players.size());
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = 15;
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = _player->GetBoss();
		tempBuffer += sizeof(USHORT);
		*(USHORT*)tempBuffer = players.size();
		tempBuffer += sizeof(USHORT);

		for (; iter != iterEnd; iter++)
		{
			SOCKADDR_IN addr = (*iter)->GetAddr();

			*(UINT*)tempBuffer = (*iter)->GetSocket();
			tempBuffer += sizeof(UINT);
			*(UINT*)tempBuffer = addr.sin_addr.S_un.S_addr;
			tempBuffer += sizeof(UINT);
			*(USHORT*)tempBuffer = ntohs(addr.sin_port);
			tempBuffer += sizeof(USHORT);
		}

		room->SendAll(sendBuffer, tempBuffer - sendBuffer);
	}
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

CLobby* CPacketHandler::GetLobby()
{
	return m_pLobby;
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
