#pragma once
#include "TcpListener.h"

class CPlayer;

class CUserTcpListener : public CTcpListener
{
private:

public:
	CUserTcpListener();
	~CUserTcpListener();

	bool OnAssociate();
	void Recv(CPlayer* _player);
};