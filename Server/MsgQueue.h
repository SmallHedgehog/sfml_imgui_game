#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Network/Socket.h"
#include "MessageType.h"


typedef struct msg
{
	Socket* cSocket;
	DATA_PACKAGE data;
	
	msg() {}
	msg(Socket* _s, const DATA_PACKAGE& _data)
	{
		cSocket = _s;
		data = _data;
	}

	msg& operator = (const msg& _msg)
	{
		if (this != &_msg)
		{
			cSocket = _msg.cSocket;
			data = _msg.data;
		}
		return *this;
	}
}MESSAGE;

class MsgQueue
{
public:
	MsgQueue();
	// ~MsgQueue();

	MsgQueue& operator << (const MESSAGE& _msg);
	MsgQueue& operator >> (MESSAGE& _msg);

	void pop(MESSAGE& _msg);
	bool try_pop(MESSAGE& _msg);
	bool empty();
	unsigned int size();

private:
	std::queue<MESSAGE> msgs;

	// Í¬²½»¥³â
	std::mutex _mutex;
	std::condition_variable _condition;
};
