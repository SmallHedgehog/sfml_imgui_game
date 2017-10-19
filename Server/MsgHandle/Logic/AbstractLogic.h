#pragma once

/*
游戏服务器端逻辑处理（Logic模块）
	（1）用户登录、注册；
	（2）用户信息处理；
	（3）用户匹配处理；
	（4）用户交战时信息处理
*/

#include <vector>
#include <string>
#include "../../Network/Socket.h"
#include "../../MessageType.h"
#include "../../UCache/Cache.h"
#include "../MultiMessageSend.h"


class AbstractLogic
{
public:
	virtual void handleMessage(Socket* _clientSocket, const char* _msg, int size, 
		MessageType _msgType = MessageType::TYPE_NONE) = 0;

	// 消息解析（消息使用'_'连接）
	void parserMessage(std::vector<std::string>& _msgs, const char* _msg)
	{
		std::string msg(_msg);

		size_t last = 0;
		size_t index = msg.find_first_of('_', last);
		while (index != std::string::npos)
		{
			_msgs.push_back(msg.substr(last, index - last));
			last = index + 1;
			index = msg.find_first_of('_', last);
		}
		if (index - last > 0)
		{
			_msgs.push_back(msg.substr(last, index - last));
		}
	}
};
