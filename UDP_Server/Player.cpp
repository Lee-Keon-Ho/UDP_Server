#include "Player.h"
#include "PacketHandler.h"
#include "Room.h"

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

bool CPlayer::UdpInit()
{
	m_pUdpListener = new CUserUdpListener();

	return false;
}

void CPlayer::SetPlayerInfo(char* _name)
{
	memcpy(m_name, _name, PLAYER_NAME_MAX);
}

void CPlayer::SetPlayerInfo(int _number, int _state, int _team)
{
	m_number = _number;
	m_state = _state;
	m_team = _team;
}

void CPlayer::SetRoom(CRoom* _room, int _boss)
{
	m_pRoom = _room;
	m_boss = _boss;
}

void CPlayer::SetBoss(int _boss)
{
	m_boss = _boss;
}

void CPlayer::SetNumber(int _number)
{
	m_number = _number;
}

void CPlayer::SetState(int _state)
{
	m_state = _state;
}

void CPlayer::SetTeam(int _team)
{
	m_team = _team;
}

void CPlayer::SetReady()
{
	if (m_ready == 0) m_ready = 1;
	else m_ready = 0;
}

void CPlayer::SetReady(int _ready)
{
	if (m_ready == 0) m_ready = 1;
	else m_ready = 0;
}

void CPlayer::SetPosition(float* _position)
{
	m_position[0] = _position[0];
	m_position[1] = _position[1];
	m_position[2] = _position[2];
}

CRoom* CPlayer::GetRoom()
{
	return m_pRoom;
}
