#pragma once

class CApp
{
public:
	CApp();
	~CApp();

	bool Init();
	void Run();
	void Destroy();

protected:
	virtual bool Initialize() { return true; }
	virtual bool CreateInstance() { return true; }
	virtual bool StartInstance() { return true; }
	virtual void RunLoop() {}
	virtual void DeleteInstance() {}
};