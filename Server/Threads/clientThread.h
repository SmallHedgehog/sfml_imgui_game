#pragma once

#include <iostream>
#include "virtualThread.h"
#include "../Network/Socket.h"
#include "../MessageType.h"
#include "../MsgQueue.h"


/*
处理来自客户端的连接（clientThread），每次客户端连接时生成客户端线程处理
*/

class clientThread : public virtualThread
{
public:
	clientThread(Socket* _clientSocket, MsgQueue* _msg);
	// TCP粘包处理
	void tcpDataSplit(const char* recvData, int recvSize);
	// 处理接收到的数据
	void handleRecvData(DATA_HEADER* pHeader);
	virtual void run();

	// 获取客户端Socket
	Socket* get_cSocket() { return clientSocket; }

private:
	Socket* clientSocket;
	// 消息队列
	MsgQueue* msgQueue;

	// 连接是否关闭
	bool isClosed;
};

