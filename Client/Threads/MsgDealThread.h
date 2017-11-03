#pragma once

#include "BaseThread.h"
#include "../MsgQueue/MsgQueue.h"

#include <functional>


class MsgDealThread : public BaseThread
{
public:
	MsgDealThread(MsgQueue* _msgQueue, std::function< void(DATA_PACKAGE&)> _callbackFunc);
	virtual void run();

private:
	std::function< void(DATA_PACKAGE&)> callbackFunc;

	MsgQueue* msgQueue;
};
