#pragma once
#include "App.h"

class CLobby;

class CLobbyServerApp : public CApp
{
public:
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