#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "../Network/Socket.h"
#include "../Threads/virtualThread.h"


class Cache
{
public:
	static std::list<Socket*> cSockets;
	static std::list<virtualThread*> cThreads;

	// Cache the user of online
	static std::unordered_set<std::string> onlineUsers;

	// Cache username->cSocket
	static std::unordered_map<std::string, Socket*> cMaps;

	// Cache the user of matching successfully(username->username)
	static std::unordered_map<std::string, std::string> cUsers;

	// Cache the user of being matching
	static std::unordered_set<std::string> isForMatchU;

	// Cache the fight users
	static std::unordered_map<std::string, std::string> fightUsers;
};
