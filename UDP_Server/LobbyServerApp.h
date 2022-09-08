#pragma once
#include "App.h"
#include "UserTcpListener.h"

class CLobby;

class CLobbyServerApp : public CApp
{
public:
	CUserTcpListener* m_pListener;
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