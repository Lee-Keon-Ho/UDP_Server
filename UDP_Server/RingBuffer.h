#pragma once

class CRingBuffer
{
private:
	int m_size;

	char* m_buffer;
	char* m_pRead;
	char* m_pWrite;
	char* m_pBufferEnd;

	int m_remainDataSize;
public:
	CRingBuffer(int _bufferSize = 65530);
	virtual ~CRingBuffer();

	// buffer가 full 인지 알려주는 함수
	bool IsFull() { return (m_remainDataSize >= m_size); }

	char* GetBuffer() { return m_buffer; }

	// 현재 buffer에 recv 되어 있는 recv data size
	int GetRemainDataSize() { return m_remainDataSize; }

	char* GetEndBufferAddr() { return m_pBufferEnd; }

	// Write 가능한 buffer size 알려주는 함수
	int GetWriteBufferSize();

	// Write buffer 주소 알려주는 함수
	char* GetWriteBuffer() { return m_pWrite; }

	// buffer를 Write한 경우 size 만큼 m_pWrite를 이동시키는 함수
	void Write(int _size);

	// Read 가능한 buffer size 알려주는 함수
	int GetReadSize();

	// Read buffer 주소 알려주는 함수
	char* GetReadBuffer() { return m_pRead; }

	// buffer를 읽은 경우 읽은 size 만큼 m_pRead를 이동시키는 함수
	void Read(int _size);

	// m_pBufferEnd - m_pRead의 값
	int GetRemainSize_EndBuffer(const char* _pBuffer) { return (int)(m_pBufferEnd - _pBuffer); }
};