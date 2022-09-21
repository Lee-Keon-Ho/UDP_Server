#include "session.h"
#include <stdio.h>
CSession::CSession()
{

}

CSession::CSession(SOCKET _socket)
{
	m_ringBuffer = new CRingBuffer(1000);
	m_socket = _socket;
	m_dataBuf.buf = m_ringBuffer->GetWriteBuffer();
	m_dataBuf.len = m_ringBuffer->GetWriteBufferSize();
}

CSession::~CSession()
{
	closesocket(m_socket);
	if (m_ringBuffer) { delete m_ringBuffer; m_ringBuffer = nullptr; }
}

bool CSession::WsaRecv()
{
	DWORD recvBytes = 0;
	DWORD flags = 0;
	DWORD err;

	if (WSARecv(m_socket, &m_dataBuf, 1, &recvBytes, &flags, &m_overlapped, NULL) == SOCKET_ERROR)
	{
		if (err = WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error WSARecv : %d \n", err);
			return false;
		}
	}
	return true;;
}

bool CSession::Send(char* _buffer, int _size)
{
	int sendSize = send(m_socket, _buffer, _size, 0);

	if (sendSize < 0) return false;

	return true;
}

void CSession::RecvEvent(int _size)
{
}

void CSession::SetAddr(SOCKADDR_IN _addr)
{
	m_addr = _addr;
}
