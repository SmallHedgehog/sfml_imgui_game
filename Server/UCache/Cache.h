#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include "../Network/Socket.h"
#include "../Threads/virtualThread.h"


class Cache
{
public:
	static std::list<Socket*> cSockets;
	static std::list<virtualThread*> cThreads;

	// »º´æusername->cSocket
	static std::unordered_map<std::string, Socket*> cMaps;

	// »º´æÆ¥Åä³É¹¦µÄusername->username
	static std::unordered_map<std::string, std::string> cUsers;
};
