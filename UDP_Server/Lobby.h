#pragma once
#include "player.h"
#include "Room.h"
#include <list>

class CLobby
{
public:
	typedef std::list<CPlayer*> playerList_t;
	typedef std::list<CRoom*> roomList_t;
private:
	playerList_t m_playerList;
	roomList_t m_roomList;

	CRITICAL_SECTION m_cs_ap;
	CRITICAL_SECTION m_cs_rp;
	CRITICAL_SECTION m_cs_ar;
	CRITICAL_SECTION m_cs_rr;
public:
	CLobby();
	~CLobby();

	void AddPlayer(CPlayer* _player);
	void RemovePlayer(CPlayer* _player);
	void AddRoom(CRoom* _room);
	CRoom* RoomIn(CPlayer* _player, int _num);
	void RemoveRoom(CRoom* _room);

	void SendAll(char* _buffer, int _size);

	playerList_t GetPlayerList() { return m_playerList; }
	roomList_t GetRoomList() { return m_roomList; }
	int GetRoomSize() { return m_roomList.size(); }
};