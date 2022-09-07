#pragma once
#include "TcpListener.h"

class CUserTcpListener : public CTcpListener
{
private:

public:
	CUserTcpListener();
	~CUserTcpListener();

	bool OnAssociate();
	void Recv();
};