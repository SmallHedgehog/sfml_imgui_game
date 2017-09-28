#pragma once

#include <iostream>
#include "network/Server.h"
#include "Threads/clientThread.h"
#include "Threads/msgHandleThread.h"
#include "MsgQueue.h"
#include "MsgHandle/MsgHandle.h"


class Application
{
public:
	Application(unsigned short port, unsigned int connections);
	~Application();

public:
	void run();

private:
	Server* server;
	// std::list<Socket*> cSockets;
	// 消息队列
	MsgQueue* msg;

	// 消息处理模块
	MsgHandle* msgHandle;

	// std::list<virtualThread*> cThreads;
};

