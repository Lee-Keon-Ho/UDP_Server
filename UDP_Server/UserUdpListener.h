#pragma once
#include "UdpListener.h"

class CRoom;

class CUserUdpListener : public CUdpListener
{
private:

public:
	CUserUdpListener();
	~CUserUdpListener();

	//bool Init(PCSTR _ip, u_short _port, CRoom* _room);
};