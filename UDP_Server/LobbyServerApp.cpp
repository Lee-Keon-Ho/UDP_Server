#include "LobbyServerApp.h"
#include <stdio.h>

CLobbyServerApp::CLobbyServerApp() : m_pListener(nullptr)
{
}

CLobbyServerApp::~CLobbyServerApp()
{
	
}

bool CLobbyServerApp::Initialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed WSAStartup()\n");
		return false;
	}

	return true;
}

bool CLobbyServerApp::CreateInstance()
{
	m_pListener = new CUdpListener();
	if (m_pListener == nullptr) return false;
	return true;
}

bool CLobbyServerApp::StartInstance()
{
	if (!m_pListener->Init("211.218.197.86", 30002)) return false;

	return true;
}

void CLobbyServerApp::RunLoop()
{
	m_pListener->Loop();
}

void CLobbyServerApp::DeleteInstance()
{
	m_pListener->CloseSocket();
	WSACleanup();
	if (m_pListener) { delete m_pListener; m_pListener = nullptr; }
}
