#include "Match.h"


void Match::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_matchUser(_clientSocket, _msg, size);
}

void Match::handleMessage_matchUser(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "handle user's match message" << std::endl;

	// Parser message
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	int msgsSize = msgs.size();
	if (msgsSize == 1)
	{
		/*
		if (Cache::isForMatchU.find(msgs[0]) != Cache::isForMatchU.end())
		{
			Cache::isForMatchU.erase(msgs[0]);
			DATA_PACKAGE dPackage;
			std::string sendData = msgs[0] + "_online";
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserToOnline, sendData.c_str(), dPackage);
			MultiMessageSend::messageSend(Cache::cSockets, dPackage);
		}
		*/
	}
	else if (msgsSize == 2)
	{
		bool sign	= false;	/* this user or match user haved matched */
		bool isFind = false;	/* whether find the match user */
		if (Cache::isForMatchU.find(msgs[0]) != Cache::isForMatchU.end() ||
			Cache::isForMatchU.find(msgs[1]) != Cache::isForMatchU.end() ||
			Cache::fightUsers.find(msgs[0]) != Cache::fightUsers.end() || Cache::fightUsers.find(msgs[1]) != Cache::fightUsers.end())
		{
			sign = true;		/* have matched or users is fighting */
		}
		else
		{
			// Get the match user's Socket
			Socket* respSocket = nullptr;
			std::unordered_map<std::string, Socket*>::iterator it = Cache::cMaps.begin();
			for (; it != Cache::cMaps.end(); ++it)
			{
				// Have found the match user's Socket
				if (it->first == msgs[1])
				{
					respSocket = it->second;
					break;
				}
			}
			if (it == Cache::cMaps.end())
			{
				// Not find the match user's Socket
				sign = true;
			}
			else
			{
				// Send match message to matched user
				if (respSocket)	/* isFind && !sign stand for this state */
				{
					isFind = true;	
					DATA_PACKAGE dPackage;
					std::string sendData = msgs[1] + "_" + msgs[0];
					FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserBeMatched, sendData.c_str(), dPackage);

					respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
				}
				else
					sign = true;
			}
		}
		if (sign)
		{
			DATA_PACKAGE data;
			std::string send_data = msgs[0] + "_" + msgs[1];
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserToMatch_FAI, send_data.c_str(), data);
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
		}
		else if (!sign && isFind)
		{
			// To cache the user for matching
			Cache::isForMatchU.insert(msgs[0]);	/* the user */
			Cache::isForMatchU.insert(msgs[1]);	/* the matched user */

			DATA_PACKAGE data;
			std::string send_data = msgs[0] + "_" + msgs[1];
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserToMatch_SUC, send_data.c_str(), data);
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
			
			// Bordcast(notify another users about the user is matching)
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserIsForMatching, send_data.c_str(), data);
			MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, data);
		}
	}
	else if (msgsSize == 3)
	{
		// Uncache the user and the match user in Cache::isForMatchU
		std::unordered_set<std::string>::iterator it = Cache::isForMatchU.find(msgs[1]);
		if (it != Cache::isForMatchU.end())
			Cache::isForMatchU.erase(it);
		it = Cache::isForMatchU.find(msgs[2]);
		if (it != Cache::isForMatchU.end())
			Cache::isForMatchU.erase(it);

		if (msgs[0] == "REJECT" || msgs[0] == "OVERTIME")	/* From the matched user */
		{
			// Bordcast to all online users
			DATA_PACKAGE notify_data;
			std::string send_data = msgs[1] + "_" + msgs[2];
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_RejectOrOvertime, send_data.c_str(), notify_data);
			MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
		}
		else if (msgs[0] == "AGREE")						/* From the matched user */
		{
			// Cache the fight users
			if (Cache::fightUsers.find(msgs[1]) == Cache::fightUsers.end() &&
				Cache::fightUsers.find(msgs[2]) == Cache::fightUsers.end())
			{
				Cache::fightUsers[msgs[1]] = msgs[2];
				Cache::fightUsers[msgs[2]] = msgs[1];
			}

			// Bordcast to all online users
			DATA_PACKAGE notify_data;
			std::string send_data = msgs[1] + "_" + msgs[2];
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_MatchedUserAgree, send_data.c_str(), notify_data);
			MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
		}
		else if (msgs[0] == "COMPLETE")						/* From the user */
		{
			// Bordcast to all online users
			DATA_PACKAGE notify_data;
			std::string send_data = msgs[1] + "_" + msgs[2];
			FilldPackage(MessageType::TYPE_MATCH, SMessageTypeFlags_UserToMatch_FAI, send_data.c_str(), notify_data);
			MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
		}
	}
}
