#pragma once
#include "Player.h"
#include <vector>

#define ROOM_NAME_MAX 64

class CRoom
{
public:
	typedef std::vector<CPlayer*> player_t;

	struct stRoom
	{
		int number;
		char name[ROOM_NAME_MAX];
		int playerCount;
		int state;
	};
private:
	stRoom m_room;

	player_t m_player;

	CRITICAL_SECTION m_cs_player;
public:
	CRoom();
	CRoom(int _num, char* _name, int _nameSize, int _playerCount, int _state);
	~CRoom();

	bool InPlayer(CPlayer* _player);
	bool OutPlayer(CPlayer* _player);
	void OnStart();

	void SendAll(char* _packet, USHORT _size);

	bool AllAddress();

	stRoom* GetRoomInfo() { return &m_room; }
	player_t GetPlayerList() { return m_player; }
	int GetPlayerSize() { return m_player.size(); }
	int GetNumber() { return m_room.number; }
};