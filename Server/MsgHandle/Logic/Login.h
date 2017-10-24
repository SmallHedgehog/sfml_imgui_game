#pragma once

/*
处理登录、注册消息类型
*/

#include "AbstractLogic.h"
#include "../Transaction/UserLogin.h"
#include "../Transaction/UserInfo.h"
#include "../../MessageType.h"
#include <iostream>


class Login : public AbstractLogic
{
public:
	// 逻辑处理，处理用户登录
	void handleMessage(Socket* _clientSocket, const char* _msg, int size,
		MessageType _msgType = MessageType::TYPE_NONE);

private:
	// 逻辑处理，处理用户注册
	void handleMessage_signup(Socket* _clientSocket, const char* _msg, int Size);
	
	// 逻辑处理，处理用户登录
	void handleMessage_signin(Socket* _clientSocket, const char* _msg, int Size);

	// 逻辑处理，处理用户登出
	void handleMessage_signout(Socket* _clientSocket, const char* _msg, int Size);
};
