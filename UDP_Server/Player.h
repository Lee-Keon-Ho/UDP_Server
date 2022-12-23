#pragma once
#include "session.h"

#define PLAYER_NAME_MAX 32

class CRoom;

class CPlayer : public CSession
{
private:
	char m_name[PLAYER_NAME_MAX];

	int m_state;
	int m_number; // room에서 말고 전체의 id socket 번호를 써도 괜찮다
	int m_nameLen;
	int m_ready;
	int m_boss;
	float m_position[3];
	bool m_bUdp;
	UINT m_sourceAddr;

	CRoom* m_pRoom;
public:
	CPlayer();
	CPlayer(SOCKET _socket);
	CPlayer(SOCKET _socket, char* _name);
	~CPlayer();

	int OnRecv() override;

	void SetPlayerInfo(char* _name, int _nameLen);
	void SetPlayerInfo(int _number, int _state);
	void SetRoom(CRoom* _room, int _boss);
	void SetBoss(int _boss);
	void SetNumber(int _number);
	void SetState(int _state);
	void SetReady();
	void SetReady(int _ready);
	void SetPosition(float* _position);
	void SetUdp(bool _bUdp);
	void SetSourceAddr(UINT _sourceAddr);

	char* GetName() { return m_name; }
	int GetState() { return m_state; }
	int GetBoss() { return m_boss; }
	int GetReady() { return m_ready; }
	int GetNumber() { return m_number; }
	bool GetUdp() { return m_bUdp; }
	int GetNameLen() { return m_nameLen; }
	long GetSourceAddr() { return m_sourceAddr; }
	CRoom* GetRoom();
};