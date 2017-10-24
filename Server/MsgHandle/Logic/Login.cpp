#include "Login.h"


void Login::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	switch (_msgType)
	{
	case TYPE_SIGNIN:
		// 登录逻辑处理
		handleMessage_signin(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNUP:
		// 注册逻辑处理
		handleMessage_signup(_clientSocket, _msg, size);
		break;
	case TYPE_SIGNOUT:
		// 登出逻辑处理
		handleMessage_signout(_clientSocket, _msg, size);
		break;
	default:
		break;
	}
}

void Login::handleMessage_signout(Socket* _clientSocket, const char* _msg, int Size)
{
	std::cout << "登出消息类型处理" << std::endl;

	if (_clientSocket)
	{
		// 解析消息
		std::vector<std::string> msgs;
		parserMessage(msgs, _msg);

		if (msgs.empty())
		{
			return;
		}
		else
		{
			if (msgs[0] == "NONE")
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
				{
					return;
				}
			}
		}
		// 更新用户在线信息
		UserLogin::updateUserOnlineRecord(msgs[0], 0);	// 0 stand for the user isn't online
		// 通知所有在线用户
		DATA_PACKAGE notify_data;
		notify_data.dataHeader.dataSize = sizeof(notify_data) - sizeof(DATA_HEADER);
		notify_data.dataHeader.msgType = MessageType::TYPE_SIGNOUT;
		strcpy(notify_data.data, msgs[0].c_str());
		MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
	}
}

void Login::handleMessage_signin(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "登录消息类型处理" << std::endl;

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNIN;

	// 解析消息
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_Signin message parameter error!");
	}
	else
	{
		// 在数据库中查询是否有该用户
		std::string passwd;
		if (UserLogin::userIsExistsAndGetPasswd(msgs[0], passwd))
		{
			// 用户查询成功，需要验证用户密码
			if (passwd == msgs[1])
			{
				std::cout << "用户登录，验证用户名、密码成功" << std::endl;
				strcpy(dPackage.data, "SUCCESS_NONE");
				
				// 更新用户在线信息
				UserLogin::updateUserOnlineRecord(msgs[0], 1);	// 1 stand for the user is online
				
				// 缓存username->cSocket
				Cache::cMaps[msgs[0]] = _clientSocket;

				// 通知所有在线用户
				DATA_PACKAGE notify_data;
				notify_data.dataHeader.dataSize = sizeof(notify_data) - sizeof(DATA_HEADER);
				notify_data.dataHeader.msgType = MessageType::TYPE_SIGNIN;
				std::string sendData = msgs[0] + "_NONE";
				strcpy(notify_data.data, sendData.c_str());
				MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, notify_data);
			}
			else
			{
				std::cout << "用户登录，验证密码失败" << std::endl;
				strcpy(dPackage.data, "PASSWORD_User's password error!");
			}
		}
		else
		{
			std::cout << "用户登录，查找用户名失败" << std::endl;
			strcpy(dPackage.data, "USERNAME_User's username error!");
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}

void Login::handleMessage_signup(Socket* _clientSocket, const char* _mgs, int size)
{
	// 注册消息类型处理
	std::cout << "注册消息类型处理" << std::endl;
	
	/* 查询数据库中是否已有该用户
	   如果没有该用户，则向数据库中插入一条用户信息
	   向注册对象发送注册成功或失败的消息
	*/
	// 解析发送过来的消息

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_SIGNUP;

	std::vector<std::string> msgs;
	parserMessage(msgs, _mgs);
	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_SIGNUP MESSAGE PARAMETER ERROR!");
	}
	else
	{
		// 在数据库中查询是否有该用户
		bool isFind = UserLogin::userIsExists(msgs[0]);
		if (isFind)
		{
			std::cout << "EXISTS" << std::endl;
			// 向注册对象发送注册失败消息
			strcpy(dPackage.data, "EXISTS_THE USER HAVE EXSIT!");
		}
		else
		{
			if (UserLogin::insertUserRecord(msgs[0], msgs[1]))
			{
				std::cout << "SUCCESS" << std::endl;
				// 向注册对象发送注册成功消息
				strcpy(dPackage.data, "SUCCESS_NONE");
				// 向用户在线信息表中插入一条用户在线信息
				UserLogin::insertUserOnlineRecord(msgs[0]);
				// 向用户信息表中插入一条记录
				UserInfo::setUserInfo(msgs[0]);
			}
			else
			{
				std::cout << "FAILD" << std::endl;
				// 向注册对象发送注册失败消息
				strcpy(dPackage.data, "UNKNOWN_INSERT USER'S DATA ERROR!");
			}
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(dPackage));
}
