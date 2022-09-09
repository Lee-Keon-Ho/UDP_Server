#include "Player.h"
#include "PacketHandler.h"

CPlayer::CPlayer()
{

}

CPlayer::CPlayer(SOCKET _socket, SOCKADDR_IN _addr) : CSession(_socket, _addr),
	m_state(0), m_number(0), m_team(0), m_ready(0), m_boss(0)
{
	memset(m_name, 0, PLAYER_NAME_MAX);
}

CPlayer::CPlayer(SOCKET _socket, SOCKADDR_IN _addr, char* _name) : CSession(_socket, _addr),
	m_state(0), m_number(0), m_team(0), m_ready(0), m_boss(0)
{
	memcpy(m_name, _name, PLAYER_NAME_MAX);
}

CPlayer::~CPlayer()
{

}

void CPlayer::SetPlayerInfo(char* _name)
{
	memcpy(m_name, _name, PLAYER_NAME_MAX);
}

void CPlayer::RecvEvent(int _size)
{
	int readSize;

	m_ringBuffer->Write(_size);

	while (true)
	{
		readSize = CPacketHandler::GetIstance()->Handle(this);

		m_ringBuffer->Read(readSize);

		if (m_ringBuffer->GetReadSize() == 0) break;
	}

	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();
}