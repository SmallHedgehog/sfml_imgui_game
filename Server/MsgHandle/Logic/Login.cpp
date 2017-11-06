#include "Login.h"


void Login::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	switch (_msgType)
	{
	case TYPE_SIGNIN:
		// signin
		handleMessage_signin(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNUP:
		// signup
		handleMessage_signup(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNOUT:
		// signout
		handleMessage_signout(_clientSocket, _msg, size);
		break;
	default:
		break;
	}
}

void Login::handleMessage_signout(Socket* _clientSocket, const char* _msg, int Size)
{
	std::cout << "handle signout message" << std::endl;

	if (_clientSocket)
	{
		// Parser message
		std::vector<std::string> msgs;
		parserMessage(msgs, _msg);

		// Uncache in onlineUsers
		if (!msgs.empty())
		{
			std::unordered_set<std::string>::iterator it = Cache::onlineUsers.find(msgs[0]);
			if (it != Cache::onlineUsers.end())
				Cache::onlineUsers.erase(it);
		}

		int msgsSize = msgs.size();
		if (msgsSize == 1)
		{
			if (msgs[0] == "NONE")	/* abnormal signout message */
			{
				bool isFind = false;
				std::unordered_map<std::string, Socket*>::iterator it;
				for (it = Cache::cMaps.begin(); it != Cache::cMaps.end(); )
				{
					Socket* cSocket = it->second;
					if (&(*(cSocket)) == &(*(_clientSocket)))
					{
						msgs[0] = it->first;
						isFind = true;
						it = Cache::cMaps.erase(it);
						break;
					}
					else
						++it;
				}
				if (it == Cache::cMaps.end() && !isFind)
					return;
			}
		}
		else if (msgsSize == 2)		/* normal signout message */
		{
			std::unordered_map<std::string, Socket*>::iterator it;
			for (it = Cache::cMaps.begin(); it != Cache::cMaps.end();)
			{
				if (msgs[0] == it->first)
				{
					it = Cache::cMaps.erase(it);
					break;
				}
				else
					++it;
			}
		}
		// Update the user's online infos
		UserLogin::updateUserOnlineRecord(msgs[0], 0);	// 0 stand for the user isn't online
		// Uncache this user in onlineUsers
		std::unordered_set<std::string>::iterator it = Cache::onlineUsers.find(msgs[0]);
		if (it != Cache::onlineUsers.end())
			Cache::onlineUsers.erase(it);
		// Uncache users in isForMatchU
		it = Cache::isForMatchU.find(msgs[0]);
		if (it != Cache::isForMatchU.end())
			Cache::isForMatchU.erase(it);

		// This user is fighting, find the match user
		std::unordered_map<std::string, std::string>::iterator item = Cache::fightUsers.find(msgs[0]);
		if (item != Cache::fightUsers.end())
		{
			Socket* respSocket = nullptr;
			std::unordered_map<std::string, Socket*>::iterator userS = Cache::cMaps.begin();
			for (; userS != Cache::cMaps.end(); ++userS)
			{
				if (userS->first == item->second)
				{
					respSocket = userS->second;
					break;
				}
			}
			if (respSocket)
			{
				DATA_PACKAGE dPackage;
				std::string text = msgs[0] + "_" + item->second;
				FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightExitMsg, text.c_str(), dPackage);

				respSocket->write((char*)&dPackage, sizeof(dPackage));
			}

			// Update databae table of UserInfo
			std::string MatchedUser = item->second;
			Cache::fightUsers.erase(msgs[0]);
			Cache::fightUsers.erase(MatchedUser);

			// Bordcast all online users
			DATA_PACKAGE notify_data;
			std::string sendData = msgs[0] + "_" + MatchedUser;
			FilldPackage(MessageType::TYPE_FIGHT, SMessageTypeFlags_FightUserToOnline, sendData.c_str(), notify_data);
			MultiMessageSend::messageSend(Cache::cSockets, notify_data);
		}

		// Bordcast all online users
		DATA_PACKAGE notify_data;
		FilldPackage(MessageType::TYPE_SIGNOUT, SMessageTypeFlags_NIsTheUserSingout, msgs[0].c_str(), notify_data);
		MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
	}
}

void Login::handleMessage_signin(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "handle signin message" << std::endl;

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNIN;

	// Parser message
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_Signin message parameter error!");
	}
	else
	{
		// Query this user in database
		std::string passwd;
		if (UserLogin::userIsExistsAndGetPasswd(msgs[0], passwd))
		{
			// Need to valitate this user's password
			if (passwd == msgs[1])
			{
				// Query this user whether have signin
				if (Cache::onlineUsers.find(msgs[0]) != Cache::onlineUsers.end())
				{
					std::cout << "this user have signined" << std::endl;
					strcpy(dPackage.data, "REPEAT_You have signined in other place!");
				}
				else
				{
					std::cout << "signin successfully" << std::endl;
					strcpy(dPackage.data, "SUCCESS_NONE");

					// Update the user's online infos
					UserLogin::updateUserOnlineRecord(msgs[0], 1);	// 1 stand for the user is online

					// Cache username->cSocket
					Cache::cMaps[msgs[0]] = _clientSocket;

					// Cache in onlineUsers
					Cache::onlineUsers.insert(msgs[0]);

					// Bordcast all online users
					DATA_PACKAGE notify_data;
					std::string sendData = msgs[0];
					FilldPackage(MessageType::TYPE_SIGNIN, sendData.c_str(), notify_data);
					MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
				}
			}
			else
			{
				std::cout << "valitate the user's password error" << std::endl;
				strcpy(dPackage.data, "PASSWORD_User's password error!");
			}
		}
		else
		{
			std::cout << "find the user's username error" << std::endl;
			strcpy(dPackage.data, "USERNAME_User's username error!");
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}

void Login::handleMessage_signup(Socket* _clientSocket, const char* _mgs, int size)
{
	std::cout << "handle signup message" << std::endl;
	
	/*
		First, validate the user have already exits, if not, turn step second.
		Second, insert user's infos into database.
		Third,	send message to the user
	*/

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNUP;

	// Parser message
	std::vector<std::string> msgs;
	parserMessage(msgs, _mgs);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_SIGNUP MESSAGE PARAMETER ERROR!");
	}
	else
	{
		// Query this user in databases
		bool isFind = UserLogin::userIsExists(msgs[0]);
		if (isFind)
		{
			std::cout << "this user is EXISTS" << std::endl;
			// send message to this user about signup infos
			strcpy(dPackage.data, "EXISTS_[error] this user have exists!");
		}
		else
		{
			if (UserLogin::insertUserRecord(msgs[0], msgs[1]))
			{
				std::cout << "this user signup SUCCESS" << std::endl;
				strcpy(dPackage.data, "SUCCESS_User signup successfully, now you can signin!");
				// insert a record in UserDataOnline
				UserLogin::insertUserOnlineRecord(msgs[0]);
				// insert a record in UserInfo
				UserInfo::setUserInfo(msgs[0]);

				// Bordcast (notify have user signup)
				DATA_PACKAGE notify_data;
				FilldPackage(MessageType::TYPE_SIGNUP, msgs[0].c_str(), notify_data);
				MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
			}
			else
			{
				std::cout << "this user signup FAILD" << std::endl;
				// send message to this user about signup infos
				strcpy(dPackage.data, "UNKNOWN_INSERT USER'S DATA ERROR!");
			}
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}
