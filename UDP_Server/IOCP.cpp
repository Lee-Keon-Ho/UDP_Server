#include "IOCP.h"
#include "session.h"

CIocp* CIocp::pInstance = nullptr;

CIocp* CIocp::GetInstance()
{
	if (pInstance == nullptr) pInstance = new CIocp();
	return pInstance;
}

void CIocp::DeleteInstance()
{
	if (pInstance) { delete pInstance; pInstance = nullptr; }
}

HANDLE CIocp::Associate(SOCKET _socket, CSession* _session)
{
	return CreateIoCompletionPort((HANDLE)_socket, m_iocp, (ULONG_PTR)_session, 0);
}

CIocp::CIocp()
{
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

CIocp::~CIocp()
{
	CloseHandle(m_iocp);
}