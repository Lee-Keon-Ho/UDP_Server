#pragma once
#include "session.h"

#define PLAYER_NAME_MAX 64

class CRoom;

class CPlayer : public CSession
{
private:
	char m_name[PLAYER_NAME_MAX];

	int m_state;
	int m_number; // room에서 말고 전체의 id socket 번호를 써도 괜찮다
	int m_team;
	int m_ready;
	int m_boss;
	float m_position[3];
	bool m_bUdp;

	CRoom* m_pRoom;
public:
	CPlayer();
	CPlayer(SOCKET _socket);
	CPlayer(SOCKET _socket, char* _name);
	~CPlayer();

	void RecvEvent(int _size) override;

	void SetPlayerInfo(char* _name);
	void SetPlayerInfo(int _number, int _state, int _team);
	void SetRoom(CRoom* _room, int _boss);
	void SetBoss(int _boss);
	void SetNumber(int _number);
	void SetState(int _state);
	void SetTeam(int _team);
	void SetReady();
	void SetReady(int _ready);
	void SetPosition(float* _position);
	void SetUdp(bool _bUdp);

	char* GetName() { return m_name; }
	int GetState() { return m_state; }
	int GetBoss() { return m_boss; }
	int GetTeam() { return m_team; }
	int GetReady() { return m_ready; }
	int GetNumber() { return m_number; }
	bool GetUdp() { return m_bUdp; }
	CRoom* GetRoom();
};