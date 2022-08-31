#pragma once
#include "UdpListener.h"
#include "App.h"

class CLobbyServerApp : public CApp
{
public:
	CUdpListener* m_pListener; // listener ���� socket ����

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