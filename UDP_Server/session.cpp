#include "session.h"

CSession::CSession()
{

}

CSession::~CSession()
{

}

bool CSession::Compare(SOCKADDR_IN _addr)
{
	if (m_addr.sin_port == _addr.sin_port)
	{
		return true;
	}
	return false;
}

void CSession::SetAddr(SOCKADDR_IN _addr)
{
	m_addr = _addr;
}
