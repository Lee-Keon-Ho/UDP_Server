#include "UserUdpListener.h"
#include "UserUdpThread.h"
#include <stdio.h>

CUserUdpListener::CUserUdpListener()
{

}

CUserUdpListener::~CUserUdpListener()
{

}

bool CUserUdpListener::Init(PCSTR _ip, u_short _port, CRoom* _room)
{
	if (!CUdpListener::Init(_ip, _port))
	{
		printf("UdpListener Init Error\n");
		return false;
	}

	CUserUdpThread udp;

	udp.Start(m_socket, _room);

	printf("udp server start...\n");
}

