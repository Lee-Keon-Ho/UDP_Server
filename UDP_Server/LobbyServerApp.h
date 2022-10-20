#pragma once
#include "App.h"
#include "TcpListener.h"
#include "UdpListener.h"

class CLobby;

class CLobbyServerApp : public CApp
{
public:
	CTcpListener* m_pListener;
	CUdpListener* m_pUdp;
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