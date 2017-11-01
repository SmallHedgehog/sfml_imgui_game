#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

#include "../MessageType.h"


class MsgQueue
{
public:
	MsgQueue();
	// ~MsgQueue();

	MsgQueue& operator << (const DATA_PACKAGE& _msg);
	MsgQueue& operator >> (DATA_PACKAGE& _msg);

	void pop(DATA_PACKAGE& _msg);
	bool try_pop(DATA_PACKAGE& _msg);
	bool empty();
	unsigned int size();

private:
	std::queue<DATA_PACKAGE> msgs;

	// 同步互斥
	std::mutex _mutex;
	std::condition_variable _condition;
};
