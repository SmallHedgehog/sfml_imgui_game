#include "Fight.h"


void Fight::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_fight(_clientSocket, _msg, size);
}

void Fight::handleMessage_fight(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "用户交战信息处理" << std::endl;

	// 解析消息数据
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	if (msgs.size() != 5)
	{
		if (msgs.size() == 7)	// 包含正常退出和逃跑
		{
			// 交战结束消息，用户名+用户信息
			std::vector<std::string> userInfos;
			userInfos.assign(msgs.begin() + 1, msgs.end() - 1);
			UserInfo::setUserInfo(msgs[0], userInfos);

			// 缓存username->username
			if (Cache::cUsers.find(msgs[0]) != Cache::cUsers.end())
				Cache::cUsers.erase(msgs[0]);

			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_END";
			strcpy(data.data, sendData.c_str());
			MultiMessageSend::messageSend(Cache::cSockets, data);
		}
		else if (msgs.size() == 8)	// 逃跑
		{
			// 交战结束消息，用户名+用户信息
			std::vector<std::string> userInfos;
			userInfos.assign(msgs.begin() + 2, msgs.end() - 1);
			UserInfo::setUserInfo(msgs[0], userInfos);

			// 缓存username->username
			if (Cache::cUsers.find(msgs[0]) != Cache::cUsers.end())
				Cache::cUsers.erase(msgs[0]);

			// 查找被发送方的Socket
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
			if (it != Cache::cMaps.end())
			{
				// 向被发送方写入消息
				DATA_PACKAGE dPackage;
				dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
				dPackage.dataHeader.msgType = MessageType::TYPE_FIGHT;
				std::string send_data = msgs[0] + "_RUN";
				strcpy(dPackage.data, send_data.c_str());
				if (respSocket)
					respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
			}

			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_END";
			strcpy(data.data, sendData.c_str());
			MultiMessageSend::messageSend(Cache::cSockets, data);
		}
	}
	else
	{
		// 查找被发送方的Socket
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
		if (it != Cache::cMaps.end())
		{
			// 向被发送方写入消息
			DATA_PACKAGE dPackage;
			dPackage.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			dPackage.dataHeader.msgType = MessageType::TYPE_FIGHT;
			strcpy(dPackage.data, _msg);
			if (respSocket)
				respSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
		}
		else
		{
			// 查找失败，向发送方写入失败消息
			DATA_PACKAGE data;
			data.dataHeader.dataSize = sizeof(DATA_PACKAGE) - sizeof(DATA_HEADER);
			data.dataHeader.msgType = MessageType::TYPE_FIGHT;
			std::string sendData = msgs[0] + "_" + msgs[1] + "_FAIL";
			strcpy(data.data, sendData.c_str());
			if (_clientSocket)
				_clientSocket->write((char*)&data, sizeof(DATA_PACKAGE));
		}
	}
}
