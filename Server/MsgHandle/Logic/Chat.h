#pragma once

#include "AbstractLogic.h"
#include "../../MessageType.h"
#include <iostream>


class Chat : public AbstractLogic
{
public:
	// 逻辑处理，请求得到用户信息等
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_ChatInfo(Socket* _clientSocket, const char* _msg, int size);
	void ParserMsg(std::vector<std::string>& vec, const char* _msg);
};
