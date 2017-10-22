#pragma once

/*
用户匹配逻辑处理
*/

#include "AbstractLogic.h"
#include "../../MessageType.h"
#include <iostream>


class Match : public AbstractLogic
{
public:
	// 逻辑处理，用户匹配处理
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_matchUser(Socket* _clientSocket, const char* _msg, int size);
};
