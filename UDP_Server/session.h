#pragma once
#include <WinSock2.h>
#include "RingBuffer.h"

class CSession
{
protected:
	WSABUF m_dataBuf;
	SOCKET m_socket;
	SOCKADDR_IN m_addr;

	WSAOVERLAPPED m_overlapped;

	CRingBuffer* m_ringBuffer;
public:
	CSession();
	CSession(SOCKET _socket, SOCKADDR_IN _addr);
	virtual ~CSession();

	bool WsaRecv();
	bool Send(char* _buffer, int _size);
	virtual void RecvEvent(int _size);

	SOCKET GetSocket() { return m_socket; }
	CRingBuffer* GetRingBuffer() { return m_ringBuffer; }
};