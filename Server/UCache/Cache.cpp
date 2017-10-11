#include "Cache.h"

std::list<Socket*> Cache::cSockets;
std::list<virtualThread*> Cache::cThreads;

// »º´æcSocket->username
std::unordered_map<std::string, Socket*> Cache::cMaps;

// »º´æÆ¥Åä³É¹¦µÄusername->username
std::unordered_map<std::string, std::string> Cache::cUsers;
