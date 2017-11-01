#include "MsgQueue.h"


MsgQueue::MsgQueue()
{
	// do someting
}

MsgQueue& MsgQueue::operator<< (const DATA_PACKAGE& _msg)
{
	std::lock_guard<std::mutex> lock(_mutex);
	msgs.push(_msg);
	_condition.notify_one();

	return *this;
}

MsgQueue& MsgQueue::operator >> (DATA_PACKAGE& _msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [this] {
		return !msgs.empty();
	});

	_msg = msgs.front();
	msgs.pop();

	return *this;
}

bool MsgQueue::empty()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return msgs.empty();
}

unsigned int MsgQueue::size()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return msgs.size();
}

void MsgQueue::pop(DATA_PACKAGE& _msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [this] {
		return !msgs.empty();
	});

	_msg = msgs.front();
	msgs.pop();
}

bool MsgQueue::try_pop(DATA_PACKAGE& _msg)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (msgs.empty())
	{
		return false;
	}
	_msg = msgs.front();
	msgs.pop();

	return true;
}
