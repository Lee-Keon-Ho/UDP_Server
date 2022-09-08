#include "PacketHandler.h"
#include "UdpListener.h"
#include "Lobby.h"
#include "PacketType.h"

CPacketHandler* CPacketHandler::pInstance = nullptr;

void CPacketHandler::SetLobby(CLobby* _pLobby)
{
	m_pLobby = _pLobby;
}

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
	tempBuf += sizeof(USHORT);
	USHORT type = *(USHORT*)tempBuf;
	tempBuf += sizeof(USHORT);

	switch (type)
	{
	case CS_PT_LOGIN:
		// 2022-09-08 End
		break;
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
