#pragma once

/*
定义抽象消息处理方法，派生类具体实现抽象方法，这里只定义
（1）接收客户端连接、收到客户端消息；
（2）客户端连接关闭的消息；
等抽象处理方法。
*/

#include "../MsgQueue.h"


class AbstractHandle
{
public:
	// 客户端连接、收到客户端消息
	virtual void handleCliConn_MsgRecv(MESSAGE& _msg) = 0;

	// 客户端连接关闭
	virtual void handleCliClose(Socket* _clientSocket, const char* _error) = 0;
};
