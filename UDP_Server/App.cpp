#include "App.h"

CApp::CApp()
{

}

CApp::~CApp()
{

}

bool CApp::Init()
{
	if (!Initialize()) return false;
	if (!CreateInstance()) return false;
	return StartInstance();
}

void CApp::Run()
{
	RunLoop();
}

void CApp::Destroy()
{
	DeleteInstance();
}