#include "User.h"


void User::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_UserInfo(_clientSocket, _msg, size);
}

void User::handleMessage_UserInfo(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "获取用户信息" << std::endl;

	// 解析消息数据
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_USER;

	if (msgs.size() != 2)
	{
		strcpy(dPackage.data, "PARAMETOR_USER MESSAGE PARAMETER ERROR!");
	}
	else
	{
		// 获取用户数据
		std::vector<std::string> userinfos;
		if (UserInfo::findUserInfo(msgs[1], userinfos))
		{
			std::string infos = "SUCCESS_";
			for (int i = 0; i < userinfos.size(); ++i)
			{
				if (i == userinfos.size() - 1)
					infos += userinfos[i];
				else
					infos += userinfos[i] + "_";
			}
			strcpy(dPackage.data, infos.c_str());
		}
		else
		{
			strcpy(dPackage.data, "ERROR_GET THIS USER'S DATA ERROR!");
		}
	}

	if (_clientSocket)
		_clientSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
}
