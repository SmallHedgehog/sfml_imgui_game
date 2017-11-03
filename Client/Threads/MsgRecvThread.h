#pragma once

#include "BaseThread.h"
#include "../Network/Client.h"
#include "../MsgQueue/MsgQueue.h"

#include <iostream>

/*
消息接收线程，将接收到的线程送往消息队列
*/


class MsgRecvThread : public BaseThread
{
public:
	MsgRecvThread(Client* _cli, MsgQueue* _msg);
	virtual void run();

private:
	// TCP粘包处理
	void tcpDataSplit(const char* recvData, int recvSize);
	// 处理接收到的数据
	void handleRecvData(DATA_HEADER* pHeader);

private:
	MsgQueue* msg;

	Client* cli;
};
