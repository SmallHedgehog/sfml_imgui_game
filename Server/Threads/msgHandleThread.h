#pragma once

#include "virtualThread.h"
#include "../MsgHandle/MsgHandle.h"
#include "../MsgQueue.h"


// 从消息队列中获取一条消息

class msgHandleThread : public virtualThread
{
public:
	msgHandleThread(MsgQueue* _msg, MsgHandle* _msgHandle);
	// ~msgHandleThread();

	virtual void run();

private:
	// 消息队列
	MsgQueue* msgQueue;

	// 消息处理模块
	MsgHandle* msgHandle;
};

