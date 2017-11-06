#include "Fight.h"


void Fight::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_fight(_clientSocket, _msg, size);
}

void Fight::handleMessage_fight(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "handle fight message" << std::endl;

	// Parser message
	// std::vector<std::string> msgs;
	// parserMessage(msgs, _msg);

	SMessageTypeFlags_ type = SMessageTypeFlags_(size);
	switch (type)
	{
	case SMessageTypeFlags_FightChatMsg:
		{
			std::vector<std::string> msgs;
			ParserMsg2(msgs, _msg, "_");
			if (msgs.size() == 2)
			{
				std::vector<std::string> tUsers;
				ParserMsg2(tUsers, msgs[0].c_str(), "/");
				if (tUsers.size() == 2)
				{
					// Find user's Socket by the user's name
					Socket* respSocket = nullptr;
					std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
					for (; it != Cache::cMaps.end(); ++it)
					{
						if (it->first == tUsers[1])
						{
							respSocket = it->second;
							break;
						}
					}
					if (respSocket)
					{
						DATA_PACKAGE dPackage;
						FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightChatMsg, _msg, dPackage);

						respSocket->write((char*)&dPackage, sizeof(dPackage));
					}
				}
			}
		}
		break;
	case SMessageTypeFlags_FightChessPosMsg:
		{
			std::vector<std::string> msgs;
			parserMessage(msgs, _msg);
			if (msgs.size() == 4)
			{
				Socket* respSocket = nullptr;
				std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
				for (; it != Cache::cMaps.end(); ++it)
				{
					if (it->first == msgs[1])
					{
						respSocket = it->second;
						break;
					}
				}
				if (respSocket)
				{
					DATA_PACKAGE dPackage;
					FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightChessPosMsg, _msg, dPackage);

					respSocket->write((char*)&dPackage, sizeof(dPackage));
				}
			}
		}
		break;
	case SMessageTypeFlags_FightExitMsg:
		{
			std::vector<std::string> msgs;
			parserMessage(msgs, _msg);
			if (msgs.size() == 2)
			{
				Socket* respSocket = nullptr;
				std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
				for (; it != Cache::cMaps.end(); ++it)
				{
					if (it->first == msgs[1])
					{
						respSocket = it->second;
						break;
					}
				}
				if (respSocket)
				{
					DATA_PACKAGE dPackage;
					FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightExitMsg, _msg, dPackage);

					respSocket->write((char*)&dPackage, sizeof(dPackage));
				}

				// Find users whether in fightUsers
				std::unordered_map<std::string, std::string>::iterator item = Cache::fightUsers.find(msgs[0]);
				if (item != Cache::fightUsers.end())
				{
					// Update databae table of UserInfo
					Cache::fightUsers.erase(msgs[0]);
					Cache::fightUsers.erase(msgs[1]);
				}

				// Bordcast all online users
				DATA_PACKAGE notify_data;
				std::string sendData = msgs[0] + "_" + msgs[1];
				FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightUserToOnline, sendData.c_str(), notify_data);
				MultiMessageSend::messageSend(Cache::cSockets, notify_data);
			}
		}
		break;
	case SMessageTypeFlags_FightVictory:
		{
			std::vector<std::string> msgs;
			parserMessage(msgs, _msg);
			if (msgs.size() == 2)
			{
				// Update databae table of UserInfo
				Cache::fightUsers.erase(msgs[0]);
				Cache::fightUsers.erase(msgs[1]);
			}
		}
		break;
	case SMessageTypeFlags_FightFailure:
		{
			std::vector<std::string> msgs;
			parserMessage(msgs, _msg);
			if (msgs.size() == 2)
			{
				// Update databae table of UserInfo
				Cache::fightUsers.erase(msgs[0]);
				Cache::fightUsers.erase(msgs[1]);
			}
		}
		break;
	}
}

void Fight::ParserMsg2(std::vector<std::string>& _vals, const char* _msg, const char* delim)
{
	std::string info(_msg);

	size_t last = 0;
	size_t index = info.find_first_of(delim, last);
	if (index == std::string::npos)
	{
		_vals.push_back(info);
		return;
	}
	_vals.push_back(info.substr(last, index - last));
	_vals.push_back(info.substr(index + 1, info.size() - (index + 1)));
}
