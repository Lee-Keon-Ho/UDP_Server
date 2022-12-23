#pragma once

class CApp
{
public:
	CApp();
	virtual ~CApp();

	bool Init();
	void Run();
	void Destroy();

protected:
	virtual bool Initialize() = 0; // { return true; }
	virtual bool CreateInstance() { return true; }
	virtual bool StartInstance() { return true; }
	virtual void RunLoop() {}
	virtual void DeleteInstance() {}
};