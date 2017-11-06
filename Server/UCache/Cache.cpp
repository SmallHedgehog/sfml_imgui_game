#include "Cache.h"

std::list<Socket*> Cache::cSockets;
std::list<virtualThread*> Cache::cThreads;

// Cache the user of online
std::unordered_set<std::string> Cache::onlineUsers;

// Cache username->cSocket
std::unordered_map<std::string, Socket*> Cache::cMaps;

// Cache the user of matching successfully(username->username)
std::unordered_map<std::string, std::string> Cache::cUsers;

// Cache the user of being matching
std::unordered_set<std::string> Cache::isForMatchU;

// Cache the fight users
std::unordered_map<std::string, std::string> Cache::fightUsers;
