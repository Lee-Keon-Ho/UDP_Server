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
	CSession(SOCKET _socket);
	virtual ~CSession();

	bool WsaRecv();
	bool Send(char* _buffer, int _size);
	virtual void RecvEvent(int _size);

	// 2022-10-18
	void RecvEvent1(int _size);
	virtual int OnRecv();

	void SetAddr(SOCKADDR_IN _addr);

	SOCKET GetSocket() { return m_socket; }
	CRingBuffer* GetRingBuffer() { return m_ringBuffer; }
	SOCKADDR_IN GetAddr() { return m_addr; }
};