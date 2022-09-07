#include "IOCP.h"

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

CIocp::CIocp()
{
	m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

CIocp::~CIocp()
{
	CloseHandle(m_iocp);
}