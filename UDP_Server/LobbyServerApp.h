#pragma once
#include "UdpListener.h"
#include "App.h"

class CLobbyServerApp : public CApp
{
public:
	CUdpListener* m_pListener; // listener 보단 socket 같다

public:
	CLobbyServerApp();
	~CLobbyServerApp();

protected:
	bool Initialize() override;
	bool CreateInstance() override;
	bool StartInstance() override;
	void RunLoop() override;
	void DeleteInstance() override;
};