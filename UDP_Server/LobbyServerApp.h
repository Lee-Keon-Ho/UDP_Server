#pragma once
#include "App.h"
#include "TcpListener.h"
#include "UdpClient.h"

class CLobby;

class CLobbyServerApp : public CApp
{
public:
	CTcpListener* m_pListener;
	CUdpClient* m_pUdp;
	CLobby* m_pLobby;

public:
	CLobbyServerApp();
	~CLobbyServerApp();

public:
	bool Initialize() override;
	bool CreateInstance() override;
	bool StartInstance() override;
	void RunLoop() override;
	void DeleteInstance() override;
};