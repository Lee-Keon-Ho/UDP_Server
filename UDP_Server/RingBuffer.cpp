#include "RingBuffer.h"
#include <memory>

CRingBuffer::CRingBuffer(int _bufferSize) : m_size(_bufferSize), m_remainDataSize(0)
{
	m_buffer = new char[m_size];
	memset(m_buffer, 0, sizeof(char) * m_size);

	m_pRead = m_buffer;
	m_pWrite = m_buffer;
	m_pBufferEnd = m_buffer + m_size;
}

CRingBuffer::~CRingBuffer()
{
	if (m_buffer) { delete m_buffer; }
}

int CRingBuffer::GetWriteBufferSize()
{
	if (IsFull()) return 0;

	if (m_pWrite >= m_pRead) return GetRemainSize_EndBuffer(m_pWrite);

	return (int)(m_pRead - m_pWrite);
}

void CRingBuffer::Write(int _size)
{
	m_pWrite += _size;
	m_remainDataSize += _size;

	if (m_pWrite == m_pBufferEnd) m_pWrite = m_buffer;
}

int CRingBuffer::GetReadSize()
{
	if (m_remainDataSize == 0) return 0;

	int size;

	if (GetRemainSize_EndBuffer(m_pRead) == 1)
	{
		if (m_remainDataSize == 1) return 0;

		char tempBuf[100];
		memcpy(tempBuf, m_pRead, 1);
		memcpy(tempBuf + 1, m_buffer, 1);

		size = *(unsigned short*)tempBuf;
	}
	else size = *(unsigned short*)m_pRead;

	if (m_remainDataSize >= size) return size;

	return 0;
}

void CRingBuffer::Read(int _size)
{
	if (m_remainDataSize >= _size)
	{
		int endBuf_Read = GetRemainSize_EndBuffer(m_pRead);

		if (endBuf_Read < _size)
		{
			endBuf_Read = _size - endBuf_Read;
			m_pRead = m_buffer;
			m_pRead += endBuf_Read;
		}
		else
		{
			m_pRead += _size;
			if (m_pRead == m_pBufferEnd) m_pRead = m_buffer;
		}

		m_remainDataSize -= _size;
	}
}
