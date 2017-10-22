#pragma once

/*
用户处理逻辑，获取用户详细信息等
*/

#include "AbstractLogic.h"
#include "../Transaction/UserInfo.h"
#include "../../MessageType.h"
#include <iostream>


class User : public AbstractLogic
{
public:
	// 逻辑处理，请求得到用户信息等
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	void handleMessage_UserInfo(Socket* _clientSocket, const char* _msg, int size);
};
