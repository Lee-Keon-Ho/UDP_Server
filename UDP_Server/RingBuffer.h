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

	// buffer�� full ���� �˷��ִ� �Լ�
	bool IsFull() { return (m_remainDataSize >= m_size); }

	char* GetBuffer() { return m_buffer; }

	// ���� buffer�� recv �Ǿ� �ִ� recv data size
	int GetRemainDataSize() { return m_remainDataSize; }

	char* GetEndBufferAddr() { return m_pBufferEnd; }

	// Write ������ buffer size �˷��ִ� �Լ�
	int GetWriteBufferSize();

	// Write buffer �ּ� �˷��ִ� �Լ�
	char* GetWriteBuffer() { return m_pWrite; }

	// buffer�� Write�� ��� size ��ŭ m_pWrite�� �̵���Ű�� �Լ�
	void Write(int _size);

	// Read ������ buffer size �˷��ִ� �Լ�
	int GetReadSize();

	// Read buffer �ּ� �˷��ִ� �Լ�
	char* GetReadBuffer() { return m_pRead; }

	// buffer�� ���� ��� ���� size ��ŭ m_pRead�� �̵���Ű�� �Լ�
	void Read(int _size);

	// m_pBufferEnd - m_pRead�� ��
	int GetRemainSize_EndBuffer(const char* _pBuffer) { return (int)(m_pBufferEnd - _pBuffer); }
};